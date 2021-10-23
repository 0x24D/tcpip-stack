#include <algorithm>
// clang-format assumes this is the related header for this file, so moves it to the top.
// clang-format off
#include "../../src/arp.h"
// clang-format on
#include "../utils/testhelper.h"

void test_no_hln() {
    const std::array<uint8_t, 2> hrd = {0x00, 0x01};
    const std::array<uint8_t, 2> pro = {0x02, 0x03};
    const uint8_t hln = 0x00;
    const uint8_t pln = 0x01;
    const std::array<uint8_t, 2> op = {0x04, 0x05};
    const std::vector<uint8_t> sha = {};
    const std::vector<uint8_t> spa = {0x06};
    const std::vector<uint8_t> tha = {};
    const std::vector<uint8_t> tpa = {0x07};

    std::vector<uint8_t> packet{};
    packet.reserve((hrd.size() + pro.size() + 1 + 1 + op.size() + sha.size() + spa.size() +
                    tha.size() + tpa.size()));
    std::ranges::copy(hrd, std::back_inserter(packet));
    std::ranges::copy(pro, std::back_inserter(packet));
    packet.push_back(hln);
    packet.push_back(pln);
    std::ranges::copy(op, std::back_inserter(packet));
    std::ranges::copy(sha, std::back_inserter(packet));
    std::ranges::copy(spa, std::back_inserter(packet));
    std::ranges::copy(tha, std::back_inserter(packet));
    std::ranges::copy(tpa, std::back_inserter(packet));

    const ARP arp{packet};

    TestHelper::equals("Hardware Type", arp.get_hrd(), hrd);
    TestHelper::equals("Protocol Type", arp.get_pro(), pro);
    TestHelper::equals("Hardware Address Length", arp.get_hln(), hln);
    TestHelper::equals("Protocol Address Length", arp.get_pln(), pln);
    TestHelper::equals("OpCode", arp.get_op(), op);
    TestHelper::equals("Sender Hardware Address", arp.get_sha(), sha);
    TestHelper::equals("Sender Protocol Address", arp.get_spa(), spa);
    TestHelper::equals("Target Hardware Address", arp.get_tha(), tha);
    TestHelper::equals("Target Protocol Address", arp.get_tpa(), tpa);
}

void test_no_pln() {
    const std::array<uint8_t, 2> hrd = {0x00, 0x01};
    const std::array<uint8_t, 2> pro = {0x02, 0x03};
    const uint8_t hln = 0x01;
    const uint8_t pln = 0x00;
    const std::array<uint8_t, 2> op = {0x04, 0x05};
    const std::vector<uint8_t> sha = {0x06};
    const std::vector<uint8_t> spa = {};
    const std::vector<uint8_t> tha = {0x07};
    const std::vector<uint8_t> tpa = {};

    std::vector<uint8_t> packet{};
    packet.reserve((hrd.size() + pro.size() + 1 + 1 + op.size() + sha.size() + spa.size() +
                    tha.size() + tpa.size()));
    std::ranges::copy(hrd, std::back_inserter(packet));
    std::ranges::copy(pro, std::back_inserter(packet));
    packet.push_back(hln);
    packet.push_back(pln);
    std::ranges::copy(op, std::back_inserter(packet));
    std::ranges::copy(sha, std::back_inserter(packet));
    std::ranges::copy(spa, std::back_inserter(packet));
    std::ranges::copy(tha, std::back_inserter(packet));
    std::ranges::copy(tpa, std::back_inserter(packet));

    const ARP arp{packet};

    TestHelper::equals("Hardware Type", arp.get_hrd(), hrd);
    TestHelper::equals("Protocol Type", arp.get_pro(), pro);
    TestHelper::equals("Hardware Address Length", arp.get_hln(), hln);
    TestHelper::equals("Protocol Address Length", arp.get_pln(), pln);
    TestHelper::equals("OpCode", arp.get_op(), op);
    TestHelper::equals("Sender Hardware Address", arp.get_sha(), sha);
    TestHelper::equals("Sender Protocol Address", arp.get_spa(), spa);
    TestHelper::equals("Target Hardware Address", arp.get_tha(), tha);
    TestHelper::equals("Target Protocol Address", arp.get_tpa(), tpa);
}

void test_hln_and_pln() {
    const std::array<uint8_t, 2> hrd = {0x00, 0x01};
    const std::array<uint8_t, 2> pro = {0x02, 0x03};
    const uint8_t hln = 0x01;
    const uint8_t pln = 0x02;
    const std::array<uint8_t, 2> op = {0x04, 0x05};
    const std::vector<uint8_t> sha = {0x06};
    const std::vector<uint8_t> spa = {0x07, 0x08};
    const std::vector<uint8_t> tha = {0x09};
    const std::vector<uint8_t> tpa = {0x0A, 0x0B};

    std::vector<uint8_t> packet{};
    packet.reserve((hrd.size() + pro.size() + 1 + 1 + op.size() + sha.size() + spa.size() +
                    tha.size() + tpa.size()));
    std::ranges::copy(hrd, std::back_inserter(packet));
    std::ranges::copy(pro, std::back_inserter(packet));
    packet.push_back(hln);
    packet.push_back(pln);
    std::ranges::copy(op, std::back_inserter(packet));
    std::ranges::copy(sha, std::back_inserter(packet));
    std::ranges::copy(spa, std::back_inserter(packet));
    std::ranges::copy(tha, std::back_inserter(packet));
    std::ranges::copy(tpa, std::back_inserter(packet));

    const ARP arp{packet};

    TestHelper::equals("Hardware Type", arp.get_hrd(), hrd);
    TestHelper::equals("Protocol Type", arp.get_pro(), pro);
    TestHelper::equals("Hardware Address Length", arp.get_hln(), hln);
    TestHelper::equals("Protocol Address Length", arp.get_pln(), pln);
    TestHelper::equals("OpCode", arp.get_op(), op);
    TestHelper::equals("Sender Hardware Address", arp.get_sha(), sha);
    TestHelper::equals("Sender Protocol Address", arp.get_spa(), spa);
    TestHelper::equals("Target Hardware Address", arp.get_tha(), tha);
    TestHelper::equals("Target Protocol Address", arp.get_tpa(), tpa);
}

int main() {
    test_no_hln();
    test_no_pln();
    test_hln_and_pln();

    return TestHelper::result("unit/arp");
}
