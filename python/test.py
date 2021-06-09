#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

'''
#str.format()
print ( '{0},{1}' . format ( 'zhangk' ,  32 ))
print ( '{},{},{}' . format ( 'zhangk' , 'boy' , 32 ))
print ( '{name},{sex},{age}' . format (age = 32 ,sex = 'male' ,name = 'zhangk' ))

print ( '{:>8}' . format ( 'zhang' ))
print ( '{:0>8}' . format ( 'zhang' ))
print ( '{:a<8}' . format ( 'zhang' ))
print ( '{:p^10}' . format ( 'zhang' ))

print ( '{:.2f}' . format ( 31.31412 ))

print ( '{:b}' . format ( 15 ))
print ( '{:d}' . format ( 15 ))
print ( '{:o}' . format ( 15 ))
print ( '{:x}' . format ( 15 ))

print ( '{:,}' . format ( 123456789 ))
'''

'''
# orderedDict
from collections import OrderedDict
'''

'''
# function parameter comment
def f(ham: str, eggs: int = 'spam') -> "Nothing to see here":
    print("function comment: ", f.__annotations__)
    print("function parameters: ", ham, eggs)
    print(type(ham),type(eggs))

f("www")
f("www", 5)
'''

'''
from wxpy import *
bot = Bot()
friend = bot.friends().search('goursatlh', sex=MALE)[0]
friend.send("Hello, this is your little dog.")
'''

'''
# pass vs return
def func():
    for i in range(10):
        if i%2 == 0:
            #continue
            pass
        print(i)

func()

def func1():
    #pass

func1
'''

# deque
# deque vs list: 1, faster(350+ faster than list) 2, thread safety
'''
import time
import sys
from collections import deque

t0 = time.clock()
l = []
for i in range(100000):
    l.append(i)
for i in range(100000):
    l.insert(0,i)
t1 = time.clock()
print("list ", t1-t0)
print(len(l), l.__sizeof__(), sys.getsizeof(l))

t0 = time.clock()
d = deque() 
for i in range(100000):
    d.append(i)
for i in range(100000):
    d.appendleft(i)
t1 = time.clock()
print("deque ", t1-t0)
print(len(d), d.__sizeof__(), sys.getsizeof(d))

# indexed access for reque is mush slower than list
t0 = time.clock()
for i in range(len(d)):
    d[i] = 0
t1 = time.clock()
print("deque indexed access ", t1-t0)

t0 = time.clock()
for i in range(len(l)):
    l[i] = 0
t1 = time.clock()
print("list indexed access ", t1-t0)
'''

'''
# sum
pagesize = 4*1024
l = [16,41,18,8,7,2,1,0,2,2,10]
f = lambda i, l: l[i] * pow(2, i)
print(f(3,l))

sum = 0
for i in range(11):
    sum += f(i, l)
    print("set ", f(i,l))
print("sum is ", sum*pagesize//1024)
'''


'''
#scrapy: web crawler backbone
from queue import Queue
init_page = "www.ifeng.com"

print("hello web crawler")
url_queue = Queue()
seen = set()
seen.insert(init_page)
url_queue.put(init_page)

while True:
    if url_queue.size():
        current_url = url_queue.get()
        store(current_url)
        for next_url in extract_urls(current_url):
            if next_url not in seen:
                seen.put(next_url)
                url_queue.put(next_url)
    else:
        break
'''

'''
# mmap
import mmap
import contextlib
import time

with open("txt", "w") as f:
    f.write("\x00"*1024)

with open("txt", "r+b") as f:
    with contextlib.closing(mmap.mmap(f.fileno(), 1024, access=mmap.ACCESS_WRITE)) as m:
        for i in range(1, 10001):
            m.seek(0)
            s = "msg" + str(i)
            s.rjust(1024, "\x00")
            m.write(s.encode())
            m.flush()
            time.sleep(1)
'''


# with ... as ...
'''
with open("txt") as file:
    data = file.read()
    print(data)
'''

'''
class Sample():
    def __enter__(self):
        print("enter")
        return self
    def __exit__(self, type, value, trace):
        print("type: ", type)
        print("vaule: ", value)
        print("trace: ", trace)
    def do_something(self):
        print("do something")
        bar = 1/0
        return bar+10

with Sample() as sample:
    sample.do_something()
print("come here")
'''


'''
# parameters for python functions

def power(x, n=2):
    sum = 1
    while n > 0:
        sum = sum*x
        n -= 1
    return sum

print(power(5))
print(power(5,3))


def stu_info(name, age, city='ShenZhen', male='man'):
    print("------------------")
    print("name:", name)
    print("age:", age)
    print("sity:", city)
    print("male:", male)

stu_info("carl", 7)
stu_info(age=7, name="carl")

def extendList(val, list=[]):
    print("id for list: ", id(list))
    list.append(val)
    return list

list1 = extendList(10)
print(list1)
list2 = extendList(123,[])
list3 = extendList('a')
print(list1)

print(list1)
print(list2)
print(list3)
'''



