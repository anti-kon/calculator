#include "calculator.h"
#include <windows.h>
#include <iostream>

int main() {
    auto a = Calculator::getInstance().solve("2 + 3 * (1 - (3 * (4 + 2))) - 4 / 5");
    std::cout << a;
    return 0;
}

