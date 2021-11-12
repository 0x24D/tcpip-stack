#include "ethernet.h"
#include <algorithm>
#include "arp.h"
#ifdef FCS_CAPTURED
#include "crc_cpp.h"
#endif

auto Ethernet::parse(const std::vector<uint8_t>& frame) -> Header {
    Header header{};

    auto begin_it = frame.begin();
    auto end_it = std::next(begin_it, 6);
    std::move(begin_it, end_it, header.dest.begin());

    begin_it = end_it;
    end_it = std::next(begin_it, 6);
    std::move(begin_it, end_it, header.src.begin());

    begin_it = end_it;
    end_it = std::next(begin_it, 2);
    std::move(begin_it, end_it, header.ethertype.begin());

    header.payload.reserve(1500);
    begin_it = end_it;
#ifdef FCS_CAPTURED
    end_it = frame.end() - 4;
#else
    end_it = frame.end();
#endif
    std::move(begin_it, end_it, std::back_inserter(header.payload));
    header.payload.resize(std::distance(begin_it, end_it));

#ifdef FCS_CAPTURED
    begin_it = end_it;
    end_it = frame.end();
    std::move(begin_it, end_it, header.crc.begin());
#endif
    return header;
}

auto Ethernet::handle(std::string_view if_name, Header header) -> std::vector<uint8_t> {
    const auto type = static_cast<EtherType>((header.ethertype[0] << 8) + (header.ethertype[1]));
    if (type == EtherType::ARP) {
        const auto arp_reply = m_arp.handle(if_name, ARP::parse(header.payload));
        if (!arp_reply.empty()) {
            header.dest = header.src;
            // TODO: This is likely not the correct way to do this, as per the standard, but it
            // works for now. Get Header::src from ARP reply
            const auto src = ARP::parse(arp_reply).sha;
            for (auto i = 0u; i < header.src.size(); ++i)
                header.src[i] = src[i];

            header.payload = arp_reply;
            auto payload_size = header.payload.size();
            for (auto i = payload_size; i < 46; ++i)
                header.payload.push_back(0x00);

            std::vector<uint8_t> reply{};
            reply.reserve((header.dest.size() + header.src.size() + header.ethertype.size() +
                           header.payload.size()));
            std::ranges::copy(header.dest, std::back_inserter(reply));
            std::ranges::copy(header.src, std::back_inserter(reply));
            std::ranges::copy(header.ethertype, std::back_inserter(reply));
            std::ranges::copy(header.payload, std::back_inserter(reply));
            return reply;
        }
    }
    return {};
}

#ifdef FCS_CAPTURED
auto Ethernet::is_valid(const Header& header) -> bool {
    crc_cpp::crc32_bzip2 crc_gen;

    for (const auto i : header.dest)
        crc_gen.update(i);

    for (const auto i : header.src)
        crc_gen.update(i);

    for (const auto i : header.ethertype)
        crc_gen.update(i);

    for (const auto i : header.payload)
        crc_gen.update(i);

    uint32_t our_crc =
        (header.crc[0] << 24) + (header.crc[1] << 16) + (header.crc[2] << 8) + (header.crc[3]);
    return our_crc == crc_gen.final();
}
#endif