'''
import fcntl
import os, time

FILE = "/home/wt/code/git/tmp.txt"
if not os.path.exists(FILE): # create the counter file if it doesn't exist 
  file = open(FILE, "w")
  file.write("0")
  file.close()

for i in range(20):
  file = open(FILE, "r+")
  print("\ntry acquire lock...")
  fcntl.flock(file.fileno(), fcntl.LOCK_EX)
  print('acquire lock success')

  counter = int(file.readline()) + 1

  file.seek(0)
  file.write(str(counter))
  print(os.getpid(), "=>", counter)

  time.sleep(2)
  file.close() # unlocks the file 

  print('release lock')
'''

'''
# copy(shallow and deep) and assignment
L = [1, 12]
L1 = L
L2 = L.copy()
L3 = list(L)

print('L ', id(L), L)
print('L1', id(L1), L1)
print('L2', id(L2), L2)
print('L3', id(L3), L3)

L1[1] = 39
print('L', L)
print('L1', L1)
print('L2', L2)
print('L3', L3)


will = ["Will", 28, ["Python", "C#", "JavaScript"]]
wilber = will
print("id", id(will), will, "id ele", [id(ele) for ele in will])
print("id", id(wilber), wilber, "id ele", [id(ele) for ele in wilber])
 
will[0] = "Wilber"
will[2].append("CSS")
print("id", id(will), will, "id ele", [id(ele) for ele in will])
print("id", id(wilber), wilber, "id ele", [id(ele) for ele in wilber])

print("shallow copy")
wilber2 = list(will)
print("id", id(will), will, "id ele", [id(ele) for ele in will])
print("id", id(wilber2), wilber2, "id ele", [id(ele) for ele in wilber2])
will[0] = "Hello"
will[2].append("CPP")
print("id", id(will), will, "id ele", [id(ele) for ele in will])
print("id", id(wilber2), wilber2, "id ele", [id(ele) for ele in wilber2])

will2 = will[:]
print("id", id(will), will, "id ele", [id(ele) for ele in will])
print("id", id(will2), will2, "id ele", [id(ele) for ele in will2])

print("deep copy")
import copy
wilber3 = copy.deepcopy(will)
print("id", id(will), will, "id ele", [id(ele) for ele in will])
print("id", id(wilber2), wilber3, "id ele", [id(ele) for ele in wilber3])
will[2].append("Shell")
print("id", id(will), will, "id ele", [id(ele) for ele in will])
print("id", id(wilber2), wilber3, "id ele", [id(ele) for ele in wilber3])
'''


'''
list = [ [ ] ] * 5
print(list, len(list))
list[0].append(10)
print(list)
list[1].append(20)
print(list)
list.append(30)
print(list)
'''

'''
#closure
def greeting_conf(prefix):
    def greeting(name):
        print(prefix, name)
    return greeting

mGreeting = greeting_conf("Good Morning")
mGreeting("Wilber")
mGreeting("Will")

aGreeting = greeting_conf("Good Afternoon")
aGreeting("Wilber")
aGreeting("Will")

print(type(mGreeting))
print(type(mGreeting.__closure__[0]))
print(mGreeting.__closure__[0].cell_contents)
print(aGreeting.__closure__[0].cell_contents)

def multipliers_lambda():
  return [lambda x : i * x for i in range(4)]

def multipliers():
    l = []
    j = 238
    m = "hello world"
    for i in range(4):
        #print('id for i ', id(i))
        #def func(x, i=i):# i is no longer a closure element
        def func(x):   # i and j are all elements of closure, all outside parameters which will be used in inter func will be stored in __closure__[]
            #print("m: ", m) # m must be used, otherwise it will not be included in __closure__[]
            return x*i*j
        l.append(func)
    return l

print([m(2) for m in multipliers_lambda()])
print([m(2) for m in multipliers()])

l1 = multipliers()
print(type(l1), type(l1[0]))

f = l1[0]
#print(id(f.__closure__[0].cell_contents))
print(f.__closure__[0].cell_contents)
#print(f.__closure__[1].cell_contents)
#print(f.__closure__[2].cell_contents)
print(f(2))

f2 = l1[1]
#print(id(f2.__closure__[0].cell_contents))
print(f2.__closure__[0].cell_contents)
#print(f2.__closure__[1].cell_contents)
#print(f2.__closure__[2].cell_contents)
print(f2(2))
'''



