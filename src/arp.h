#ifndef ARP_H
#define ARP_H

#include <array>
#include <cstdint>
#include <string_view>
#include <tuple>
#include <vector>

class ARP {
public:
    enum struct HardwareType : uint16_t { ETHERNET = 0x0001 };
    enum struct ProtocolType : uint16_t { IPV4 = 0x0800 };
    enum struct OpCode : uint16_t { REQUEST = 0x0001, REPLY = 0x0002 };
    struct Header {
        std::array<uint8_t, 2> hrd{};
        std::array<uint8_t, 2> pro{};
        uint8_t hln;
        uint8_t pln;
        std::array<uint8_t, 2> op{};
        std::vector<uint8_t> sha{};
        std::vector<uint8_t> spa{};
        std::vector<uint8_t> tha{};
        std::vector<uint8_t> tpa{};
    };
    struct TranslationTableRow {
        std::array<uint8_t, 2> protocol_type;
        std::vector<uint8_t> sender_protocol_address;
        std::vector<uint8_t> sender_hardware_address;
    };
    [[nodiscard]] static auto parse(const std::vector<uint8_t>& packet) -> Header;
    [[nodiscard]] auto handle(std::string_view if_name, Header&& header) -> std::vector<uint8_t>;
private:
    std::vector<TranslationTableRow> m_translation_table{};
};

#endif
