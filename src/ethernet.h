#ifndef ETHERNET_H
#define ETHERNET_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

class Ethernet {
public:
    enum struct EtherType : uint16_t { ARP = 0x0806 };
    explicit Ethernet(const std::vector<uint8_t>& frame);
    [[nodiscard]] auto to_string() const -> std::string;
    [[nodiscard]] auto get_dest() const -> std::array<uint8_t, 6>;
    [[nodiscard]] auto get_src() const -> std::array<uint8_t, 6>;
    [[nodiscard]] auto get_ethertype() const -> std::array<uint8_t, 2>;
    [[nodiscard]] auto get_payload() const -> std::vector<uint8_t>;
    [[nodiscard]] auto get_crc() const -> std::array<uint8_t, 4>;
#ifdef FCS_CAPTURED
    [[nodiscard]] auto is_valid() const -> bool;
#endif
    void handle() const;
private:
    std::array<uint8_t, 6> m_dest{};
    std::array<uint8_t, 6> m_src{};
    std::array<uint8_t, 2> m_ethertype{};
    std::vector<uint8_t> m_payload{};
    std::array<uint8_t, 4> m_crc{};
};

#endif