'''
import random

l = [ random.randint(1, 100) for i in range(10)]
print(l)
print(l[::2])
print([ i for i in l[::2] if i % 2 == 0])



f = lambda  x: x * x 
print(f(2))

l = [f(x) for x in range(4)]
print(l)

def multipliers():
  #return [lambda x : i * x for i in range(4)]
  return [lambda x,i=i : i * x for i in range(4)] 

print([m(2) for m in multipliers()])

l = [i for i in range(10) if i % 2 == 0]
print(l)
'''
'''
# matplotlib
import sys
import tkinter
import matplotlib.pyplot as plt

x = range(1000)
y = [sys.getrefcount(i) for i in x]
fig, ax = plt.subplots()
plt.plot(x, y, '.')
ax.set_xlabel('number')
ax.set_ylabel('sys.getrefcount(number)')
plt.show()
'''

'''
#@functools.warps()
import functools


def logged(func):
    @functools.wraps(func)
    def with_logging(*args, **kwargs):
        print(func.__name__, " was called")
        return func(*args, **kwargs)
    return with_logging

@logged
def f(x):
   """does some math"""
   #print('call func 1')
   return x + x * x

print(f.__name__, f.__doc__)
f(2)
'''

'''
str = 'hello worldworld'
ret = str.rfind('w')
print(ret)
str1 = str[ret+1:]
print(str1)
str2 = str[:ret]
print(str2)
'''

'''
# class
class App():
    c = 200
    def __init__(self, a, b):
        self.a = a
        self.b = b
        c = 5
    def app1(self, val):
        App.c = val
    
# __call_ for class
    def __call__(self, a, b):
        self.a = a
        self.b = b

a = App(1,2)
print(a.a, a.b, a.c, App.c)
a.app1(35)
print(a.a, a.b, a.c, App.c)

a(2,3)
print(a.a, a.b)

#a.app1(22)

b = App(10,20)
print(b.a, b.b, b.c, App.c)

#class attribute and instance attribute
class A():
    name = "this is A"
class B(A):
    pass
b = B()
b.name = "this is B" #instance attribute will override the class attribute which has the same name
print("B.name", B.name)
print("b.name", b.name)
c = B()
print("c.name", c.name)

# classmathod
class Kls(object):
    no_inst = 0
    def __init__(self):
        Kls.no_inst = Kls.no_inst + 1
        self.no_inst = 22
    @classmethod
    def get_no_of_instance(self): #input para is class, not instance
        return self.no_inst

ik1 = Kls()
print(ik1.get_no_of_instance())
ik2 = Kls()
print(ik1.get_no_of_instance())
print(ik1.no_inst, ik2.no_inst)
'''

'''
# class type
class Student(object):
    "describe class Student"
    __x = 2
    def __init__(self, name, score):
        self.name = name
        self.__score = score 
    def printc(self):
        print(self.name, self.__score, self.__x)

r = Student("walter", 20)
#print(r.__x)
#print(dir(r))
r.printc()
#print(r.__score) #error, private element, can't be accessed outside of the class
#r.__x = 22 #equal to add a new public element for object r
#print(r.__x)
#print(r._Student__score) #not recommend # __score will be expanded to _Student__score by python compiler
'''

'''
# inherit
class animal():
    def printx(self):
        print("this is animal...")
class cat(animal):
    def printx(self):
        print("this is cat...")
class people():
    def printx(self):
        print("this is people...")
a = animal()
a.printx()
c = cat()
c.printx()
print(isinstance(c, animal))
p = people()
print(isinstance(p, animal))

#polymorphic
def run(animal):  #file-like object
    animal.printx()
run(a)
run(c)
run(p)

#advanced features 
# __slots__ list the attributes of the class, you can't add a new member that isn't in slots
class AA:
    __slots__ = ("name", "score")
    def __init__(self, name ,score):
        self.name = name
        self.score = score
a = AA("walter", 90) 
#a.age = 18 # this element is not in the __slots__
print(a.name, a.score)
print(a.__slots__)
'''

'''
# add func to a instance and a class
class Student(object):
    pass

s = Student()

# add a method to an instance : MethodType = class method(object)
from types import MethodType
def set_age(self, age):
    self.age = age
def set_score(self, score):
    self.score = score

s.set_age = MethodType(set_age, s)
s.set_age(2)
print(s.age)

# add a method to a class
Student.set_score = set_score
s.set_score(69)
print(s.score)
'''

'''
# @property change attribute to functions
class Student():
    def __init__(self, score=0):
        self._score = score

    @property
    def score(self): #equire to get_score()
        print("get") 
        return self._score

    @score.setter
    def score(self, score): #equire to set_score()
        if not isinstance(score, int):
            raise ValueError("score must be an int")
        if score < 0 or score > 100:
            raise ValueError("score must be in (0,100)")
        print("set", score)
        self._score = score
s = Student()
s.score = 2
print(s.score)
'''

#print(s.score)
#print(s.score) # you can only to use s.score, s.score = xx, you can't use s.score()/s.score(value)

'''
# function with the same name: the second funciton will take effect.
def test(a=1):
    print("AAA", a)

def test():
    print("BBB")
test()
'''

