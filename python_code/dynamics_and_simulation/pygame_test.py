from pygame_utils import Game
from math_utils import EulerIntegrator, Derivative
from extra_systems import *
from example_system import SystemExample

sys = SystemExample()

g = Game(sys=sys, keep_history = 1)

g.run()
