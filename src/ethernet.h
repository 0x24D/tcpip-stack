#ifndef ETHERNET_H
#define ETHERNET_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

class Ethernet {
public:
    explicit Ethernet(const std::vector<uint8_t>& frame);
    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] std::array<uint8_t, 6> get_dest() const;
    [[nodiscard]] std::array<uint8_t, 6> get_src() const;
    [[nodiscard]] std::array<uint8_t, 2> get_ethertype() const;
    [[nodiscard]] std::vector<uint8_t> get_payload() const;
    [[nodiscard]] std::array<uint8_t, 4> get_crc() const;
#ifdef FCS_CAPTURED
    [[nodiscard]] bool is_valid() const;
#endif
private:
    std::array<uint8_t, 6> m_dest{};
    std::array<uint8_t, 6> m_src{};
    std::array<uint8_t, 2> m_ethertype{};
    std::vector<uint8_t> m_payload{};
    std::array<uint8_t, 4> m_crc{};
};

#endif