'''
# variable namespace
x = 'hello world'
print(locals())

def func():
    print(locals())
func()

def func1(x):
    print(locals())
func1(x)


def funcx(x, y):
    print(x-y)
funcx(5, 2)
funcx(y=2, x=5)
'''


'''
# getattr()/setattr()/hasattr()
a = [1, 3, 0]
f = getattr(a, '__len__')
print(f)
print(f())

try:
    setattr(a, 'name', 'test')
except BaseException as e:
    print("except: ", e)

class a:
    c = 1
A = a()
print(hasattr(A, 'c'))
setattr(A, 'name', "test")
setattr(a, 'name', "test")
print(hasattr(A, 'name'))
print(A.name)
print(hasattr(a, 'name'))

# so you can't use 'setattr()' to add attr to a build-in type, such as 'list', right?
'''

'''
import asyncio

async def compute(x, y):
    print("Compute %s + %s ..." % (x, y))
    await asyncio.sleep(1.0)
    return x + y

async def print_sum(x, y):
    result = await compute(x, y)
    print("%s + %s = %s" % (x, y, result))

loop = asyncio.get_event_loop()
loop.run_until_complete(print_sum(1, 2))
loop.close()
'''

'''
# asterisks in function parameters
def function_with_one_star(*t):
    print(t, type(t))

def function_with_two_stars(**d):
    print(d, type(d))

function_with_one_star(1, 2, 3)
function_with_two_stars(a = 1, b = 2, c = 3)

def func(a=1, b=2, c=3):
    print(a+b+c)
func()
func(4,5,6)
func(2,3)
func(100)
'''

#decorator
def document_it(func):
    def new_function(*args, **kwargs):
        print('Running function:', func.__name__)
        print('Positional arguments:', args)
        print('Keyword arguments:', kwargs)
        result = func(*args, **kwargs)
        print('document_it Result:', result)
        return result
    return new_function

def add_ints(a, b):
    return a + b

cooler_add_ints = document_it(add_ints)
cooler_add_ints(3, 5)

def log(func):
    print('call %s():' % func.__name__)
    return func

def log1(func):
    def wrapper(*args, **kw):
        print('call %s():' % func.__name__)
        return func(*args, **kw)
    return wrapper

def now(name):
    print('2017-11-27 ', name)

now = log(now)
now('carl')
#log(now)('carl')
#now('carl')

@log1
def now1(name):
    print('2017-12-27', name)
#now1('carl')

#issues
#1, why define a new function in log1. 
#decorator end


'''
#variable parameters

def func_with_one_start(*t):
    print(t, type(t))
    for i in t:
        print(i, end=' ')
    print()

def func_with_two_start(**t):
    print(t, type(t))
    for i,j in t.items():
        print(i, ':', j, end=' ')
    print()

func_with_one_start(1, 2, 3)
func_with_two_start(a=1, b=2, c=3)

def func_1(a, *b):
    print(a, b)
    for i in b:
        print(i)

func_1(1, 20, 3)

def lazy_sum(*args):
    def sumx():
        ax = 0
        for para in args:
            ax = ax + para
        return ax
    return sumx

f = lazy_sum(2,1,3,5)
print(f)


def count():
    fs = []
    for i in range(1, 4):
        def f():
             return i*i
        fs.append(f)
    return fs

f1, f2, f3 = count()
print(f1(), f2(), f3())

def count1():
    def f(j):
        def g():
            return j*j
        return g
    fs = []
    for i in range(1, 4):
        fs.append(f(i))
    return fs

f11, f22, f33 = count1()
print(f11(), f22(), f33())

'''

#decorator end

'''
#socket
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
        print("received from %s:%s." % (addr.host, addr.port))
        s.sendto(b'Hello, %s!' % data, addr)
else:           # client
    print("create socket: udp fd %s" % s )
    #r = s.bind(('127.0.0.1', port))
    #print("bind to: 127.0.0.1:87, ret ", r)

    for data in [b'walter', b'sam', b'carl']:
        s.sendto(data, ('127.0.0.1', port))
        print(s.recv(1024).decode('utf-8'))

s.close()
'''


'''
#yield from
def inner():
    coef = 1
    total = 0
    while True:
        #try:
            input_val = yield total         # a = yield b     => b is return value, a is input parameter
            total = total + coef * input_val
        #except SwitchSign:
        #    coef = -(coef)
        #except BreakOut:
        #    return total

def outer():
    print("Before inner(), I do this.")
    total = 0
    a = inner()
    total = a.send(None)
    while True:
        input = yield total
        total = a.send(input)
    print("After inner(), I do that.")

def outer2():
    print("Before inner(), I do this.")
    yield from inner()
    print("After inner(), I do that.")
    return 0

g = outer()
#g = outer2()
print(type(g), g)


#g = inner()
#g.send(None)
#for i in range(1,5):
#    print("outside gen: ", g.send(i))
'''

