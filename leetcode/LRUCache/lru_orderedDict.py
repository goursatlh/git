#!/usr/bin/env python3
from collections import OrderedDict

class LRUCache(OrderedDict):
    def __init__(self, capacity: int):
        self.capacity = capacity

    def get(self, key: int) -> int:
        if self.__contains__(key):
            self.move_to_end(key)
            return self[key]
        else:
            return -1

    def put(self, key: int, value: int) -> None:
        if self.__contains__(key):
            self[key] = value
            self.move_to_end(key)
        else:
            if len(self) < self.capacity:
                self[key] = value
            else:
                self.popitem(last = False)
                self[key] = value

        return

    def show(self):
        print(self)

if __name__ == "__main__":
        obj = LRUCache(2)
        obj.put(2,1)
        obj.put(1,1)
        obj.put(2,3)
        obj.show()
        obj.put(4,1)
        obj.show()
        print(obj.get(1))
        print(obj.get(2))


