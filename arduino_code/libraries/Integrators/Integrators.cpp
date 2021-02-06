#include <Integrators.h>

RungeKutta4::RungeKutta4(double deltaT, std::function<double(double, double)> &func){
    f = func;
    dt = deltaT;
}

double RungeKutta4::step(double t, double x, double u){
        k1 = f(t, x, u);
        k2 = f(t+0.5*self.dt, x+0.5*self.dt*k1, u);
        k3 = f(t+0.5*self.dt, x+0.5*self.dt*k2, u);
        k4 = f(t+    self.dt, x+    self.dt*k3, u);
        return x + dt * (k1 + 2*k2 + 2*k3 + k4) / 6;
}