'''
#part1 
def consumer():
    r = ''
    print('[CONSUMER] first in ')
    while True:
        n = yield r # if raise by send(args), n = args, if send(None)/next(), n = None.
        print('[CONSUMER] get n ', n)
        if not n:
            print('return ', n)
            return
        print('[CONSUMER] Consuming %s...' % n)
        r = '200 OK'

def produce(c):
    print('[PRODUCER] first in', type(c))
    c.send(None)
    print('[PRODUCER] come here')
    n = 0
    while n < 5:
        n = n + 1
        print('[PRODUCER] Producing %s...' % n)
        r = c.send(n)
        print('[PRODUCER] Consumer return: %s' % r)
    c.close()

c = consumer()
produce(c)
'''



'''
#part2 
import asyncio

@asyncio.coroutine
def hello():
    print("Hello world!")
    r = yield from asyncio.sleep(3)
    print("Hello again!")
print("Is coroutine ?", asyncio.iscoroutinefunction(hello))

loop = asyncio.get_event_loop()
print(type(loop))
loop.run_until_complete(hello())
loop.close()
'''

#part3
'''
import threading
#@asyncio.coroutine
def hello():
    print('Hello world! (%s)' % threading.currentThread())
    yield from asyncio.sleep(1)
    print('Hello again! (%s)' % threading.currentThread())

async def hello2():
    print('Hello world 3.5! (%s)' % threading.currentThread())
    await asyncio.sleep(1)
    print('Hello again 3.5! (%s)' % threading.currentThread())


print(type(hello))
loop = asyncio.get_event_loop()
tasks = [hello(), hello()]
tasks2 = [hello2(), hello2()]
loop.run_until_complete(asyncio.wait(tasks))
loop.run_until_complete(asyncio.wait(tasks2))
loop.close()
'''
#part4
'''
@asyncio.coroutine
def wget(host):
    print('wget %s...' % host)
    connect = asyncio.open_connection(host, 80)
    reader, writer = yield from connect
    header = 'GET / HTTP/1.0\r\nHost: %s\r\n\r\n' % host
    writer.write(header.encode('utf-8'))
    yield from writer.drain()
    while True:
        line = yield from reader.readline()
        if line == b'\r\n':
            break
        print('%s header > %s' % (host, line.decode('utf-8').rstrip()))
    # Ignore the body, close the socket
    writer.close()

loop = asyncio.get_event_loop()
tasks = [wget(host) for host in ['www.sina.com.cn', 'www.sohu.com', 'www.163.com']]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
'''
#####################################################################################



'''
print("usage for ftp")

import sys 
if len(sys.argv) != 3:
    print("Bad input: usage: ./network.py src_file dst_file")
    exit(0)
src = sys.argv[1]
dst = sys.argv[2]
print("ftp 10.88.21.228, to get %s to %s" % (src, dst))

from ftplib import FTP
f = FTP("10.88.21.228")
f.login("wt", "wt")
print(f.dir())

f.retrlines('RETR %s' % src, open(dst, 'w').write)

f.quit()
'''

'''
#try...except...finally...
import logging
try: 
    print('try...')
    #r = 10 / 0
    #r = 10 / int("a") # it will get the first exception
    r = 10 / a
    print('result:', r)
except ZeroDivisionError as e:
    print('except1:', e)
    #logging.exception(e)
except ValueError as e:
    print('except2:', e)
    #logging.exception(e)
except BaseException as e: # BaseException is the base class for all exceptions
    print('except3:', e)
    logging.exception(e)
finally:
    print('finally...')
print('END')

#define an exception to catch
def func(a):
    if a < 0:
        raise ValueError("Error value %s" % a)
    return

def func2(a):
    func(a)
    return

try:
    func2(-1) # we can catch the exception here, not need to add the try.. in every function
except BaseException as e:
    print("except: ", e)
    #raise #don't process the exception here, raise to outside
finally:
    print("finally")

# note:
# 1, If the exception has not been catched, it will be throwen to the outside until the python interpreter, then the program will be ended. 
# 2, If the function has not handled the exception, it can only record the exception then throw to outside function to process using raise. 
'''

