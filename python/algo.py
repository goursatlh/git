#!/usr/bin/env python3
import random
import datetime
print("please input the number: ")
num=int(input())
L=[]
while num > 0:
    L.append(random.randint(1, 1000000))
    num -= 1
#print("origin nums: ")
#print(L)

#public apis begin
def exchange(l, i, j):
    tmp = l[i]
    l[i] = l[j]
    l[j] = tmp
    return
#public apis end

Ls = L.copy();
Ls.sort() #use build-in sort function to verity the sorted result is right

def bubble(l):
    lens = len(l)
    for i in range(0, lens-1):
        for j in range(0, lens-1-i):
            if l[j] > l[j+1]:
                tmp = l[j]
                l[j] = l[j+1]
                l[j+1] = tmp
    return l

# bubble sort
Lb = L.copy()
begin = datetime.datetime.now()
bubble(Lb)
end = datetime.datetime.now()
if  Lb != Ls:
    print("sort failed.")
else:
    print("bubble sort time spend: ", end-begin)
#print("sorted nums: ")
#print(L)

# quick sort
def partition(l, left, right):
    index = (right - left+1)//2
    restore = left
    exchange(l, left+index, right)
    for i in range(left, right):
        if l[i] < l[right]:
            if i != restore:
                exchange(l, i, restore)
            restore += 1
    exchange(l, restore, right) 
    return restore

def quick(l, left, right):
    if left < right:
        index = partition(l, left, right)
        quick(l, left, index-1)
        quick(l, index+1, right)
    return

Lq = L.copy()
begin = datetime.datetime.now()
quick(Lq, 0, len(Lq)-1)
end = datetime.datetime.now()
if  Lq != Ls:
    print("quick sort failed.")
else:
    print(" quick sort time spend: ", end-begin)
