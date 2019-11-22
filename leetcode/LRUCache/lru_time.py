#!/usr/bin/env python3
import time
import operator

class LRUCache:
    def __init__(self, capacity: int):
        # dict1 for key:value
        self.lru = dict()
        self.lru_len = 0
        self.lru_max_len = capacity

        # dict2 for key:use_time
        self.lru_time = dict()

    def get(self, key: int) -> int:
        if self.lru.__contains__(key):
            self.lru_time[key] = time.clock()
            return self.lru[key]
        else:
            return -1

    def put(self, key: int, value: int) -> None:
        if self.lru.__contains__(key):
            self.lru[key] = value
            self.lru_time[key] = time.clock()
        else:
            if self.lru_len+1 <= self.lru_max_len:
                self.lru[key] = value
                self.lru_len += 1
                self.lru_time[key] = time.clock()
            else:
                # del the lru element in dict1 and dict2
                sorted_lru_time = sorted(self.lru_time.items(), key=operator.itemgetter(1))
                key_del = sorted_lru_time[0][0]
                self.lru.pop(key_del)
                self.lru_time.pop(key_del)
                # add the new key:vaule
                self.lru[key] = value
                self.lru_len += 1
                self.lru_time[key] = time.clock()

        return

    def show(self):
        print(self.lru)
        print(self.lru_time)

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


