#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <unistd.h>

// From https://www.kernel.org/doc/Documentation/networking/tuntap.txt, slightly modified
int tun_alloc(std::string dev) {
  struct ifreq ifr;
  int fd, err;

  if ((fd = open("/dev/net/tun1", O_RDWR)) < 0) {
    std::cerr << "Could not create TAP/TUN device\n";
    exit(1);
  }

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
   *        IFF_TAP   - TAP device
   *
   *        IFF_NO_PI - Do not provide packet information
   */
  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
  if (!dev.empty())
    (ifr.ifr_name, dev.c_str(), IFNAMSIZ);

  if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
    close(fd);
    return err;
  }
  dev = ifr.ifr_name;
  return fd;
}

int main() {
  return 0;
}