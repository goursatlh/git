#!/usr/bin/env python3

import cv2
import sys

if len(sys.argv) != 2:
    print("Bad input: usage: ./orctest.py pic")
    exit(0)
pic_str = sys.argv[1]
print("orc pic: ", pic_str)

img = cv2.imread(pic_str, 0)
cv2.namedWindow('wave', cv2.WINDOW_NORMAL)
cv2.imshow('wave', img)
cv2.waitKey(0)
print(type(img))
print(dir(img))
print(img.shape)
print(img.size)
px = img[100,90]
print(px)
