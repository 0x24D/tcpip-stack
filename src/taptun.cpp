#include "taptun.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <utility>
#include "linux/if_tun.h"
#include "net/if.h"

taptun::taptun(std::string &name, const interface_type &type, std::string addr, std::string route)
    : m_addr(std::move(addr)), m_route(std::move(route)) {
    struct ifreq ifr {};

    const auto dev = (type == interface_type::TAP ? "/dev/net/tap" : "/dev/net/tun");
    auto fd = open(dev, O_RDWR);
    if (fd < 0) {
        std::cerr << "Cannot open " << dev << ": " << errno << '\n';
        exit(1);
    }

    if (!name.empty()) {
        strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ);
    }

    ifr.ifr_flags = (type == interface_type::TAP ? IFF_TAP : IFF_TUN) | IFF_NO_PI;

    auto err = ioctl(fd, TUNSETIFF, &ifr);

    if (err < 0) {
        std::cerr << "Could not ioctl " << dev << ": " << errno << '\n';
        close(fd);
        exit(err);
    }

    name = ifr.ifr_name;
    m_fd = fd;
    m_name = name;

    // Bring interface up
    auto ret = std::system((std::string{"ip link set dev "} + m_name + std::string{" up"}).c_str());
    if (ret != 0) {
        std::cerr << "Could not bring up " << m_name << ": " << ret << '\n';
    }
    // Set route to interface
    ret = std::system(
        (std::string{"ip route add dev "} + m_name + std::string{' '} + m_route).c_str());
    if (ret != 0) {
        std::cerr << "Could not set route to " << m_name << ": " << ret << '\n';
    }
    // Set interface address
    ret = std::system(
        (std::string{"ip address add dev "} + m_name + std::string{" local "} + m_addr).c_str());
    if (ret != 0) {
        std::cerr << "Could not set address of " << m_name << ": " << ret << '\n';
    }
}

taptun::~taptun() {
    close(m_fd);
}
