from pygame_utils import Game
from math_utils import EulerIntegrator, Derivative
from system_utils import SpringSystem

sys = SpringSystem(state=[100,0], dt=0.01)
g = Game(sys=sys, keep_history = 10)

g.run()
