#include "calculator.h"
#include <windows.h>
#include <iostream>

int main() {
    auto a = Calculator::getInstance().solve("cos(3.14)");
    std::cout << a;
    return 0;
}

