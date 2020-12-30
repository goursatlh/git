#!/usr/bin/env python3
import cv2
import numpy as np
import sys

if len(sys.argv) != 2:
    print("Bad input: usage: ./orctest.py pic")
    exit(0)
pic_str = sys.argv[1]
print("process picture: ", pic_str)

src = cv2.imread(pic_str) 
#cv2.namedWindow("wave", cv2.WINDOW_AUTOSIZE)
#cv2.namedWindow("wave", cv2.WINDOW_NORMAL)
cv2.namedWindow("wave_hsv", cv2.WINDOW_NORMAL)
#cv2.namedWindow("wave_blue", cv2.WINDOW_NORMAL)
#cv2.namedWindow("wave_erode", cv2.WINDOW_NORMAL)
#cv2.namedWindow("wave_contours", cv2.WINDOW_NORMAL)
#cv2.namedWindow("wave_last", cv2.WINDOW_NORMAL)
cv2.namedWindow("result", cv2.WINDOW_NORMAL)
#cv2.imshow("wave", src)

# rgb -> hsv
hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)

def getpos(event,x,y,flags,param):
    if event==cv2.EVENT_LBUTTONDOWN:
        print(hsv[y,x])
cv2.imshow("wave_hsv", hsv)
cv2.setMouseCallback("wave_hsv", getpos)

# get bule area
low_hsv = np.array([100,220,220])
high_hsv = np.array([124,255,255])
mask = cv2.inRange(hsv, lowerb=low_hsv, upperb=high_hsv)
#cv2.imshow("wave_blue", mask)

# erode and dilate
kernel = np.ones((3,3), np.uint8)
erosion = cv2.erode(mask, kernel, iterations=3)
#cv2.imshow("wave_erode", erosion)

'''
mask = np.zeros([4000,4000], dtype=np.uint8)
#cv2.rectangle(mask, (mask[1151,1271], mask[3287,1475]), (255,255,255), -1)
cv2.rectangle(erosion, (1151,1271), (3287,1475), (0,255,0), 10)
#get_img = cv2.add(erosion, np.zeros(np.shape(erosion), dtype=np.uint8), mask=mask)
cv2.imshow("wave_last", erosion)
'''

#result = erosion[1151:3287, 1271:1475]
result = mask[1271:1475, 1151:3287]
cv2.imshow("result", result)

cv2.imwrite('result.png', result)
'''
ret, thresh = cv2.threshold(mask, 127,255,0)
#image,contours,hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
#contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 
contours, hierarchy = cv2.findContours(thresh,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE) 
cv2.drawContours(erosion, contours, 3, (0,255,0), 3)
cv2.imshow("wave_contours", erosion)
'''

# kick esc to exit
if cv2.waitKey(0)&0xFF:
    cv2.destroyAllWindows()

