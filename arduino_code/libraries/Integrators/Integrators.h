#include <functional>

class RungeKutta4{
    RungeKutta4(double, std::function<double(double, double)>);

    public:
    //methods
    double step(double, double, double);
    //members
    double dt;
    std::function<double(double, double)> f();
};