#include "calculator.h"
#include <stack>
#include <Windows.h>
#include <stdexcept>
#include <sstream>

double Calculator::solve(const std::string &task) {
    std::vector<std::string> splitTask = splitExpression(task);
    std::queue<std::string> infixNotation = toInfixNotation(splitTask);
    return calculateInfixNotation(infixNotation);
}

std::vector<std::string> Calculator::splitExpression (const std::string& expression) {
    std::vector<std::string> expressionElements;
    std::string charBuffer;

    for (auto expressionChar : expression) {
        if (isdigit(expressionChar) || expressionChar == '.') {
            if(!charBuffer.empty() && !isdigit(charBuffer.front())) {
                expressionElements.push_back(charBuffer);
                charBuffer.clear();
            }
            charBuffer.push_back(expressionChar);
        } else if (standardOperators.contains(expressionChar) ||
                   expressionChar == '(' || expressionChar == ')') {
            if(!charBuffer.empty()) {
                expressionElements.push_back(charBuffer);
                charBuffer.clear();
            }
            expressionElements.push_back({expressionChar});
        } else if (!isspace(expressionChar)) {
            if (!charBuffer.empty() && isdigit(charBuffer.front())) {
                expressionElements.push_back(charBuffer);
                charBuffer.clear();
            }
            charBuffer.push_back(expressionChar);
        }
    }

    if(!charBuffer.empty()) expressionElements.push_back(charBuffer);

    return expressionElements;
}



std::queue<std::string> Calculator::toInfixNotation(const std::vector<std::string>& splitExpression) {
    std::stack<std::string> operatorsStack;
    std::queue<std::string> outputQueue;
    for (auto const& expressionElement : splitExpression) {
        if (isdigit(expressionElement.front())) outputQueue.push(expressionElement);
        else if (expressionElement.front() == '(') operatorsStack.push(expressionElement);
        else if (expressionElement.front() == ')') {
            while (operatorsStack.top().front() != '(') {
                outputQueue.push(operatorsStack.top());
                operatorsStack.pop();
                if (operatorsStack.empty()){
                    //exp
                }
            }
            operatorsStack.pop();
        } else {
            while (!operatorsStack.empty() &&
                   (standardOperators.contains(operatorsStack.top().front()) &&
                    standardOperators.at(operatorsStack.top().front())->getPriority() >=
                    standardOperators.at(expressionElement.front())->getPriority())) {
                outputQueue.push(operatorsStack.top());
                operatorsStack.pop();
            }
            operatorsStack.push(expressionElement);
        }
    }
    while (!operatorsStack.empty()) {
        if (operatorsStack.top().front() == '(') {
            //exp
        } else
            outputQueue.push(operatorsStack.top());
        operatorsStack.pop();
    }

    return outputQueue;
}

double Calculator::calculateInfixNotation(std::queue<std::string>& expression) {
    std::stack<double> numsStack;
    while (!expression.empty()) {
        if (isdigit(expression.front().front())) {
            numsStack.push(stod(expression.front()));
        } else if (standardOperators.contains(expression.front().front())) {
            double right = numsStack.top();
            numsStack.pop();
            double left = numsStack.top();
            numsStack.pop();
            numsStack.push(standardOperators.at(expression.front().front())->calculate({left, right}));
        } else if (additionalOperators.contains(expression.front())) {
            if (additionalOperators.at(expression.front())->getArgsNum() == 1) {
                double arg = numsStack.top();
                numsStack.pop();
                numsStack.push(additionalOperators.at(expression.front())->calculate({arg}));
            } else if (additionalOperators.at(expression.front())->getArgsNum() == 2) {
                double right = numsStack.top();
                numsStack.pop();
                double left = numsStack.top();
                numsStack.pop();
                numsStack.push(additionalOperators.at(expression.front())->calculate({left, right}));
            }
        } else {
            //ex
        }
        expression.pop();
    }
    return numsStack.top();
}

void Calculator::loadAdditionalOperators(const std::string &path) {
    WIN32_FIND_DATAA wfd;
    HANDLE dllFile = FindFirstFileA((path + std::string("*.dll")).c_str(), &wfd);
    if (dllFile != INVALID_HANDLE_VALUE) {
        do {
            std::string dllName(wfd.cFileName);
            HMODULE libraryFile = LoadLibraryA((path + dllName).c_str());

            std::string definition((char *) (GetProcAddress(libraryFile, "funcDefinition")));
            std::string function((char *) (GetProcAddress(libraryFile, "funcName")));
            int* argsNum = (int*) (GetProcAddress(libraryFile, "arity"));

            if(*argsNum == 1)
            {
                std::function<double(double)> importFunc =
                        (double(*)(double))(GetProcAddress(libraryFile, function.c_str()));
                if (importFunc == nullptr) {
                    std::ostringstream error_message;
                    error_message << "No function " << function.c_str() << "in " << libraryFile;
                    throw std::runtime_error(error_message.str());
                }
                additionalOperators.insert({definition, (Operator *) new UnaryOperator (
                                                          importFunc,Operator::Priority::HIGH)});
            } else if(*argsNum == 2) {
                std::function<double(double, double)> importFunc =
                        (double(*)(double, double))(GetProcAddress(libraryFile, function.c_str()));
                if (importFunc == nullptr) {
                    std::ostringstream error_message;
                    error_message << "No function " << function.c_str() << "in " << libraryFile;
                    throw std::runtime_error(error_message.str());
                }
                additionalOperators.insert({definition, (Operator *) new BinaryOperator (
                        importFunc,Operator::Priority::HIGH)});
            }
        } while (NULL != FindNextFileA(dllFile, &wfd));
        FindClose(dllFile);
    }
    else {
        throw std::runtime_error("Files not found");
    }
}
