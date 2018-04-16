#!/usr/bin/env python3.5
# -*- coding: utf-8 -*-      

# zero copy 
import socket 
import select 
import sys
import time
import datetime
import os

if len(sys.argv) < 3:
    print("bad para, Usage:  tpye port [client: ip] [client: filename]")
    exit(0)

type = int(sys.argv[1])
port = int(sys.argv[2])

if type == 1:
    # server
    sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sk.setsockopt(1,2,1)
    sk.bind(("0.0.0.0", port))
    sk.listen()
    s_list = [sk.fileno()]
    s_dict = {} 
    while 1:
        #s_list_x = s_list # do we need to reinit the fd-set ?
        rlist, wlist, elist = select.select(s_list, [], [])
        for fd in rlist:
            if fd == sk.fileno():
                # new connection
                sk2, addr_info = sk.accept()
                print("new connection from ", addr_info, sk2.fileno())
                s_list.append(sk2.fileno())
                s_dict[sk2.fileno()] = sk2
            else:
                sk3 = s_dict[fd]
                print("data coming.. ", sk3.fileno())
                total_len = 0
                #start = time.time() #only record in seconds
                start1 = datetime.datetime.now()
                while 1:
                    data = sk3.recv(1024)
                    if len(data) > 0:
                        total_len += len(data) 
                    else:
                        print("peer closes this connection.")
                        s_list.remove(sk3.fileno())
                        break
                end = time.time()
                end1 = datetime.datetime.now()
                print("recv total %d bytes, spend %s s" % (total_len, end1-start1))
else:
    # client
    serip = sys.argv[3]
    filename = sys.argv[4]
    sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sk.connect((serip, port))

    #start = datetime.datetime.now()
    f = open(filename, 'rb')
    #message = f.read()
    #print("connect to %s to transfer file %s total len %d bytes" % (serip, filename, len(message)))
    #sk.sendall(message)
    #end = datetime.datetime.now()
    #print("send all data to server, spend %s s" % (end-start))


    print("use zero-copy function to send the data")
    start1 = datetime.datetime.now()
    lenx = os.path.getsize(filename)
    print("file size ", lenx)
    ret = 0
    offset = 0
    while 1:
        ret = os.sendfile(sk.fileno(), f.fileno(), offset, lenx)
        offset += ret
        if ret == 0:
            break
    end1 = datetime.datetime.now()
    print("send all data to server using zero-copy, spend %s s" % (end1-start1))
    sk.close()


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
