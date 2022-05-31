
from pygame import Vector2, draw, init, display, time, event, QUIT
import numpy as np
from system_utils import SpringSystem

class Game:
    def __init__(self):
        init()
        display.set_caption("2D Car Kinematics (Differential Thrust Model)")
        self.width = 1281
        self.height = 721
        self.screen = display.set_mode((self.width, self.height))
        self.clock = time.Clock()
        self.ticks = 60
        self.exit = False
        self.vel_data = []
        self.time = []
        self.Ts = 0.01
        self.n = 0
        self.center_offset = Vector2(self.width/2, self.height/2)

        self.system = SpringSystem([500,0] ,self.Ts)

    def step(self):
        # Get desired position
        xd = 100*np.sin(self.Ts*self.n)
        yd = 10*np.cos(self.Ts*self.n)
        posd = Vector2(xd, yd)

        # Get control input
        # errp = np.array([xd, yd]) - self.system.state[0:2]
        # vd = errp*20
        # u = (vd - self.system.state[2:]) * 100

        # Project system one time step
        self.system.step()
        # Get system position in Vector2 format
        pos = self.system.get_pos_vec2()

        # Draw stuff
        self.screen.fill((0,0,0))
        # Draw actual position
        draw.circle(self.screen, center = self.center_offset + pos, radius = 10, color = (255,0,0))
        # Draw desired position as a line
        # draw.circle(self.screen, center = self.center_offset + posd, radius = 2, color = (0,255,0))
        # draw.line(self.screen, start_pos = self.center_offset, end_pos = self.center_offset + posd, color = (0,255,0), width = 2)
        # self.screen.blit(self.screen)
        # Draw crosshairs
        draw.line(self.screen, start_pos=Vector2(self.width/2, 0), end_pos=Vector2(self.width/2, self.height), width=2, color = (0,0,255))
        draw.line(self.screen, start_pos=Vector2(0, self.height/2), end_pos=Vector2(self.width, self.height/2), width=2, color = (0,0,255))
        display.flip()
        self.clock.tick(self.ticks)
        self.n += 1


    def run(self):
        while not self.exit:
            for e in event.get():
                if e.type == QUIT:
                    self.exit = True
            self.step()

if __name__ == '__main__':
    game = Game()
    game.run()