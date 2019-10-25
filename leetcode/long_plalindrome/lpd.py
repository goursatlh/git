#!/usr/bin/env python3

def ldp(s, left, right):
    l = left
    r = right
    while l>=0 and r<len(s) and s[l] == s[r] :
        l = l - 1
        r = r + 1
    print("ldp ", s, left, right, r-l-1)
    return r-l-1

class Solution:
        def longestPalindrome(self, s: str) -> str:
            if s == None:
                return None
            if len(s) == 0 or len(s) == 1:
                return s
            max_len = 0
            max_len2 = 0
            for i in range(len(s)-max_len//2):
                len1 = ldp(s, i, i)
                len2 = ldp(s, i, i+1)

                if len2 >= len1 :
                    max_len2 = len2
                    l = i - max_len2//2 +1
                    r = i + max_len2//2
                else:
                    max_len2 = len1
                    l = i - max_len2//2
                    r = i + max_len2//2
                if max_len2 > max_len:
                    max_len = max_len2
                    l_max = l
                    r_max = r

            return s[l_max:r_max+1]

if __name__ == "__main__":
    str1 = input("please input the string: ")
    longpd = Solution()
    str2 = longpd.longestPalindrome(str1)
    print(str2)
