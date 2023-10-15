#include "calculator.h"
#include <windows.h>
#include <iostream>

int main() {
    std::string input;

    while (true) {
        std::getline(std::cin, input);
        std::cout << Calculator::getInstance().solve(input) << std::endl;
        input.clear();
    }

}

