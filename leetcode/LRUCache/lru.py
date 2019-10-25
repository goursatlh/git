#!/usr/bin/env python3
import time

class LRUCache_note:
    def __init__(self, key, value):
        self.key = key
        self.value = value
        self.time = time.clock()

    def show(self):
        print(self.key, self.value, self.time)

class LRUCache:
    def __init__(self, capacity: int):
        self.lru = dict()
        self.lru_len = 0
        self.lru_max_len = capacity

        self.lru_time = dict()

    def get(self, key: int) -> int:
        if self.lru.__contains__(key):
            self.lru_time[key] = time.clock()
            return self.lru[key]
        else:
            return -1

    def put(self, key: int, value: int) -> None:
        if self.lru_len+1 <= self.lru_max_len:
            self.lru[key] = value
            self.lru_len += 1
        return

if __name__ == "__main__":
        obj = LRUCache(10)
        print(obj.get(2))
        obj.put(1, 'a')
        print(obj.get(1))

        obj_note = LRUCache_note(2, 'b')
        obj_note.show()
        obj_note2 = LRUCache_note(3, 'c')
        obj_note2.show()

