#include "ethernet.h"
#include <sstream>
#include "arp.h"
#ifdef FCS_CAPTURED
#include "crc_cpp.h"
#endif

Ethernet::Ethernet(const std::vector<uint8_t>& frame) {
    auto begin_it = frame.begin();
    auto end_it = std::next(begin_it, 6);
    std::move(begin_it, end_it, m_dest.begin());

    begin_it = end_it;
    end_it = std::next(begin_it, 6);
    std::move(begin_it, end_it, m_src.begin());

    begin_it = end_it;
    end_it = std::next(begin_it, 2);
    std::move(begin_it, end_it, m_ethertype.begin());

    m_payload.reserve(1500);
    begin_it = end_it;
#ifdef FCS_CAPTURED
    end_it = frame.end() - 4;
#else
    end_it = frame.end();
#endif
    std::move(begin_it, end_it, std::back_inserter(m_payload));
    m_payload.resize(std::distance(begin_it, end_it));

#ifdef FCS_CAPTURED
    begin_it = end_it;
    end_it = frame.end();
    std::move(begin_it, end_it, m_crc.begin());
#endif
}

auto Ethernet::to_string() const -> std::string {
    std::stringstream ss{};
    for (std::size_t i = 0; i < m_src.size(); ++i) {
        ss << std::hex << (m_src[i] >> 4) << (m_src[i] & 0xF);
        if (i != m_src.size() - 1)
            ss << ':';
    }
    ss << " -> ";
    for (std::size_t i = 0; i < m_dest.size(); ++i) {
        ss << std::hex << (m_dest[i] >> 4) << (m_dest[i] & 0xF);
        if (i != m_dest.size() - 1)
            ss << ':';
    }
    ss << " (0x";
    for (const auto i : m_ethertype) {
        ss << std::hex << (i >> 4) << (i & 0xF);
    }
    ss << ")\n";
    return ss.str();
}

auto Ethernet::get_dest() const -> std::array<uint8_t, 6> {
    return m_dest;
}

auto Ethernet::get_src() const -> std::array<uint8_t, 6> {
    return m_src;
}

auto Ethernet::get_ethertype() const -> std::array<uint8_t, 2> {
    return m_ethertype;
}

auto Ethernet::get_payload() const -> std::vector<uint8_t> {
    return m_payload;
}

auto Ethernet::get_crc() const -> std::array<uint8_t, 4> {
    return m_crc;
}

#ifdef FCS_CAPTURED
auto Ethernet::is_valid() const -> bool {
    crc_cpp::crc32_bzip2 crc_gen;

    for (const auto i : m_dest)
        crc_gen.update(i);

    for (const auto i : m_src)
        crc_gen.update(i);

    for (const auto i : m_ethertype)
        crc_gen.update(i);

    for (const auto i : m_payload)
        crc_gen.update(i);

    uint32_t our_crc = (m_crc[0] << 24) + (m_crc[1] << 16) + (m_crc[2] << 8) + (m_crc[3]);
    return our_crc == crc_gen.final();
}
#endif

void Ethernet::handle() const {
    const auto type = static_cast<EtherType>((m_ethertype[0] << 8) + (m_ethertype[1]));
    if (type == EtherType::ARP) {
        auto arp = ARP(m_payload);
        arp.handle();
    }
}
