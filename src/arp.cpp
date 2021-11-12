#include "arp.h"
#include <algorithm>

auto ARP::parse(const std::vector<uint8_t>& packet) -> Header {
    Header header{};

    auto begin_it = packet.begin();
    auto end_it = std::next(begin_it, 2);
    std::move(begin_it, end_it, header.hrd.begin());

    begin_it = end_it;
    end_it = std::next(begin_it, 2);
    std::move(begin_it, end_it, header.pro.begin());

    begin_it = end_it;
    end_it = std::next(begin_it, 1);
    header.hln = *begin_it;

    begin_it = end_it;
    end_it = std::next(begin_it, 1);
    header.pln = *begin_it;

    begin_it = end_it;
    end_it = std::next(begin_it, 2);
    std::move(begin_it, end_it, header.op.begin());

    if (header.hln > 0) {
        header.sha.reserve(header.hln);
        begin_it = end_it;
        end_it = std::next(begin_it, header.hln);
        std::move(begin_it, end_it, std::back_inserter(header.sha));
    }

    if (header.pln > 0) {
        header.spa.reserve(header.pln);
        begin_it = end_it;
        end_it = std::next(begin_it, header.pln);
        std::move(begin_it, end_it, std::back_inserter(header.spa));
    }

    if (header.hln > 0) {
        header.tha.reserve(header.hln);
        begin_it = end_it;
        end_it = std::next(begin_it, header.hln);
        std::move(begin_it, end_it, std::back_inserter(header.tha));
    }

    if (header.pln > 0) {
        header.tpa.reserve(header.pln);
        begin_it = end_it;
        end_it = std::next(begin_it, header.pln);
        std::move(begin_it, end_it, std::back_inserter(header.tpa));
    }
    return header;
}

auto get_address(const std::string& cmd, char delim, unsigned int octets, int base = 10)
    -> std::vector<uint8_t> {
    auto fp = popen(cmd.c_str(), "r");
    if (fp == nullptr) {
        return {};
    }

    std::string ret;
    int c;
    while ((c = fgetc(fp)) != EOF)
        ret += static_cast<char>(c);

    pclose(fp);

    std::vector<uint8_t> addr;
    auto start = 0;
    auto end = ret.find(delim);
    for (auto i = 0u; i < octets; ++i) {
        auto octet = ret.substr(start, end - start);
        addr.push_back(stoi(octet, nullptr, base));
        start = end + 1;
        end = i < octets - 1 ? ret.find(delim, start) : ret.length();
    }
    return addr;
}

auto ARP::handle(std::string_view if_name, Header&& header) -> std::vector<uint8_t> {
    const auto h = (header.hrd[0] << 8) + header.hrd[1];
    if (static_cast<HardwareType>(h) ==
        HardwareType::ETHERNET) {  // Do I have the hardware type in ar$hrd?
        const auto p = (header.pro[0] << 8) + header.pro[1];
        if (static_cast<ProtocolType>(p) ==
            ProtocolType::IPV4) {  // Do I speak the protocol in ar$pro?
            auto merge_flag = false;
            /* If the pair <protocol type, sender protocol address> is
            already in my translation table, update the sender
            hardware address field of the entry with the new
            information in the packet and set Merge_flag to true. */
            for (auto& [protocol_type, sender_protocol_address, sender_hardware_address] :
                m_translation_table) {
                if (header.pro == protocol_type && header.spa == sender_protocol_address) {
                    sender_hardware_address = header.sha;
                    merge_flag = true;
                }
            }

            // Get protocol address
            std::string protocol_cmd = "ip -br addr show dev ";
            protocol_cmd.append(if_name);
            protocol_cmd.append(" | tr -s ' ' | cut -d ' ' -f 3 | cut -d '/' -f 1");
            auto protocol_address = get_address(protocol_cmd, '.', 4);

            if (header.tpa == protocol_address) {  // Am I the target protocol address?
                if (!merge_flag)
                    m_translation_table.push_back({header.pro, header.spa, header.sha});

                const auto o = (header.op[0] << 8) + header.op[1];
                if (static_cast<OpCode>(o) == OpCode::REQUEST) {
                    /* Swap hardware and protocol fields, putting the local
                    hardware and protocol addresses in the sender fields.
                    Set the ar$op field to ares_op$REPLY
                    Send the packet to the (new) target hardware address on
                    the same hardware on which the request was received. */
                    std::swap(header.spa, header.tpa);
                    header.tha = header.sha;

                    // Get hardware address
                    std::string hardware_cmd = "cat /sys/class/net/";
                    hardware_cmd.append(if_name);
                    hardware_cmd.append("/address");
                    header.sha = get_address(hardware_cmd, ':', 6, 16);

                    const auto r = static_cast<uint16_t>(OpCode::REPLY);
                    header.op[0] = (r >> 8);
                    header.op[1] = (r & 0xFF);

                    std::vector<uint8_t> reply{};
                    reply.reserve((header.hrd.size() + header.pro.size() + 1 + 1 +
                                   header.op.size() + header.sha.size() + header.spa.size() +
                                   header.tha.size() + header.tpa.size()));
                    std::ranges::copy(header.hrd, std::back_inserter(reply));
                    std::ranges::copy(header.pro, std::back_inserter(reply));
                    reply.push_back(header.hln);
                    reply.push_back(header.pln);
                    std::ranges::copy(header.op, std::back_inserter(reply));
                    std::ranges::copy(header.sha, std::back_inserter(reply));
                    std::ranges::copy(header.spa, std::back_inserter(reply));
                    std::ranges::copy(header.tha, std::back_inserter(reply));
                    std::ranges::copy(header.tpa, std::back_inserter(reply));

                    return reply;
                }
            }
        }
    }
    return {};
}
