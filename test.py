#!/usr/bin/env python3
'''
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
from collections import Iterable
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

#generator
L = [x * x for x in range(1, 10)]
print(L)

g = (x * x for x in range(1, 10))
print(g)
for i in range(1, 10):
    print(next(g))
print(g)

#g = (x * x for x in range(1, 10))
for i in g:
    print(i)

def fib(max):
    n,a,b = 0,0,1
    while n < max:
        yield b
        a, b=b, a+b
        n = n + 1
    return "done"
fib(20)

def odd():
    print('step 1')
    yield 1
    print('step 2')
    yield(3)
    print('step 3')
    yield(5)
o = odd()
for i in range(1,4):
    print(next(o))


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

def funcx(*n):
    sum = 0
    for i in n:
        sum = sum + i*i
    return sum
print(funcx(*list(range(10))))
print(funcx())

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
