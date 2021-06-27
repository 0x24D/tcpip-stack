#ifndef TAP_TUN_H
#define TAP_TUN_H

#include <string>
#include <vector>

class taptun {
public:
    enum struct interface_type : bool { TAP = true, TUN = false };
    taptun(std::string &name, const interface_type &, std::string addr, std::string route);
    ~taptun();
    void listen() const;
    [[nodiscard]] std::vector<uint8_t> read() const;
private:
    int m_fd;
    std::string m_name;
    std::string m_addr;
    std::string m_route;
};

#endif
