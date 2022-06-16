from matplotlib.pyplot import spring
import numpy as np
from pygame import Vector2
from math_utils import EulerIntegrator, Derivative
from controls_utils import PControl
import yaml
import os

class System:
    def __init__(self):
        raise NotImplementedError
    @staticmethod
    def xdot():
        raise NotImplementedError
    def get_pos_vec2(self):
        raise NotImplementedError
    def get_vel_vec2(self):
        raise NotImplementedError
    def step(self):
        raise NotImplementedError
    def to_yaml(self):
        raise NotImplementedError
    def from_yaml(self):
        raise NotImplementedError
    def to_dict(self):
        raise NotImplementedError

class SystemRobot(System):
    def __init__(self, state = [0,0,0,0,0,0], dt = 0.01, controllerv = PControl(), controllerw = PControl()):
        self.state = state # The current state of the system
        self.state_d1 = [0,0,0,0,0,0] # The state one time step back
        self.dt = controllerv.dt # The delta t between time steps
        self.controllerv = controllerv
        self.controllerw = controllerw
        self.controllerw.dt = self.controllerv.dt
        self.R = 10 # Length from center of bot to wheel
    
    def step(self, desired = [0,0,0,20,0,90]):
        u = [0,0,0]

        # Velocity controller
        u[0] = self.controllerv.step(desired[3], self.state[3])

        # Omega controller
        u[1] = self.controllerw.step(desired[5], self.state[5])

        self.state = self.state + self.xdot(self.state, u)*self.dt
        # print(f"u:{u}  | state: {self.state}")
    
    # TODO: Maybe this shouldn't be a static method
    def xdot(self, x: np.array, u: np.array):
        l_power = u[0] + u[1]
        r_power = u[0] - u[1]
        accel = l_power + r_power 
        alpha = (l_power - r_power) * self.R
        v_mag = np.sqrt(x[3]**2 + x[4]**2)
        vx = x[3]*np.cos(x[2])
        vy = x[3]*np.sin(x[2])
        # print(f"state:{self.state}")
        # print(f"vx: {vx}, vy: {vy}, total:{np.sqrt(vx**2 + vy**2)}")
        return np.array([x[3]*np.cos(x[2]*np.pi/180), x[3]*np.sin(x[2]*np.pi/180), x[5], accel, 0, alpha])
        # return np.array([0,0,x[5],0,0,10])

    def get_pos_vec2(self):
        return Vector2(self.state[0], self.state[1])
    
    def get_vel_vec2(self):
        return Vector2(self.state[3]*np.cos(self.state[2]*np.pi/180), self.state[3]*np.sin(self.state[2]*np.pi/180))
    
    def get_theta(self):
        return self.state[2]
    
    def to_dict(self):
        return dict({"controllerv":self.controllerv.to_dict(),\
                    "controllerw":self.controllerw.to_dict(),\
                    "state":self.state})
    def get_state_dict(self):
        return dict({"x":self.state[0],\
                    "y":self.state[1],\
                    "theta":self.state[2],\
                    "vx":self.state[3],\
                    "vy":self.state[4],\
                    "omega":self.state[5]})

    def to_yaml(self):
        print("Converting to yaml")
        local = os.path.dirname(__file__)
        file = open(local + '/system_robot_params.yaml', 'w+')
        yaml.dump(self.to_dict(),  file)

    def from_yaml(self):
        print("Updating from yaml")
        local = os.path.dirname(__file__)
        file = open(local + '/system_robot_params.yaml', 'r')
        params = yaml.load(stream=file, Loader=yaml.CLoader)
        self.controllerv = PControl.from_dict(params['controllerv'])
        self.controllerw = PControl.from_dict(params['controllerw'])
        self.state = params['state']
        self.controllerv.reset()
        self.controllerw.reset()
        return params

    def get_parameter_dictionary(self):
        pass