## process begin
'''
#process apis
import os
print("Proces(%d) start..." % os.getpid())
pid = os.fork()
if pid == 0:
    print("Son process(%d), parent process(%d)" % (os.getpid(), os.getppid()))
    exit()
else:
    print("Parent process(%d)" % os.getpid())

#process api cross-platform. There are two ways to start processes, os.fork() or multiprocess.Process(). The previous one only works 
#for Unix-like system, the one in the back is used for cross-platform.
from multiprocessing import Process
def run_proc(name):
    print("Child process %s/%d run" % (name, os.getpid()))

if __name__  == "__main__" :
    print("Parent process %d" % os.getpid())
    p = Process(target=run_proc, args=("test",))
    p.start()
    p.join()
    print("process %d end" % os.getpid())

#thread apis
import threading
def loop():
    print("Thread(%s) start..." % threading.current_thread().name)
    
print("Thread(%s) is running..." % threading.current_thread().name)
t = threading.Thread(target=loop, name="loop thread" )
t.start()
t.join() #wait t to end up
print("Thread(%s) end..." % threading.current_thread().name)

#subprocess
import subprocess
print("$ nslookup www.python.org")
cmd = ["ps", "-aux"]
#r = subprocess.call(["nslookup", "www.python.org"])
r = subprocess.call(cmd)
print("Exit code: ", r)
'''
'''
#comm between processes
from multiprocessing import Process,Queue
import os,time,random
def reader(q):
    print("Process to read, PID %s" % os.getpid())
    while True:
        value = q.get(True)
        print("Get %s from queue" % value)

def writer(q):
    print("Process to write, PID %s" % os.getpid())
    for value in ["A", "B", "C"]:
        print("Put %s to queue" % value)
        q.put(value)
        time.sleep(random.random())

if __name__ == '__main__':
    print("Process start, PID %s" % os.getpid())
    q = Queue()
    pr = Process(target=reader, args=(q,))
    pw = Process(target=writer, args=(q,))
    pr.start()
    pw.start()
    pw.join()
    pr.terminate()
'''

## process end

'''
# namespace and scope
def scope_test():
    def do_local():
        #print("do_local", spam)
        spam = "local spam"

    def do_nonlocal():
        nonlocal spam
        spam = "nonlocal spam"

    def do_global():
        global spam   #create a variable in global namespace 
        spam = "global spam"

    spam = "test spam"
    do_local()
    print("After local assignment:", spam)
    do_nonlocal()
    print("After nonlocal assignment:", spam)
    do_global()
    print("After global assignment:", spam)

scope_test()
print("In global scope:", spam)

saw = "hello"
def testxx():
    #nonlocal saw
    saw = "world"
    print("testxx saw ", saw)
testxx()

def test():
    def do_nonlocal():
        nonlocal spam2
        spam2 = "fuck"
        def do_nonlocal2():
            nonlocal spam2
            spam2 = "nonlocal2 spam"
        do_nonlocal2()
    
    spam2 = "test spam"
    do_nonlocal()
    print("after nonlocal2 asssignment:", spam2)
test()


sam = 'fuck'
def func12():
    #print(sam) # error, sam in this function hides sam in the global namespace
    sam = 'you'
func12()
'''

'''
#strip(),rstrip(),lstrip()
#Return a copy of the string S with leading whitespace removed.
str = "   hello world    \n";
print(str)
print(str.strip())
print(str.rstrip())
print(str.lstrip())

str1 = "***hello world    ***";
print(str1)
print(str1.strip('*'))  # * can't be identified if lines end with \n
print(str1.rstrip('*'))
print(str1.lstrip('*'))
'''

'''
# rex in python
import re
import os # execute shell command in .py

fs = os.popen("who", "r")
for eachline in fs:
    #print(eachline.rstrip())
    print(re.split(r"\s+", eachline.rstrip()))
    #print(re.match(r"\d+", eachline.rstrip()))
    print(re.match(r"[0-9a-zA-Z\-]+", eachline.rstrip()))
fs.close()

m = re.match(r"^(\d{3})-(\d{8})$", "010-12345678")
print(m.group(0))
print(m.group(1))
print(m.group(2))
'''

'''
# file io
f = open("txt", "r+")         
print("read1: ", f.read().strip()) # read all data from this file
print("read2: ", f.read().strip())
f.write("fuck you then\n")
f.seek(0)
print("write: ", f.read().strip())

print("readlines: ")
f.seek(0)
for i in f.readlines():
    print(i.strip())

f.seek(0)
print("readline1: ", f.readline().strip())
print("readline2: ", f.readline().strip())

print("dump the whole file")
f.seek(0)
for i in f:
    print(i.strip())
f.close()
'''

