
from pygame import Vector2, draw, init, display, time, event, QUIT
import numpy as np
from system_utils import System2D
from system_utils import SpringSystem, System, SpringSystem2D
import traceback

class Game:
    def __init__(self, sys: System, keep_history: bool=False):
        init()
        display.set_caption("2D Car Kinematics (Differential Thrust Model)")
        self.width = 1281
        self.height = 721 
        self.dt = sys.dt
        self.keep_history = keep_history
        self.screen = display.set_mode((self.width, self.height))
        self.clock = time.Clock()
        self.exit = False
        self.vel_data = []
        self.time = []
        
       

        self.n = 0
        self.center_offset = Vector2(self.width/2, self.height/2)

        self.system = sys

    def step(self):
        #TODO: Make another layer to move computation out of the pygame step function.
        # Either make a Controller class that holds all this logic, or another layer?

        # Get desired position
        xd = 100*np.sin(self.dt*self.n)
        yd = 10*np.cos(self.dt*self.n)
        posd = Vector2(xd, yd)

        # Get control input
        # errp = np.array([xd, yd]) - self.system.state[0:2]
        # vd = errp*20
        # u = (vd - self.system.state[2:]) * 100

        # Project system one time step
        self.system.step()

        # Empty what was previously drawn on the screen
        if not self.keep_history:
            self.screen.fill((0,0,0))
        # Draw coordinate system axes of the system, because of the way image rendering starts (0,0) top left instead of middle
        # Draw them first so system draws can go over them
        draw.line(self.screen, start_pos=Vector2(self.width/2, 0), end_pos=Vector2(self.width/2, self.height), width=2, color = (0,0,255))
        draw.line(self.screen, start_pos=Vector2(0, self.height/2), end_pos=Vector2(self.width, self.height/2), width=2, color = (0,0,255))
        
        # Get important positions in screen coordinates
        sys_center = self.center_offset + self.system.get_pos_vec2()
        sys_vel = self.system.get_vel_vec2()

        # Draw actual position
        draw.circle(self.screen, center = sys_center, radius = 5, color = (255,0,0))

        # Draw vel vector
        draw.line(self.screen, color = (255,125,0), start_pos=sys_center, end_pos=sys_center + sys_vel)

        # Draw desired position as a line
        # draw.circle(self.screen, center = self.center_offset + posd, radius = 2, color = (0,255,0))
        # draw.line(self.screen, start_pos = self.center_offset, end_pos = self.center_offset + posd, color = (0,255,0), width = 2)
        # self.screen.blit(self.screen)

        display.flip()
        self.clock.tick(1/self.dt)
        self.n += 1


    # Run loop for pygame, checks events, checks exit, runs step() function
    def run(self):
        while not self.exit:
            for e in event.get():
                if e.type == QUIT:
                    self.exit = True
            self.step()

# Run the utils to test the spring system
if __name__ == '__main__':
    try:
        # Create system we want to simulate
        s = SpringSystem([100,0] ,0.01)
        s2 = System2D([0,0,10,10], 0.01)
        s3 = SpringSystem2D(np.array([100,100,0,0]), 0.01, spring_constant=np.array([4,1.1]))
        # Create Game object, pass in System
        game = Game(s3, keep_history=False)
        # Run Game
        game.run()
    except Exception as e:
        print("Caught an Exception")   
        print(traceback.format_exc())