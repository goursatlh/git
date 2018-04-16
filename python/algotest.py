#!/usr/bin/env python3.5
import random
import datetime

def data_input(n):
    list = []
    for i in range(n):
        list.append(random.randint(1,100))
    print("list add: 0x%x" % id(list))
    return list #can we return stack variable ? 

def if_equire(l1, l2):
    if l1 is l2:
        return true
    if len(l1) != len(l2):
        return false
    else:
        for i in range(len(l1)):
            if l1[i] != l2[i]:
                return false
    return true

def bubble_sort(l):
    for i in range(len(l)-1):
        for j in range(len(l)-i-1):
            if l[j+1] < l[j]:
                l[j+1], l[j] = l[j], l[j+1]
    return 


def quick_sort(l, left, right):
    def partation(left, right):
        index = (right-left+1)//2
        restore = left
        l[left+index], l[right] = l[right],l[left+index]
        for i in range(left, right):
            if l[i] < l[right]:
                if i != restore:
                    l[i], l[restore] = l[restore], l[i]
                restore=restore+1
        l[restore], l[right] = l[right], l[restore]
        return restore
    if left < right:
        index = partation(left, right)
        quick_sort(l, left, index-1)
        quick_sort(l, index+1, right)
    return

print("please input the number you want to sort: ")
n = int(input())
l = data_input(n)# copy assign
print("l addr: 0x%x" % id(l))

l1 = list(l)
start = datetime.datetime.now()
l1.sort()
end = datetime.datetime.now()
print("built-in sort costs ", end-start)
if len(l1) < 20:
    print(l1)

l2 = list(l)
start = datetime.datetime.now()
bubble_sort(l2)
end = datetime.datetime.now()
print("bubble sort costs ", end-start)
if l2 != l1:
    print("bad sort result")
if len(l2) < 20:
    print(l2)

l3 = list(l)
if len(l3) < 20:
    print(l3)
start = datetime.datetime.now()
quick_sort(l3, 0, len(l3)-1)
end = datetime.datetime.now()
print("quick sort costs ", end-start)
if l3 != l1:
    print("bad sort result")
if len(l3) < 20:
    print(l3)
