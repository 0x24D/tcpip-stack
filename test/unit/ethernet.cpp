#include <algorithm>
#include <numeric>
// clang-format assumes this is the related header for this file, so moves it to the top.
// clang-format off
#include "../../src/ethernet.h"
// clang-format on
#include "../utils/testhelper.h"

void test_min_payload() {
    constexpr std::array<uint8_t, 6> dest{0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    constexpr std::array<uint8_t, 6> src{0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
    constexpr std::array<uint8_t, 2> ethertype{0x0C, 0x0D};

    constexpr auto payload_size = 46;
    std::vector<uint8_t> payload(payload_size);
    std::iota(payload.begin(), payload.end(), 0x0E);

    constexpr std::array<uint8_t, 4> crc{0xFC, 0xFD, 0xFE, 0xFF};

    std::vector<uint8_t> frame{};
    frame.reserve((dest.size() + src.size() + ethertype.size() + payload.size() + crc.size()));
    std::ranges::copy(dest, std::back_inserter(frame));
    std::ranges::copy(src, std::back_inserter(frame));
    std::ranges::copy(ethertype, std::back_inserter(frame));
    std::ranges::copy(payload, std::back_inserter(frame));
    std::ranges::copy(crc, std::back_inserter(frame));

    const auto eth = Ethernet::parse(frame);

    TestHelper::equals("Destination MAC", eth.dest, dest);
    TestHelper::equals("Source MAC", eth.src, src);
    TestHelper::equals("Ethertype", eth.ethertype, ethertype);
    TestHelper::equals("Payload", eth.payload, payload);
    TestHelper::equals("CRC", eth.crc, crc);
}

void test_max_payload() {
    constexpr std::array<uint8_t, 6> dest{0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    constexpr std::array<uint8_t, 6> src{0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
    constexpr std::array<uint8_t, 2> ethertype{0x0C, 0x0D};

    constexpr auto payload_size = 1500;
    constexpr auto overflow_value = std::numeric_limits<uint8_t>::max() + 1;
    constexpr auto count = static_cast<int>(payload_size / overflow_value);
    std::vector<uint8_t> payload(payload_size);
    for (auto i = 0; i < count; ++i) {
        std::iota(
            payload.begin() + i * overflow_value, payload.begin() + (i + 1) * overflow_value, 0x00);
    }
    std::iota(payload.begin() + count * overflow_value, payload.end(), 0x00);

    constexpr std::array<uint8_t, 4> crc{0xFC, 0xFD, 0xFE, 0xFF};

    std::vector<uint8_t> frame{};
    frame.reserve((dest.size() + src.size() + ethertype.size() + payload.size() + crc.size()));
    std::ranges::copy(dest, std::back_inserter(frame));
    std::ranges::copy(src, std::back_inserter(frame));
    std::ranges::copy(ethertype, std::back_inserter(frame));
    std::ranges::copy(payload, std::back_inserter(frame));
    std::ranges::copy(crc, std::back_inserter(frame));

    const auto eth = Ethernet::parse(frame);

    TestHelper::equals("Destination MAC", eth.dest, dest);
    TestHelper::equals("Source MAC", eth.src, src);
    TestHelper::equals("Ethertype", eth.ethertype, ethertype);
    TestHelper::equals("Payload", eth.payload, payload);
    TestHelper::equals("CRC", eth.crc, crc);
}

void test_is_valid() {
    constexpr std::array<uint8_t, 6> dest{0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    constexpr std::array<uint8_t, 6> src{0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
    constexpr std::array<uint8_t, 2> ethertype{0x0C, 0x0D};

    constexpr auto payload_size = 46;
    std::vector<uint8_t> payload(payload_size);
    std::iota(payload.begin(), payload.end(), 0x0E);

    {
        constexpr std::array<uint8_t, 4> crc{0xFC, 0xFD, 0xFE, 0xFF};

        std::vector<uint8_t> frame{};
        frame.reserve((dest.size() + src.size() + ethertype.size() + payload.size() + crc.size()));
        std::ranges::copy(dest, std::back_inserter(frame));
        std::ranges::copy(src, std::back_inserter(frame));
        std::ranges::copy(ethertype, std::back_inserter(frame));
        std::ranges::copy(payload, std::back_inserter(frame));
        std::ranges::copy(crc, std::back_inserter(frame));

        const auto hdr = Ethernet::parse(frame);

        TestHelper::equals("Checksum is invalid", Ethernet::is_valid(hdr), false);
    }

    {
        constexpr std::array<uint8_t, 4> crc{0x53, 0x94, 0x19, 0x27};

        std::vector<uint8_t> frame{};
        frame.reserve((dest.size() + src.size() + ethertype.size() + payload.size() + crc.size()));
        std::ranges::copy(dest, std::back_inserter(frame));
        std::ranges::copy(src, std::back_inserter(frame));
        std::ranges::copy(ethertype, std::back_inserter(frame));
        std::ranges::copy(payload, std::back_inserter(frame));
        std::ranges::copy(crc, std::back_inserter(frame));

        const auto hdr = Ethernet::parse(frame);

        TestHelper::equals("Checksum is valid", Ethernet::is_valid(hdr), true);
    }
}

int main() {
    test_min_payload();
    test_max_payload();
    test_is_valid();

    return TestHelper::result("unit/ethernet");
}
