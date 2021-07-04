#include <iostream>
#include "taptun.h"

int main() {
    auto if_name = std::string{"tap0"};
    auto tap_if = TapTun(if_name, TapTun::InterfaceType::TAP, "10.0.0.5", "10.0.0.0/24");
    std::cout << "Configured tap interface: " << if_name << '\n';
    tap_if.listen();
    return 0;
}