'''
# join()
str = "-"
#seq = [1", "2", "3", "4"]
seq = [b"1", b"2"]
print(b" ".join(seq))

# file input paras
import sys
print("argv[0] is ", sys.argv[0], "argc is ", len(sys.argv))
print(type(sys.argv[1]))    # warnning: type is str, not int

for i in range(1, len(sys.argv)):
    sys.argv[i] = int(sys.argv[i])
    print(" ", sys.argv[i])

def my_abs(a):
    if not isinstance(a, (int, float)):
        raise TypeError("bad operand para")
    if a >= 0:
        return a
    else:
        return -a

print(my_abs(-128))
#my_abs("a")

if __name__ == "__main__":
    print(my_abs(-222))

# input parameters
import sys
print("input paras list:")
for i in range(0, len(sys.argv)):
    print(" ", sys.argv[i], end="")
print()

#print
print("hello world.","fuck you then")
name = input("please input your name: ")
print("hello", name)

#enumerate()
import string
s = string.ascii_lowercase
e = enumerate(s)
print(s)
print(list(e))
print("name %s: age %d"%("walter", 33))

for i, j in enumerate(s):
    print(i,j)

#class
class Student(object):
    def __init__(self, name, score):
        self.name = name
        self.score = score
    def print_score(self):
        print(self.name, self.score)

boy = Student("jerry", 20)
boy.print_score()

#list 
classmates = ["jerry", "tom", "carl"]
print(classmates)
print(len(classmates))
print(classmates[0])
print(classmates[-1])
classmates.append("walter")
print(classmates)
classmates.insert(1, "james")
print(classmates)
classmates.pop()
print(classmates)
classmates.pop(1)
print(classmates)
classmates[0] = 1100
print(classmates)

for stu in classmates:
    print(stu)

#for
sum = 0
for i in [0,1,2,3,4,5,6]:
    sum += i
print(sum)

b = range(100)
print(b)
b = list(range(100))
print(b)

#how to confirm an obj is an iterable obj
from collections import Iterable #only import Iterable, not all items in collections
i = isinstance("abc", Iterable)
print(i)
i = isinstance(123, Iterable)
print(i)
i = isinstance([1, 2, 3], Iterable)
print(i)

for i,value in enumerate([1, 2, 3]):
    print(i, value)

#tuble
i = (1,2,3,4)
print(i)
i = (1,)
print(i)

s = 100
i = (s,)
print(i)
s = 200
print(i)

#slice
s = ["hello", "world", "fuck", "you"]
print(s[1:2])
print(s[-3:-1])

L = list(range(100))
print(L[:10:2])
print(L[::5])

S = "hello, world"
print(S[:5])
print(S[-5:])
print(S[-5:-1])
#dict
D = {"a":1, "b":2, "c":3, "d":4, "e":5}
print(D)
print(D["a"])
D.pop("a")
print(D)

#dict: add/modify/del entry 
D["a"] = 22
D[77] = 99
del D["b"]
print(D)
print(len(D))


for i in D:
    print("key:", i)

for i in D.values():
    print("value:", i)

for i,j in D.items():
    print("key:", i, "value:", j)

#list comprehension
l = [i*i for i in range(1,30) if i % 2 == 0]
print(l)

l = [i+j for i in "abc" for j in "efg"]
print(l)

import os
l = [d for d in os.listdir(".")]
print(l)

L = ["Hello", "WORLD", "AppLE", "THINKpad"]
l = [i.lower() for i in L ]
print(l)
'''

'''
#generator
L = [x * x for x in range(1, 10)]
print(L)

g = (x * x for x in range(1, 10))
print("first cycle ")
for i in g:
    print(i, end=' ')
print()
for i in g:
    print(i, end=' ')

def h():
        print("hello ")
        m = yield 5
        print("m: ", m)
        d = yield 12
        print("d: ",d)
        print("world")
c = h()
c.__next__()
c.send(None)
#c.__next__()

# a = yield b
def countdown(n):
        print("Counting down from", n)
        while n >= 0:
                newvalue = (yield n)
                print("newvalue ", newvalue)
                # If a new value got sent in, reset n with it
                if newvalue is not None:
                        n = newvalue
                else:
                        n -= 1

c = countdown(10)
for x in c:
        print(x)
        if x == 10:
                c.send(3)


#print("next ", next(g))

def fib(max):
    n,a,b = 0,0,1
    print("come here enter ", n)
    while n < max:
        print("come here 1 ", n)
        yield b
        print("come here 2 ", n)
        a, b=b, a+b
        n = n + 1
        if b > 20:
            print("greate than 20")
            return
    return "done"
g = fib(20)
print(type(g))
print(next(g))
print(next(g))
print(next(g))
print(next(g))
print(next(g))

'''
'''
def circle(x):
    return 3.14 *x *x
print(circle(2))

def my_abs(x):
    if not isinstance(x, (int, float)):
        raise TypeError("bad input")
        print("come herr")
        if x > 20:
            print("bigger than 20")
        else:
            print("smaller than 20")

my_abs('s')

import math

def move(x, y, step, angle=0):
    nx = x + step * math.cos(angle)
    ny = y + step * math.sin(angle)
    return nx, ny
print(math.pi)
print(math.sin(math.pi/3))
r = move(10, 20, 3, math.pi/6)
print(r)

def power(x, y = 2):
    s = 1
    while y > 0:
        s = s * x 
        y -= 1
    return s
print(power(2, 3))
print(power(2)) #use default parameter

def enroll(name, gender, age=6, city="beijing"):
    print("name", name)
    print("gender", gender)
    print("age", age)
    print("city", city)
enroll("sam", "M")
enroll("tom", 'F', city="shenzhen")
'''
'''
def add_end(L=[]):
    L.append("END")
    return L
print(add_end([1, 2, 3]))
print(add_end())
print(add_end())
print(add_end())
'''
'''
# list, tuple, dict, set
L = [3, 2, 1]
print(L)
print(L[0], L[1])
L.append("hello")
print(L)
L[0] = 0.3
print(L)
L.insert(1, [9,9])
print(L)
L.append(2) #insert duplicate element
print(L)
L.pop()
print(L)
L.pop(1)
print(L)

T = (2, 1);
print(T)
print(T[0])
#T[0] = 3

T1 = (1, "hello", T, L)
print(T1)
L[0] = 99
print(T1)

D = {"sam":1, "gary":3}
print(D)
print(D["sam"])
D[100] = "jerry"
print(D)
print(D.get("sam", -1))
print(D.get(100, -1))
print(D.get(1000, -1))
print(D)
D.pop(100)
print(D)
D[T] = 20
print(D)
#D[T1] = 30

S = set([10, 2, 3, 0, 0, 2])
print(S)
S1 = set(T)
print(S1)
print(D)
S2 = set(D)
print(S2)
'''
'''
def funcx(*n):
    sum = 0
    for i in n:
        sum = sum + i*i
    return sum
print(funcx(*list(range(3))))
print(funcx())
'''
'''
def fact(n):
    if n == 1:
        return n
    else:
        return n * fact(n-1)
print(fact(100))

def fact1(n):
    return fact_iter(n, 1)

def fact_iter(num, product):
    if num == 1:
        return product
    return fact_iter(num - 1, num * product)
print(fact1(100))
'''

