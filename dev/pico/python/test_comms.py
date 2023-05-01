import numpy as np
import serial as ser
import socket
import base64

IP_SEND = "192.168.1.37"
IP_RECV = "192.168.1.38"
PORT_SEND=9900
PORT_RECV=9999

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    print("Try bind")
    sock.bind((IP_RECV, PORT_RECV))
    print("Try sockopt")
    while True:
        data, addr = sock.recvfrom(10)
        print(f"base64decode: {base64.b64decode(data)} | utf8decode: {data.decode('utf-8')}")
except Exception as e:
    print(e)