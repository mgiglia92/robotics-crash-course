from matplotlib.pyplot import spring
import numpy as np
from pygame import Vector2
from math_utils import EulerIntegrator, Derivative

class System:
    def __init__(self):
        raise NotImplementedError
    @staticmethod
    def xdot():
        raise NotImplementedError
    def get_pos_vec2(self):
        raise NotImplementedError

class System2D(System):
    def __init__(self, state, dt):
        self.state = state # The current state of the system
        self.state_d1 = [0,0,0,0] # The state one time step back
        self.dt = dt # The delta t between time steps
        self.integrator = EulerIntegrator(0)


    def step(self, u):
        self.state = self.state + System2D.xdot(self.state, u)*self.dt

    @staticmethod
    def xdot(x: np.array, u: np.array):
        return np.array([x[2], x[3], u[0], u[1]])

    def get_pos_vec2(self):
        return Vector2(self.state[0], self.state[1])

class SpringSystem(System):
    def __init__(self, state, dt, spring_constant = 1):
        self.state = state # The current state of the system
        self.state_d1 = [0,0] # The state one time step back
        self.dt = dt # The delta t between time steps
        self.integrator = EulerIntegrator(0)
        self.spring_constant = spring_constant


    def step(self):
        # Get the spring force on the system
        spring_force = -1 * self.state[0]*self.spring_constant
        self.state = self.state + SpringSystem.xdot(self.state, spring_force)*self.dt

    @staticmethod
    def xdot(x: np.array, u: np.array):
        return np.array([x[1], u])

    def get_pos_vec2(self):
        return Vector2(self.state[0], 0)