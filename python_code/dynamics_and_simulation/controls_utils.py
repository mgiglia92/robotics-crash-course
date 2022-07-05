import numpy as np
from math_utils import EulerIntegrator, Derivative

class Controller:
    def __init__(self, dt=0.01, llim=-100, ulim=100):
        self.dt = dt
        self.llim = llim # The lower limit of the controller output
        self.ulim = ulim # The upper limit of the controller output
        
        raise NotImplementedError

    # Steps the controller one time step, returns control input
    def step(self):
        raise NotImplementedError
    
    # Saturate the controller output
    def saturate(self, u):
        return np.clip(u, self.llim, self.ulim)
    
    def to_dict(self):
        raise NotImplementedError

    # Reset the integrator and derivative parameters for controller reset
    def reset(self):
        raise NotImplementedError

class PControl(Controller):
    def __init__(self, dt=0.01, llim=-100, ulim=100, kp=0.0, ki=0.0, kd=0.0, antiwindup=True):
        self.dt = dt
        self.llim=llim
        self.ulim=ulim
        self.kp=kp
        self.ki=ki
        self.kd=kd
        self.antiwindup=antiwindup
        self.integrator = EulerIntegrator(initial_value = 0)
        self.derivative = Derivative(initial_value = 0.0)
    
    def step(self, desired, actual):
        err = desired-actual

        # Do Euler integration for this timestep
        integral_sum = self.integrator.integrate(err, self.dt)

        # Deal with integrator windup
        if self.antiwindup:
            if self.ki != 0.0:
                # I = clip(I*ki)/ki
                self.integrator.integral = self.saturate(self.integrator.integral*self.ki) / self.ki

        # Get derivative
        err_dot = self.derivative.derivative(err, self.dt, fcn_type='normal')

        # Get controller output
        output = self.kp*err + self.ki*integral_sum + self.kd*err_dot

        # print(f'p: {err*self.kp}, i: {integral_sum*self.ki}, d: {err_dot*self.kd}')

        # Saturate output
        return self.saturate(output)
    
    
    def to_dict(self):
        return dict({"kp":self.kp, "ki":self.ki, "kd":self.kd, "llim":self.llim, "ulim":self.llim, "dt":self.dt, "antiwindup":self.antiwindup})
    
    def reset(self):
        self.integrator.integral=0
        self.integrator.previous_value = 0
        self.derivative.previous_value = 0



    @staticmethod
    def from_dict(d:dict):
        dt = d['dt']
        kp = d['kp']
        ki = d['ki']
        kd = d['kd']
        llim = d['llim']
        ulim = d['ulim']
        antiwindup = d['antiwindup']
        c = PControl(dt, llim, ulim, kp, ki, kd, antiwindup)
        c.reset()
        return c