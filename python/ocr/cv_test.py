#!/usr/bin/env python3

import cv2
import numpy as np
import sys

if len(sys.argv) != 2:
    print("Bad input: usage: ./orctest.py pic")
    exit(0)
pic_str = sys.argv[1]
print("orc pic: ", pic_str)

img = cv2.imread(pic_str, 0)
#cv2.namedWindow('wave', cv2.WINDOW_NORMAL)
#cv2.imshow('wave', img)
#cv2.waitKey(0)
#print(type(img))
#print(dir(img))
print(img.shape)
#print(img.size)
#print(img.dtype)
#px = img[100,90]
#print(px)

'''
edges = cv2.Canny(img, 30, 70)
cv2.imshow('canny', np.hstack((img, edges)))
cv2.waitKey(0)
'''

#ret, th = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY) #2-valule
#ret, th = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY_INV)
#cv2.imshow('thresh', th)
#cv2.waitKey(0)






