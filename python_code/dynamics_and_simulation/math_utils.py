from argparse import ArgumentError
import numpy as np


class EulerIntegrator:
    def __init__(self, initial_value):
        self.integral = 0.0
        self.type = 'trap'
        self.previous_value = initial_value
    
    def integrate(self, x: float, dt: float, type='trap'):
        # Integrate using the trapezoidal rule
        if type == 'trap':
            out = dt * (0.5) * (x + self.previous_value)
            self.integral += out # Add onto integral value so this can be called in loop
            self.previous_value = x # Update previous value for the function
            return out

        # Integrate using the right hand riemann sum
        elif type == 'right':
            out = dt * x
            self.integral += out    # Add onto integral value so this can be called in loop
            self.previous_value = x # Update previous value for the function
            return out

        else:
            print(f"Incorrect argument for type:\n Allowed: right, trap\n Provided: {type}")
            raise ArgumentError


class Derivative:
    def __init__(self, initial_value):
        self.previous_value = initial_value
    
    def derivative(self, x: float, dt: float, type='normal'):
        if type == 'normal':
            out = (x - self.previous_value)/dt
            return out
        if type == 'dirty':
            return self.derivative(x, dt, 'normal')
        else:
            print(f"Incorrect argument for type:\n Allowed: normal, dirty\n Provided: {type}")
            raise ArgumentError
