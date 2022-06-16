

from pygame import KEYDOWN, Vector2, draw, init,time, display, event, QUIT
import pygame
import numpy as np
from system_utils import System2D
from system_utils import SpringSystem, System, SpringSystem2D, System1DControl, System2DControl, SystemRobot
from controls_utils import PControl
import traceback
from time import sleep

class Game:
    def __init__(self, sys: System, keep_history: int = 0):
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

        self.sys_surface = pygame.Surface((self.width, self.height))
        self.n = 0
        self.center_offset = Vector2(self.width/2, self.height/2)
        self.keep_history_index = 0
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
        self.keep_history_index += 1
        if self.keep_history_index >= self.keep_history:
            self.screen.fill((50,50,50))
            self.sys_surface.fill((100,0,100))
            self.sys_surface.set_alpha(100)
            self.screen.set_alpha(100)
            self.keep_history_index = 0
        # Draw coordinate system axes of the system, because of the way image rendering starts (0,0) top left instead of middle
        # Draw them first so system draws can go over them

        # Get important positions in screen coordinates
        sys_center = self.system.get_pos_vec2()
        sys_vel = self.system.get_vel_vec2()
        sys_angle = self.system.get_theta()
        rw_pos = self.rotate_to_body_to_pygame_frame(Vector2(0,10), self.system)
        lw_pos = self.rotate_to_body_to_pygame_frame(Vector2(0,-10), self.system)
        local_x = self.rotate_to_body_to_pygame_frame(Vector2(10, 0), self.system)
        # print(f"rw: {rw_pos}, lw: {lw_pos}")
        # Draw actual position
        if type(self.system) == SystemRobot:
            draw.circle(self.screen,  center = sys_center + self.center_offset, radius = 2, color = (255,255,255))
            draw.circle(self.screen,  center = rw_pos + sys_center + self.center_offset, radius = 2, color = (0,255,0))
            draw.circle(self.screen,  center = local_x + sys_center + self.center_offset, radius = 2, color = (255,0,0))
            
        else:
            draw.circle(self.screen, center =sys_center, radius = 5, color = (255,0,0))
        # Draw vel vector
        draw.line(self.screen, color = (255,125,0), start_pos=sys_center + self.center_offset, end_pos=sys_center + self.center_offset + sys_vel)

        # sur, rect = self.rotate(self.sys_surface, angle=sys_angle, pivot = self.center_offset, offset=Vector2(0,0))
        # sur = pygame.transform.rotate(self.sys_surface, angle=-1+sys_angle)
        # rect = sur.get_rect(center=sys_center)
        # Draw desired position as a line
        # draw.circle(self.screen, center = self.center_offset + posd, radius = 2, color = (0,255,0))
        # draw.line(self.screen, start_pos = self.center_offset, end_pos = self.center_offset + posd, color = (0,255,0), width = 2)
        
        # self.screen.blit(self.sys_surface, dest=(0,0))
        # self.screen.blit(sur, dest=rect)        
        draw.line(self.screen, start_pos=Vector2(self.width/2, 0), end_pos=Vector2(self.width/2, self.height), width=2, color = (0,0,255))
        draw.line(self.screen, start_pos=Vector2(0, self.height/2), end_pos=Vector2(self.width, self.height/2), width=2, color = (0,0,255))
        
        self.screen.blit(self.screen, dest=(0,0))
        display.flip()
        self.clock.tick(1/self.dt)
        self.n += 1

    def rotate_to_body_to_pygame_frame(self, vec, system):
        angle = system.state[2] # Theta from state vector
        c = np.cos(angle*np.pi/180)
        s = np.sin(angle*np.pi/180)
        R = np.array(((c ,-s),  (s, c)))
        v = np.array((vec[0], vec[1]))
        rot = np.dot(R, v)
        return rot

    def rotate(self, surface, angle, pivot, offset):
        """Rotate the surface around the pivot point.

        Args:
            surface (pygame.Surface): The surface that is to be rotated.
            angle (float): Rotate by this angle.
            pivot (tuple, list, pygame.math.Vector2): The pivot point.
            offset (pygame.math.Vector2): This vector is added to the pivot.
        """
        rotated_image = pygame.transform.rotate(surface, angle)  # Rotate the image.
        rotated_offset = offset.rotate(angle)  # Rotate the offset vector.
        # Add the offset vector to the center/pivot point to shift the rect.
        rect = rotated_image.get_rect(center=pivot+rotated_offset)
        return rotated_image, rect  # Return the rotated image and shifted rect.

    # Run loop for pygame, checks events, checks exit, runs step() function
    def run(self):
        while not self.exit:
            for e in event.get():
                if e.type == QUIT:
                    self.exit = True
                if e.type == KEYDOWN:
                    if e.key == 114:
                        print("r pressed, reset sim from yaml")
                        self.system.from_yaml()
                        print(self.system.state)
            self.step()

# Run the utils to test the spring system
if __name__ == '__main__':
    try:
        # Create system we want to simulate
        s = SpringSystem([100,0] ,0.01)
        s2 = System2D([0,0,10,10], 0.01)
        s3 = SpringSystem2D(np.array([100,100,0,0]), 0.01, spring_constant=np.array([4,1.1]))
        s4 = System1DControl([100,50,0,0,0,0], dt=0.01, \
            controller=PControl(kp=10,kd=1))
        s4 = System2DControl([100,50,0,0,0,0], dt=0.01, \
            controllerx=PControl(kp=10,kd=1), controllery=PControl(kp=10,kd=1))
        s5 = SystemRobot([0,-100,0,0,0,0], controllerv=PControl(kp=10, ki=1), controllerw=PControl(kp=0.01, ki=0.1))
        # Create Game object, pass in System
        game = Game(s5, keep_history=5000)
        # Run Game
        game.run()
    except Exception as e:
        print("Caught an Exception")   
        print(traceback.format_exc())