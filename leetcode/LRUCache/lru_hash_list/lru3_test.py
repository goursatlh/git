#!/usr/bin/env python3
from ctypes import *
import os
import re

lib = cdll.LoadLibrary(os.getcwd() + '/lru3.so')
f1 = open("dict1.txt", "r")
f2 = open("dict2.txt", "r")
#input = f.read().split(',')
#input2 = f2.read().split(',')

str1 = f1.read()
str2 = f2.read()

para1 = eval(str1)
para2 = eval(str2)
pLRUCache = None 
for i in range(len(para1)):
    print(para1[i], para2[i])
    if para1[i] == "LRUCache":
        pLRUCache = lib.lRUCacheCreate(para2[i][0])
    elif para1[i] == "put":
        lib.lRUCachePut(pLRUCache, para2[i][0], para2[i][1])
    else:
        lib.lRUCacheGet(pLRUCache, para2[i][0])


 


'''
input = f.read()
p= re.compile('.+?"(.+?)"')
iter = p.findall(input)
for var in iter:
    print(var)

input2 = f2.read()
#p2= re.compile('\[([^]]*)\]')
iter2 = p2.findall(input2)
dictx = eval(input2)
print(type(dictx))
print(type(dictx[1]))
'''
#pLRUCache = lib.lRUCacheCreate(input2[0][0])
#lib.lRUCachePut(pLRUCache, 1, 1)
#lib.lRUCachePut(pLRUCache, 2, 2)
#lib.lRUCacheShow(pLRUCache)

