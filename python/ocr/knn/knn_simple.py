#!/usr/bin/env python3
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

trainData = np.random.randint(0,100,(25,2)).astype(np.float32)

print(trainData)

responses = np.random.randint(0,2,(25,1)).astype(np.float32)
print(responses)

red = trainData[responses.ravel()==0]
plt.scatter(red[:,0], red[:,1], 80, 'r', '^')

blue = trainData[responses.ravel()==1]
plt.scatter(blue[:,0], blue[:,1], 80, 'b', 's')

newcomer = np.random.randint(0,100,(1,2)).astype(np.float32)
plt.scatter(newcomer[:,0], newcomer[:,1], 80, 'g', 'o')

knn = cv.ml.KNearest_create()
knn.train(trainData, cv.ml.ROW_SAMPLE, responses)
ret, results, neighbours, dist = knn.findNearest(newcomer, 3)

print("result: {}\n".format(results))
print("neighbours: {}\n".format(neighbours))
print("result: {}\n".format(dist))

plt.show()