'''
#slice for list and tuple 
L = [4, 2, 7, 0, 8]
print(L[0:4])
print(L[-3:-2])
L = list(range(100))
print(L[-10:-1])
print(L[-10:])
print(L[-10::2])
print(L[0:10:3])
T = tuple([4, 2, 7, 0, 8])
print(T)
print(T[:4])
L1 = list(range(10))
def printx():
    i = 1
    while i <= 10:
        print(L1[::i])
        i+=1
printx()
'''


'''
#map reduce
from collections import Iterable
from collections import Iterator
def f(x):
    return x *x
r = map(f, [1, 2, 3, 4, 5, 6, 7, 8, 9])
print(isinstance(r, Iterable))
print(isinstance(r, Iterator))
print(list(r))

from functools import reduce
def add(x, y):
    return x + y
print(reduce(add, [1, 3, 5, 7, 9]))

r = map(str, [1, 2, 3, 4, 5])
print(list(r))

#"123" => 123
print("str2int")
def f1(x):
    return {'0':0, '1':1, '2':2, '3':3, '4':4, '5':5}[x] # map can be indexed?

def f2(x1, x2):
    return x1 * 10 + x2

r1 = map(f1, "212345")
r2 = reduce(f2, r1)
print(r2)
'''


'''
#list and iterator and iteratorable
r = [x * x for x in range(10)]
print(r)

g = (x * x for x in range(10))
print(next(g))
for i in g:
    print(i, end=' ')
print()

def fib(x):
    n, a, b = 0, 0, 1
    while n < x:
        #print(b, end=' ')
        yield b
        a, b = b, a + b
        n += 1
    return "done"
for i in fib(20):
    print(i, end=' ')

# generator and exception catch
a = "hello world"
def odd():
    print('step 1')
    yield 1
    print('step 2')
    return
    yield(3)
    print('step 3')
    yield(5)

o = odd()
while True:
    try: 
        print(next(o))
    except StopIteration as e:
        print("catch an error: ", e.value)
        break




#type()/isinstance()/dir()
print("a is ", type(a))
print("c is ", type(a))
print("p is ", type(a))
print(dir(a))
print(a.__sizeof__())
print(dir("hello"))

#advanced 
# __slots__
class AA:
    __slots__ = ("name", "score")
    def __init__(self, name ,score):
        self.name = name
        self.score = score
a = AA("walter", 90)

print(a.name, a.score)
print(a.__slots__)

# @property change attribute to functions
class Student():
    @property
    def score(self): #get_score()
        print("get")
        return self._score
    @score.setter
    def score(self, score): #set_score()
        if not isinstance(score, int):
            raise ValueError("score must be an int")
        if score < 0 or score > 100:
            raise ValueError("score must be in (0,100)")
        print("set")
        self._score = score
s = Student()
#s.score = 99
s._score = 98
print(s._score)
print(type(s.score))
#print(s.score) # you can only to use s.score/get, s.score = /set, you can't use s.score()/s.score(value)
#print(help(property))
#print(dir(s))
#s.score("walter")
#s.score = "walter"
#s.score = 99
'''
