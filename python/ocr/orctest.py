#!/usr/bin/env python3

from PIL import Image
import pytesseract
import sys

if len(sys.argv) != 2:
    print("Bad input: usage: ./orctest.py pic")
    exit(0)
pic_str = sys.argv[1]
print("orc pic: ", pic_str)

class Languages:
    CHS = 'chi_sim'
    ENG = 'eng'

def img_to_str(image_path, lang=Languages.ENG):
    return pytesseract.image_to_string(Image.open(image_path), lang)

#print(img_to_str('pic/lan.jpg', lang=Languages.ENG))
print(img_to_str(pic_str, lang=Languages.ENG))
#print(img_to_str('pic/wave1.jpg', lang=Languages.ENG))
#print(img_to_str('pic/wave2.jpg', lang=Languages.ENG))
#print(img_to_str('pic/wave3.png', lang=Languages.ENG))


