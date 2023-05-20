from comms.messages import *
from comms.serialize import *
from comms.packet import *
from comms.controller import *
import socket
import time
# ip_address = socket.gethostbyname("PICOHOME")
# print(f"pico ip_address: {ip_address}")
# p = Packet.from_bytes(b'AAAA=92cb9d68f8ec81f3c51db3af82af257d8d99a110')

print("DEBUG")
w = WirelessController(WirelessInterface)
# c = CommsController('/dev/ttyACM0')

try:
    while True:
        time.sleep(1)
except Exception as e:
    print(e)