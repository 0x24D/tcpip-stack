#include <iostream>
#include "taptun.h"

int main() {
    auto if_name = std::string{"tap0"};
    auto tap_if = taptun(if_name, taptun::interface_type::TAP, "10.0.0.5", "10.0.0.0/24");
    std::cout << "Configured tap interface: " << if_name << '\n';
    return 0;
}
