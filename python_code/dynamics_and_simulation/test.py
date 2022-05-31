import numpy as np
import matplotlib.pyplot as plt
from math_utils import EulerIntegrator, Derivative

def s_dot(x, u, dt):
    xdot = x[1] 
    xddot = u[0] - (0.1*x[1])
    return np.array([xdot, xddot])

def x_next(x,u,dt):
    return x + s_dot(x,u,dt)*dt



if __name__ == "__main__":
    N = 1000
    kp = 1
    ki = 0.0
    kd = 1.5
    x_desired = 0
    m = 1
    k = 1
    dt = 0.05
    s0 = np.array([0,0])
    state_array = np.zeros(shape=(N,2))
    time = np.zeros(shape=(N))
    xd = np.zeros(shape=(N))
    u = np.zeros(shape=(N,1))
    integrator = 0
    error_before = 0 - s0[0]
    for i in range(1000):
        x_desired=np.sin(i/100)
        error = x_desired - s0[0]
        integrator += error*dt
        derivative = (error - error_before)/dt
        output = np.array([kp*error+ ki*integrator + kd*derivative])
        if i ==0:
            xd[0] = x_desired
            state_array[0,:] = s0
            time[0] = 0
            u[0] = output
        else:
            xd[i] = x_desired
            x1 = x_next(state_array[i-1,:], output, dt)
            state_array[i,:] = x1
            time[i] = i*dt
            u[i] = output
        s0 = state_array[i,:]
        error_before = error
    plt.figure(1)
    pos = state_array[:,0]
    vel = state_array[:,1]
    plt.plot(time, xd, label='desired')
    plt.plot(time, pos, label='pos')
    plt.plot(time, vel, label='vel')
    plt.plot(time, u, label='acceleration')
    plt.legend()
    plt.xlabel('time (s)')
    plt.ylabel('pos, vel')
    plt.title('State over time')
    plt.show()