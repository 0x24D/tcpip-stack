#ifndef ARP_H
#define ARP_H

#include <array>
#include <cstdint>
#include <vector>

class ARP {
public:
    explicit ARP(const std::vector<uint8_t>& packet);
    [[nodiscard]] auto get_hrd() const -> std::array<uint8_t, 2>;
    [[nodiscard]] auto get_pro() const -> std::array<uint8_t, 2>;
    [[nodiscard]] auto get_hln() const -> uint8_t;
    [[nodiscard]] auto get_pln() const -> uint8_t;
    [[nodiscard]] auto get_op() const -> std::array<uint8_t, 2>;
    [[nodiscard]] auto get_sha() const -> std::vector<uint8_t>;
    [[nodiscard]] auto get_spa() const -> std::vector<uint8_t>;
    [[nodiscard]] auto get_tha() const -> std::vector<uint8_t>;
    [[nodiscard]] auto get_tpa() const -> std::vector<uint8_t>;
    void handle() const;
private:
    std::array<uint8_t, 2> m_hrd{};
    std::array<uint8_t, 2> m_pro{};
    uint8_t m_hln{};
    uint8_t m_pln{};
    std::array<uint8_t, 2> m_op{};
    std::vector<uint8_t> m_sha{};
    std::vector<uint8_t> m_spa{};
    std::vector<uint8_t> m_tha{};
    std::vector<uint8_t> m_tpa{};
};

#endif
