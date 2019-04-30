# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://doc.scrapy.org/en/latest/topics/items.html

import scrapy

class WeiboItem(scrapy.Item):
    # define the fields for your item here like:
    wb_name = scrapy.Field()
    pic_urls = scrapy.Field()
    video_urls = scrapy.Field()
    video_name = scrapy.Field()
    pic_paths = scrapy.Field()
    video_paths = scrapy.Field()
    follower  = scrapy.Field()
