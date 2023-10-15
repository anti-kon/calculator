#include <cmath>
extern "C" __declspec(dllexport)
double external_func(double a) {
    return cos(a);
}