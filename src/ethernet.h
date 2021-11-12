#ifndef ETHERNET_H
#define ETHERNET_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include "arp.h"

class Ethernet {
public:
    enum struct EtherType : uint16_t { ARP = 0x0806 };
    struct Header {
        std::array<uint8_t, 6> dest{};
        std::array<uint8_t, 6> src{};
        std::array<uint8_t, 2> ethertype{};
        std::vector<uint8_t> payload{};
        std::array<uint8_t, 4> crc{};
    };
    [[nodiscard]] static auto parse(const std::vector<uint8_t>& frame) -> Header;
    [[nodiscard]] auto handle(std::string_view if_name, Header header) -> std::vector<uint8_t>;
#ifdef FCS_CAPTURED
    [[nodiscard]] static auto is_valid(const Header& header) -> bool;
#endif
private:
    ARP m_arp{};
};

#endif
