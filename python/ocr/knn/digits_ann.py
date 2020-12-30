import cv2
import pickle
import numpy as np
import gzip
 
"""OpenCV ANN Handwritten digit recognition example
 
Wraps OpenCV's own ANN by automating the loading of data and supplying default paramters,
such as 20 hidden layers, 10000 samples and 1 training epoch.
 
The load data code is taken from http://neuralnetworksanddeeplearning.com/chap1.html
by Michael Nielsen
"""
 
def vectorized_result(j):
  e = np.zeros((10, 1))
  e[j] = 1.0
  return e
 
def load_data():
    with gzip.open('./mnist.pkl.gz') as fp:
        training_data, valid_data, test_data = pickle.load(fp,encoding='bytes')
        fp.close()
    return (training_data, valid_data, test_data)
 
def wrap_data():
    tr_d, va_d, te_d = load_data()
     
    training_inputs = [np.reshape(x, (784, 1)) for x in tr_d[0]]
    training_results = [vectorized_result(y) for y in tr_d[1]]
    training_data = list(zip(training_inputs, training_results))
     
    validation_inputs = [np.reshape(x, (784, 1)) for x in va_d[0]]
    validation_data = list(zip(validation_inputs, va_d[1]))
     
    test_inputs = [np.reshape(x, (784, 1)) for x in te_d[0]]
    test_data = list(zip(test_inputs, te_d[1]))
    return (training_data, validation_data, test_data)
 
 
def create_ANN(hidden = 20):
  ann = cv2.ml.ANN_MLP_create()
  ann.setTrainMethod(cv2.ml.ANN_MLP_RPROP | cv2.ml.ANN_MLP_UPDATE_WEIGHTS)
  ann.setActivationFunction(cv2.ml.ANN_MLP_SIGMOID_SYM)
  ann.setLayerSizes(np.array([784, hidden, 10]))
  ann.setTermCriteria(( cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 100, 0.1 ))
  return ann
 
def train(ann, samples = 10000, epochs = 1):
  tr, val, test = wrap_data()
   
  for x in range(epochs):
    counter = 0
    for img in tr:
      if (counter > samples):
        break
      if (counter % 1000 == 0):
        print ("Epoch %d: Trained %d/%d" % (x, counter, samples))
      counter += 1
      data, digit = img
      ann.train(np.array([data.ravel()], dtype=np.float32), cv2.ml.ROW_SAMPLE, np.array([digit.ravel()], dtype=np.float32))
    print ("Epoch %d complete" % x)
  return ann, test
 
def predict(ann, sample):
  resized = sample.copy()
  rows, cols = resized.shape
  if rows != 28 and cols != 28 and rows * cols > 0:
      resized = cv2.resize(resized, (28, 28), interpolation=cv2.INTER_CUBIC)
  return ann.predict(np.array([resized.ravel()], dtype=np.float32))


