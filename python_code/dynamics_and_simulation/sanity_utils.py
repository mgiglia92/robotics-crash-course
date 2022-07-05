import numpy as np
from system_utils import SystemRobot
from controls_utils import *

# Rotate a vector with numpy
angle = 45 # Angle to rotate vector by
c = np.cos(angle*np.pi/180)
s = np.sin(angle*np.pi/180)
R = np.array(((c, -s), (s, c)), dtype=float)
v = np.array((0,10), dtype=float) # Vector to rotate

print(f"vector: {v}, angle: {angle}\nrotate vec:\n{np.dot(R, v)}")

s5 = SystemRobot([0,-100,0,0,0,0], controllerv=PControl(kp=10, ki=1), controllerw=PControl(kp=0.01, ki=0.1))
print(s5.to_yaml())
print(s5.from_yaml())
print("as")