#include "ethernet.h"
#include <sstream>

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
    end_it = frame.end() - 4;
    std::move(begin_it, end_it, std::back_inserter(m_payload));
    m_payload.resize(std::distance(begin_it, end_it));

    begin_it = end_it;
    end_it = frame.end();
    std::move(begin_it, end_it, m_crc.begin());
}

std::string Ethernet::to_string() const {
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

std::array<uint8_t, 6> Ethernet::get_dest() const {
    return m_dest;
};

std::array<uint8_t, 6> Ethernet::get_src() const {
    return m_src;
};

std::array<uint8_t, 2> Ethernet::get_ethertype() const {
    return m_ethertype;
};

std::vector<uint8_t> Ethernet::get_payload() const {
    return m_payload;
};

std::array<uint8_t, 4> Ethernet::get_crc() const {
    return m_crc;
};
