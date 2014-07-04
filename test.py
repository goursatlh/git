#!/usr/bin/env python3
print("hello world.","fuck you then")
name = input("please input your name: ")
print("hello", name)

#enumerate()
import string
s = string.ascii_lowercase
e = enumerate(s)
print(s)
print(list(e))

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















 
