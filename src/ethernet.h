#ifndef ETHERNET_H
#define ETHERNET_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

class ethernet {
public:
    explicit ethernet(const std::vector<uint8_t>& frame);
    [[nodiscard]] std::string to_string() const;
private:
    std::array<uint8_t, 6> m_dest{};
    std::array<uint8_t, 6> m_src{};
    std::array<uint8_t, 2> m_ethertype{};
    std::vector<uint8_t> m_payload{};
    std::array<uint8_t, 4> m_crc{};
};

#endif
