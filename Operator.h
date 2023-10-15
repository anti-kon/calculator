#ifndef CALCULATOR_OPERATOR_H
#define CALCULATOR_OPERATOR_H

#include <functional>

class Operator {
public:
    enum Priority {
        LOW,
        MIDDLE,
        HIGH
    };
private:
    int argsNum;
    Priority priority;
public:
    explicit Operator (int argsNum, Priority priority) : argsNum(argsNum), priority(priority) {};
    virtual ~Operator() = default;
    virtual double calculate (std::vector<double> args) = 0;
    Priority getPriority () { return priority; }
    int getArgsNum () const { return argsNum; }
};

class UnaryOperator : public Operator {
    std::function<double(double)> func;
public:
    explicit UnaryOperator (std::function<double(double)> operatorFunc, Priority priority) :
            Operator (1, priority), func(std::move(operatorFunc)) {}
    double calculate (std::vector<double> args) override { return func(args[0]); }
};

class BinaryOperator : public Operator {
    std::function<double(double, double)> func;
public:
    explicit BinaryOperator (std::function<double(double, double)> operatorFunc, Priority priority) :
            Operator (2, priority), func(std::move(operatorFunc)) {}
    double calculate (std::vector<double> args) override { return func(args[0], args[1]); }
};

#endif
