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

l = [i*i for i in range(1,100) if i % 2 == 0]
print(l)
