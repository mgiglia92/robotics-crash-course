from pygame import Vector2
from system_utils import System
import numpy as np

class SystemExample(System):
    #This is similar to the "Constructor" in c++
    def __init__(self):
        self.state = [0,0] # Initialize pos:0, vel:0
        self.state_d1 = [0,0] # This is the state one time step prior!
        self.dt = 0.01 # This is the delta time between calculations
        self.des_x = 100 # This is the desired position we want the system to go to
    
    # This function will "step" the system one time step into the
    # future. All the important math will happen here.
    # This example has a 1D P-controller
    def step(self):
        # Get error
        error = self.des_x - self.state[0]

        # Calculate controller output
        kp = 1 # The proportional gain
        output = error*kp # This will be an acceleration!

        # Before updating the state, push the values of the current state to the state_d1 variable
        # To store it for later calculations. Remember to do this only AFTER you've used the state_d1 value
        # (In this example we do NOT use the state_d1 variable)
        self.state_d1 = self.state

        # Do euler first order integration, to update the system state 
        self.state = self.state + self.xdot(self.state, output)*self.dt

    
    # This function is the derivative of the system
    # The naming convention comes from the mathematical notation
    # It takese in two arguments:
    # x: the current state of the system [x,y,theta,vx,vy,omega]
    # u: The "input" to the system. This is system dependent
    # For example, for a 1D spring system 'u' will be an acceleration
    # that comes from the spring force (Fspring = -k*delta_x)
    def xdot(self, x, u):
        return np.array([x[1],u])

    # These three are convenience, it returns the values we need for position/velocity/theta
    # in the format pygame requires, the 'Vector2' class
    def get_pos_vec2(self):
        return Vector2(self.state[0], 0)
    
    def get_vel_vec2(self):
        return Vector2(self.state[1], 0)
    
    def get_theta(self):
        return 0