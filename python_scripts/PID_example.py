from gekko import GEKKO
import numpy as np
import matplotlib.pyplot as plt
import sys
import linecache


def PrintException():
    exc_type, exc_obj, tb = sys.exc_info()
    f = tb.tb_frame
    lineno = tb.tb_lineno
    filename = f.f_code.co_filename
    linecache.checkcache(filename)
    line = linecache.getline(filename, lineno, f.f_globals)
    print('EXCEPTION IN ({}, LINE {} "{}"): {}'.format(filename, lineno, line.strip(), exc_obj))


class Solver():
    def __init__(self):
        self.solver = GEKKO(remote=False)

        ntd = 500

        self.solver.time = np.linspace(0,50, ntd)

        self.solver.options.NODES = 2
        self.solver.options.SOLVER = 3  #Solver Type IPOPT
        self.solver.options.IMODE = 6 #MPC solution mode, will manipulate MV's to minimize Objective functions

    
    def solve_pid(self, x0, v0, kp, ki, kd, xd):
        """Look into slack variables to make this probelm solving quicker.

        Args:
            x0 ([type]): [description]
            v0 ([type]): [description]
            kp ([type]): [description]
            ki ([type]): [description]
            kd ([type]): [description]
            xd ([type]): [description]
        """        
        self.x = self.solver.Var(value = x0)  # Position of system
        self.v = self.solver.Var(value = v0)  # Velocity of system
        self.e = self.solver.Var(value = (xd-x0)) # Positional error of system
        self.pid_output = self.solver.Var() # Output of PID algorithm
        self.a = self.solver.MV(value = self.pid_output, lb=-10, ub=10) # Acceleration input to system
        self.a.STATUS = 1  # Allow GEKKO to change this value

        self.solver.Equation(self.e == xd - self.x) # Define error function
        self.solver.Equation(self.x.dt() == self.v) # Define derivative of position to equal velocity
        self.solver.Equation(self.v.dt() == self.a) # Define derivative of velocity to equal acceleration
        self.solver.Equation(self.pid_output == (self.e*kp) + (self.e.dt()*kd)) # Define pid_output from error multipled by gains (passed into this function)

        self.solver.Obj((self.pid_output - self.a)**2) # Objective function to force acceleration to equal pid_output
        # I had to do this so that I could bound acceleration, for some reason was getting erros when useing IMODE=4 (simulation mode)
        # And bounding variables, so I made acceleration an MV with bounds, and just penalized the solver for deviating from 
        # making acceleration != pid_output, squared error which is typical to remove sign

        self.solver.solve(disp=True)  # Solve the system and display results to terminal

    
    def plot_data(self):
        """Will plot
        """        
        fig = plt.figure(1)
        plt.subplot(3, 1, 2)
        plt.plot(self.solver.time, self.v.value, 'r-')
        plt.ylabel('velocity')
        plt.subplot(3, 1, 1)
        plt.plot(self.solver.time, self.x.value, 'r-')
        plt.ylabel('position')
        plt.subplot(3, 1, 3)
        plt.plot(self.solver.time, self.a.value, 'r-')
        plt.ylabel('acceleration')
        
        plt.draw()
        plt.show(block=True)

class RobotSolver():
    def __init__(self):
        self.solver = GEKKO(remote=False)

        ntd = 100

        self.solver.time = np.linspace(0, 3, ntd)

        self.solver.options.NODES = 2
        self.solver.options.SOLVER = 3  #Solver Type IPOPT
        self.solver.options.IMODE = 6 #MPC solution mode, will manipulate MV's to minimize Objective functions
    
    def solve_pid(self, w0, wd, v_base, a1_bias, kp, kd):
        self.w = self.solver.Var(value = w0) # Angular Velocity of system
        self.a1 = self.solver.MV(value = v_base, lb=-255, ub=255) # Power of Wheel 1
        self.a1.STATUS = 1
        self.a2 = self.solver.MV(value = v_base, lb=-255, ub=255) # Power of Wheel 2
        self.a2.STATUS = 1
        self.a1_unbound = self.solver.Var(value = v_base) # Unbounded velocities
        self.a2_unbound = self.solver.Var(value = v_base)
        self.e = self.solver.Var(value = wd - w0) # Error of angular velocity (wd - w) wd is desired angular velocity
        self.pid_output = self.solver.Var(self.e*kp) # Output of PID algorithm
        self.r = self.solver.Param(value = 2) # Distance from rotation center to wheel
        self.v_base = self.solver.Param(value = v_base) # Base velocity of both wheels

        self.solver.Equation(self.e == wd - self.w) # Error equation
        self.solver.Equation(self.pid_output == self.e*kp + self.e.dt()*kd) # PID algorithm equation
        self.solver.Equation(self.a1_unbound == self.v_base + self.pid_output) # Power of wheel 1 based off PID
        self.solver.Equation(self.a2_unbound == self.v_base - self.pid_output) # VelocPowerity of wheel 2 based off PID
        self.solver.Equation(self.w.dt() == (self.a2-self.a1) * self.r) # Derivative of angular velocity based on wheel velocities

        self.solver.Obj((self.a2 - self.a2_unbound)**2) # Objective functions to allow bounding of variables
        self.solver.Obj((self.a1 - self.a1_unbound)**2)
        # self.solver.open_folder()
        self.solver.solve(disp=True) # Solve the system of equations and objective functions

    def plot_data(self):
        """Will plot
        """        
        fig = plt.figure(2)
        plt.subplot(3, 1, 1)
        plt.plot(self.solver.time, self.w.value, 'r-')
        plt.ylabel('angular velocity')
        plt.subplot(3, 1, 2)
        plt.plot(self.solver.time, self.a1.value, 'b-')
        plt.ylabel('a1')
        # plt.ylim(-280, 280)
        plt.subplot(3, 1, 3)
        plt.plot(self.solver.time, self.a2.value, 'g-')
        plt.ylabel('a2')
        # plt.ylim(-280, 280)       

        plt.draw()
        plt.show(block=True)

if __name__ == "__main__":
    # s = Solver()
    # s.solve_pid(x0=0, v0=10, kp=10, ki=0, kd=10, xd=10)
    # print(s.x.value)
    # s.plot_data()
    r = RobotSolver()
    r.solve_pid(w0 = 2, wd = 0, v_base = 100, a1_bias = 10, kp = 10, kd = 1)
    r.plot_data()