# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html

import scrapy
from scrapy.pipelines.images import ImagesPipeline
from scrapy.exceptions import DropItem

class WeiboPipeline(ImagesPipeline):
    def get_media_requests(self, item, info):
        if len(item['pic_urls']) != 0:
            i = 0
            for pic_url in item['pic_urls']:
                i = i+1
                yield scrapy.Request(pic_url, meta={'name': item['wb_name'], 'index': str(i)}, dont_filter=True)

    def item_completed(self, results, item, info):
        image_paths = [ x['path'] for ok, x in results if ok ]
        if not image_paths:
            raise DropItem("Item contains no images")
        item['pic_paths'] = image_paths
        return item

    def file_path(self, request, response=None, info=None):
        name = request.meta['name']
        index = request.meta['index']
        filename = name+'-'+index+'.jpg'
        return filename
