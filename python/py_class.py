#!/usr/bin/env python3
'''
class Dog:
    """ dog class """
    def __init__(self, name, age):
        self.name = name
        self.ag = age
    def sit(self):
        print("dog sits")
    
my_dog = Dog("sam", 2)
my_dog.sit()
'''

class Car:
    def __init__(self, name, model, age):
        self.name = name
        self.model = model
        self.age = age

    def run(self):
        pass

class ElectricCar(Car):
    def __init__(self, name, model, age, batterySize=100):
        #Car.__init__(self, name, model, age)
        super().__init__(name, model, age)
        self.batterySize = batterySize 
    pass

my_car = Car("Tesla", 1200, 3)
print(my_car.name)
my_electricCar = ElectricCar("Tesla", 1200, 3)
print(my_electricCar.name)
print(my_electricCar.batterySize)

my_electricCar1 = ElectricCar("Tesla", 1200, 3, 200)
print(my_electricCar1.name)
print(my_electricCar1.batterySize)
'''
class A:
    def __init__(self):
        print("A define")

class B(A):
    def __init__(self):
        A.__init__(self)
        print("B define")

class C(A):
    def __init__(self):
        A.__init__(self)
        print("C define")

class D(B, C):
    def __init__(self):
        B.__init__(self)
        C.__init__(self)
        print("D define")

d = D()
'''



'''
class A:
    def __init__(self):
        print("A define")

class B(A):
    def __init__(self):
        super().__init__()
        print("B define")

class C(A):
    def __init__(self):
        super().__init__()
        print("C define")

class D(B, C):
    def __init__(self):
        super().__init__()
        print("D define")

d = D()
print(D.mro())
'''


