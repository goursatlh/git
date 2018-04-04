#!/usr/bin/env python3.5
# -*- coding: utf-8 -*-       
import socket
import time

connected = False
trynum = 0
sock_list = []
while not connected:
    try:
        trynum += 1
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print(sock)
        sock_list.append(sock)
        print("fd %d create" % sock.fileno())
        sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        sock.connect(('127.0.0.1', 55555))
        print(sock)
        connected = True
    except socket.error as message:
        print(sock)
        print(message)

    if not connected:
        print("reconnect")

print("tcp self connection occurs, ", trynum)
print("try to run the following command: ")
print("netstat -an|grep 55555")
time.sleep(1800)
for sk in sock_list:
    sk.close()
'''
import socket
import sys

if len(sys.argv) != 3:
    print("Bad input: usage: ./test.py type port")
    exit(0)

type = int(sys.argv[1])
port = int(sys.argv[2])

print("input: ", type, port)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

if type == 1:   # server
    print("create socket: udp fd %s" % s )
    r = s.bind(('0.0.0.0', port))
    print("bind to: 127.0.0.1:87, ret ", r)

    while True:
        data, addr = s.recvfrom(1024)
        print("received from %s:%s." % addr)
        newaddr = (addr[0], port)
        print("send to %s:%s." % newaddr)
        s.sendto(b'Hello, %s!' % data, newaddr)
else:           # client
    print("create socket: udp fd %s" % s )
    #r = s.bind(('127.0.0.1', port))
    #print("bind to: 127.0.0.1:87, ret ", r)

    for data in [b'walter', b'sam', b'carl']:
        s.sendto(data, ('127.0.0.1', port))
        print(s.recv(1024).decode('utf-8'))

s.close()
'''
