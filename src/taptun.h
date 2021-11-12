#ifndef TAP_TUN_H
#define TAP_TUN_H

#include <string>
#include <vector>
#include "ethernet.h"

class TapTun {
public:
    enum struct InterfaceType : bool { TAP = true, TUN = false };
    TapTun(std::string& name, const InterfaceType&, std::string addr, std::string route);
    ~TapTun();
    void listen();
    [[nodiscard]] auto read() const;
    void write(const std::vector<std::uint8_t>& frame) const;
private:
    int m_fd;
    std::string m_name;
    std::string m_addr;
    std::string m_route;
    Ethernet m_ethernet{};
};

#endif
