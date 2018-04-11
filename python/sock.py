#!/usr/bin/env python3.5
# -*- coding: utf-8 -*-       
import socket 
import select 
import sys

if len(sys.argv) < 3:
    print("bad para, Usage:  tpye port [client: ip] [client: filename]")
    exit(0)

type = int(sys.argv[1])
port = int(sys.argv[2])

if type == 1:
    # server
    sk = socket.socket(socket.AF_INET, sock.SOCK_STREAM)
    sk.bind(("0.0.0.0", port))
    sk.listen()
    s_list = [sk.fileno()]
    s_dict = {} 
    while 1:
        rlist, wlist, elist = sk.select(s_list, [], [])
        for fd in rlist:
            if fd == sk.fileno():
                # new connection
                sk2 = sk.accept()
                s_list.append(sk2.fileno())
                s_dict[sk2.fileno()] = sk2
            else:
                sk3 = s_dict[fd]
                len = 0
                while 1:
                    data = sk3.recv()
                    if len(data) > 0:
                        len += len(data) 
                    else:
                        print("peer closes this connection.")
                        s_list.remove(sk3.fileno())
                        break;
                 print("recv total %d bytes" % len)
else:
    # client




'''
# epoll 
from socket import *
from select import *

sk = socket(AF_INET, SOCK_STREAM)
sk.bind(('0.0.0.0', 8888))
sk.listen()
ep = epoll()
ep.register(sk.fileno(), EPOLLIN)

s_dir = {}
while 1:
    events = ep.poll(1, 4)
    for fd,event in events:
        if fd == sk.fileno():
            sk2,addr_info = sk.accept()
            ep.register(sk2.fileno(), EPOLLIN)
            print("recv connection from ", addr_info)
            print("add socke to epoll, fd ", sk2.fileno())
            s_dir[sk2.fileno()] = sk2
        elif event == EPOLLIN:
            sk3 = s_dir[fd]
            data = sk3.recv(1024)
            if len(data) > 0:
                print("recv data: ", data)
            else:
                print("peer close this connection, fd ", sk3.fileno())
                ep.unregister(sk3.fileno())
                sk3.close()

ep.close()
'''

'''
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
