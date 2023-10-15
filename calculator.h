#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <queue>
#include "Operator.h"


class Calculator {
public:
    Calculator(Calculator const&) = delete;
    Calculator& operator = (Calculator const&) = delete;
    Calculator (Calculator&&) = delete;
    Calculator& operator = (Calculator &&) = delete;
    ~Calculator () = default;

    static Calculator& getInstance() {
        static Calculator calculator;
        return calculator;
    }

    double solve (const std::string& task);
private:
    std::unordered_map<char, Operator *> standardOperators;

    Calculator () {
        standardOperators = {
            std::pair<char, Operator *>
                    {'+', (Operator *) new BinaryOperator (
                            [](double left, double right) {return left + right;},
                            Operator::Priority::LOW)},
            std::pair<char, Operator *>
                    {'-', (Operator *) new BinaryOperator (
                            [](double left, double right) {return left - right;},
                            Operator::Priority::LOW)},
            std::pair<char, Operator *>
                    {'/', (Operator *) new BinaryOperator (
                            [](double left, double right) {return left / right;},
                            Operator::Priority::MIDDLE)},
            std::pair<char, Operator *>
                    {'*', (Operator *) new BinaryOperator (
                            [](double left, double right) {return left * right;},
                            Operator::Priority::MIDDLE)},
        };
    }

    std::vector<std::string> splitExpression (const std::string& expression);
    std::queue<std::string> toInfixNotation (const std::vector<std::string>& splitExpression);
    double calculateInfixNotation (std::queue<std::string>& expression);
};
#endif //CALCULATOR_CALCULATOR_H
