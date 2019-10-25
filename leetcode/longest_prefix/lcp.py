#!/usr/bin/env python3

def is_at(prefix, strs):
    for i in range(len(strs)):
        if prefix != strs[i][0:len(prefix)]:
            return 0
    return 1

def shortest_str(strs):
    short = 0
    index = 0
    for i in range(len(strs)):
        len1 = len(strs[i])
        if len1 < short:
            short = len1
            index = i
    return index

class Solution:
        def longestCommonPrefix(self, strs: [str]) -> str:
            if len(strs) == 0 :
                return ""
            max_len = 0
            index = shortest_str(strs)
            for i in range(len(strs[index])):
                flag = is_at(strs[0][0:i+1], strs)
                if flag and (i+1)>max_len:
                    max_len = i+1

            return strs[0][0:max_len]


if __name__ == "__main__":
    s = ["abc", "ab", "abdf"]
    lcp1 = Solution()
    print(lcp1.longestCommonPrefix(s))