class System1DControl(System):
    def __init__(self, state = [0,0,0,0,0,0], dt = 0.01, controller = PControl()):
        self.state = state # The current state of the system
        self.state_d1 = [0,0,0,0,0,0] # The state one time step back
        self.dt = controller.dt # The delta t between time steps
        self.controller = controller
    
    def step(self, desired = [0,0,0,0,0,0]):
        u = [0,0,0]
        u[0] = self.controller.step(desired[0], self.state[0])
        self.state = self.state + System1DControl.xdot(self.state, u)*self.dt
        # print(f"u:{u}  | state: {self.state}")
    
    @staticmethod
    def xdot(x: np.array, u: np.array):
        return np.array([x[3], x[4], x[5], u[0],u[1],u[2]])

    def get_pos_vec2(self):
        return Vector2(self.state[0], self.state[1])
    
    def get_vel_vec2(self):
        return Vector2(self.state[3], self.state[4])

class System2DControl(System):
    def __init__(self, state = [0,0,0,0,0,0], dt = 0.01, controllerx = PControl(), controllery = PControl()):
        self.state = state # The current state of the system
        self.state_d1 = [0,0,0,0,0,0] # The state one time step back
        self.dt = controllerx.dt # The delta t between time steps
        self.controllerx = controllerx
        self.controllery = controllery
        self.controllery.dt = self.controllerx.dt #Force controllers to have same dt
    
    def step(self, desired = [0,0,0,0,0,0]):
        u = [0,0,0]
        u[0] = self.controllerx.step(desired[0], self.state[0])
        u[1] = self.controllery.step(desired[1], self.state[1])
        
        self.state = self.state + System1DControl.xdot(self.state, u)*self.dt
        # print(f"u:{u}  | state: {self.state}")
    
    @staticmethod
    def xdot(x: np.array, u: np.array):
        return np.array([x[3], x[4], x[5], u[0],u[1],u[2]])

    def get_pos_vec2(self):
        return Vector2(self.state[0], self.state[1])
    
    def get_vel_vec2(self):
        return Vector2(self.state[3], self.state[4])

class System2D(System):
    def __init__(self, state, dt):
        self.state = state # The current state of the system
        self.state_d1 = [0,0,0,0] # The state one time step back
        self.dt = dt # The delta t between time steps
        self.integrator = EulerIntegrator(0)


    def step(self, u=None):
        if u is None:
            self.state = self.state + System2D.xdot(self.state, [0,0])*self.dt
        elif type(u) is np.array:
            self.state = self.state + System2D.xdot(self.state, u)*self.dt
        else:
            print("Input to step function not a numpy array or None")
            raise NotImplementedError

    @staticmethod
    def xdot(x: np.array, u: np.array):
        return np.array([x[2], x[3], u[0], u[1]])

    def get_pos_vec2(self):
        return Vector2(self.state[0], self.state[1])
    
    def get_vel_vec2(self):
        return Vector2(self.state[2], self.state[3])

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
        # x: [pos, vel]
        # u: acceleration
        return np.array([x[1], u])

    def get_pos_vec2(self):
        return Vector2(self.state[0], 0)
    
    def get_vel_vec2(self):
        return Vector2(self.state[1], 0)

class SpringSystem2D(System):
    def __init__(self, state, dt, spring_constant = np.array([1,0.5])):
        # [x,y,vx,vy]
        self.state = state # The current state of the system
        self.state_d1 = [0,0,0,0] # The state one time step back
        self.dt = dt # The delta t between time steps
        self.integrator = EulerIntegrator(0)
        self.spring_constant = spring_constant

    def step(self):
        # Get the spring force on the system
        spring_force = np.zeros(2)
        spring_force[0] = -1 * self.state[0]*self.spring_constant[0]
        spring_force[1] = -1 * self.state[1]*self.spring_constant[1] 
        self.state = self.state + SpringSystem2D.xdot(self.state, spring_force)*self.dt

    @staticmethod
    def xdot(x: np.array, u: np.array):
        return np.array([x[2], x[3], u[0], u[1]])

    def get_pos_vec2(self):
        return Vector2(self.state[0], self.state[1])
    
    def get_vel_vec2(self):
        return Vector2(self.state[2], self.state[3])

