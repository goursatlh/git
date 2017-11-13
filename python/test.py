#!/usr/bin/env python3
# -*- coding: utf-8 -*-  
'''
def consumer():
    r = ''
    while True:
        n = yield r
        if not n:
            return
        print('[CONSUMER] Consuming %s...' % n)
        r = '200 OK'

def produce(c):
    c.send(None)
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
print("usage for asyncio")
import asyncio

@asyncio.coroutine
def hello():
    print("Hello world!")
    r = yield from asyncio.sleep(10)
    print("Hello again!")

loop = asyncio.get_event_loop()
loop.run_until_complete(hello())
print("close")
loop.close()
'''

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
    nonlocal saw
    saw = "world"
    print("testxx saw ", saw)
testxx()

def test():
    def do_nonlocal():
        spam = "fuck"
        nonlocal spam
        def do_nonlocal2():
            nonlocal spam
            spam = "nonlocal2 spam"
        do_nonlocal2()
    spam = "test spam"
    do_nonlocal()
    print("after nonlocal2 asssignment:", spam)
test()

#strip(),rstrip(),lstrip()
str = " hello  \n";
print(str)
print(str.strip())
print(str.rstrip())
print(str.lstrip())
print("strip end")

# rex in python
import re
import os # execute shell command in .py

fs = os.popen("who", "r")
for eachline in fs:
    print(eachline.rstrip())
    print(re.split(r"\s\s+", eachline.rstrip()))
fs.close()

# file io
f = open("txt", "r+")         
print("read1: ", f.read().strip())
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

#map reduce 
def f(x):
    return x *x
r = map(f, [1, 2, 3, 4, 5, 6, 7, 8, 9])
print(list(r))
from functools import reduce
def add(x, y):
    return x + y
print(reduce(add, [1, 3, 5, 7, 9]))

r = map(str, [1, 2, 3, 4, 5])
print(list(r))
#str2int

#"123" => 123
print("str2int")
def f1(x):
    return {'0':0, '1':1, '2':2, '3':3}[x]
def f2(x1, x2):
    return x1 * 10 + x2
r1 = map(f1, "123")
r2 = reduce(f2, r1)
print(r2)
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

#class type
class Student(object):
    "decribe class Student"
    __x = 2
    def __init__(self, name, score):
        self.name = name
        self.__score = score 
    def printc(self):
        print(self.name, self.__score, self.__x)

r = Student("walter", 20)
print(dir(r))
r.printc()
#print(r.__score) #error, private element, can't be accessed outside of the class
#r.__x = 22 #equal to add a new public element for object r
#print(r.__x)
print(r._Student__score) #not recommend

#inherit
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

#type()/isinstance()/dir()
print("a is ", type(a))
print("c is ", type(a))
print("p is ", type(a))
print(dir(a))
print(a.__sizeof__())
print(dir("hello"))

#class attribute and instance attribute
class A():
    name = "this is A"
class B(A):
    pass
b = B()
b.name = "this is B" #instance attribute will override the class attribute which has the same name
print(B.name)
print(b.name)
c = B()
print(c.name)

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

#classmethod
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
print(ik2.get_no_of_instance())
print(ik2.get_no_of_instance.__name__)

def printx():
    print("hello world")
f = printx
f()
print(printx.__name__, f.__name__)
'''
