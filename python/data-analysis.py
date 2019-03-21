#!/usr/bin/env python3

import pandas as pd
from pandas import Series
from pandas import DataFrame

format_begin = "\033[1;32;43m"
format_begin_2 = "\033[1;39m"
format_begin_3 = "\033[5;32m"
format_end  = "\033[0m"
def printx(para):
    print(format_begin, para, format_end, sep='') # sepatator: sep="", end=""
def printy(para):
    print(format_begin_2, para, format_end, sep='') # sepatator: sep="", end=""
def print_flash(para):
    print(format_begin_3, para, format_end, sep='') # sepatator: sep="", end=""
'''
# Series
obj = Series([1,2,"walter"])
print(obj)
print("index: ", obj.index)
print("values: ", obj.values)

obj2 = Series([99,2,-1], index = ['b', 'a', 'd'])
print(obj2)

printx("DataFrame:                                                                                   ")
# DataFrame
data = {'state': ["Ohio", "Ohio", "Nevada"],
        'year': [2001, 1998, 2019],
        'pop': [1.5, 0.9, 2.2]}
print(type(data))
print(data)

df = DataFrame(data)
print(type(data))
print(df)
print(df.columns)
print(df["state"])
print(df.state)

printy("row: ")
print(df.loc[0])

print("add a new column for all index: ")
df["debt"] = 12.0
print(df)

printy("add a new column for specific index: ")
price = Series([9000], index = [1])
df["price"] = price
print(df)

printy("add a new column by math")
df["eastern"] = df["state"] == "Ohio"
print(df)

printy("del a column")
del df["eastern"]
print(df)
print()
'''

printx("Request:                                                                                   ")

import requests
url = 'https://api.github.com/repos/pandas-dev/pandas/issues'
reps = requests.get(url)
print(reps)
data = reps.json()
print(data[0]['number'])
print(data[0]['title'])
print(data[0]['created_at'])

issues = DataFrame(data, columns = ['number', 'title'])
print(issues)





