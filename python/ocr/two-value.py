#!/usr/bin/env python3
from PIL import Image
import sys

if len(sys.argv) != 2:
    print("Bad input: usage: ./orctest.py pic")
    exit(0)
pic_str = sys.argv[1]
print("orc pic: ", pic_str)

def clear_pot(img_str):
    im = Image.open(img_str)
    data = im.getdata()
    w,h = im.size
    black_point = 0

    for x in range(1,w-1):
        for y in range(1,h-1):
            mid_pixel = data[w*y + x]
            if mid_pixel < 50:
                top_pixel = data[w*(y-1)+x]
                left_pixel = data[w*y+(x-1)]
                down_pixel = data[w*(y+1)+x]
                right_pixel = data[w*y+(x+1)]

                if top_pixel < 10:
                    black_point += 1
                if left_pixel < 10:
                    black_point += 1
                if down_pixel < 10:
                    black_point += 1
                if right_pixel < 10:
                    black_point += 1
                if black_point < 1:
                    im.putpixel((x,y),255)
                black_point = 0
    im.save('new'+img_str)

def two_value(img_str):
    image=Image.open(img_str)
    lim=image.convert('L')
    threshold=165
    table=[]
     
    for j in range(256):
      if j<threshold:
        table.append(0)
      else:
        table.append(1)
  
    bim=lim.point(table,'1')
    bim.save('new'+pic_str)

def black2white(img_str):
    image=Image.open(img_str)
    print(image.getcolors())
    rgb_start = image.getcolors()[0]
    rgb_end = image.getcolors()[-1]
    rgb_start_color,rgb_end_color = rgb_start[1],rgb_end[1]
    rgb_start_int,rgb_end_int = rgb_start[0],rgb_end[0]
    print(rgb_start,rgb_end)
    if rgb_start_int > rgb_end_int:
        if rgb_start_color == 0 or rgb_start_color == (0, 0, 0):
            print("black bg, white font")
    else:
        if rgb_start_color == 255 or rgb_start_color == (255, 255, 255):
            print("black bg, white font %s" % rgb_start_int)

#black2white(pic_str)



two_value(pic_str)
clear_pot('new'+pic_str)





