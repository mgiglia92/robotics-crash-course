from comms.messages import *
from comms.serialize import *
from comms.packet import *
from comms.controller import *
import socket
import time
from input.Gamepad import Gamepad
import threading
import struct

class JoyInterface():
    def __init__(self):
        self.w = WirelessController(WirelessInterface)
        self.type = Gamepad.PS4
        self.linearAxis = 'LEFT-Y'
        self.angularAxis = 'LEFT-X'
        self.linear = 0
        self.angular= 0
        self.timer = threading.Timer(0.25, self.append_to_queue)
        self.timer.start()
        
    def linear_handler(self, position):
        self.linear=position
        # self.append_to_queue()

    def angular_handler(self, position):
        self.angular=position
        # self.append_to_queue()

    def append_to_queue(self):
        msg = Twist((self.linear, self.angular))
        pout = msg.pack()
        self.w.send(pout)
        print(f"Added: {msg} to queue, raw {self.linear} | {self.angular}")
        self.timer = threading.Timer(.25, self.append_to_queue)
        self.timer.start()

# w = WirelessController(WirelessInterface)
j = JoyInterface()

gamepadType = Gamepad.PS4
# Wait for a connection
if not Gamepad.available():
    print('Please connect your gamepad...')
    while not Gamepad.available():
        time.sleep(1.0)
gamepad = gamepadType()
print('Gamepad connected')

linearAxis = 'LEFT-Y'
angularAxis = 'LEFT-X'

gamepad.startBackgroundUpdates()

gamepad.addAxisMovedHandler(linearAxis ,j.linear_handler)
gamepad.addAxisMovedHandler(angularAxis, j.angular_handler)

try:
    while True:
        time.sleep(0.1)
        while j.w.has_packet():
            pin = j.w.get_packet()
            packet_receive(pin)
finally:
    gamepad.disconnect()