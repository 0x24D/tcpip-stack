#ifndef TAP_TUN_H
#define TAP_TUN_H

#include <string>
#include <vector>

class TapTun {
public:
    enum struct InterfaceType : bool { TAP = true, TUN = false };
    TapTun(std::string &name, const InterfaceType &, std::string addr, std::string route);
    ~TapTun();
    void listen() const;
    [[nodiscard]] auto read() const;
private:
    int m_fd;
    std::string m_name;
    std::string m_addr;
    std::string m_route;
};

#endif
