# -*- coding: utf-8 -*-
import scrapy
from video_download.items  import  VideoDownloadItem

class MeijuSpider(scrapy.Spider):
    name = 'meiju'
    allowed_domains = ['meijutt.com']
    start_urls = ['http://www.meijutt.com/new100.html']

    def parse(self, response):
        url_main = "www.meijutt.com"
        movies = response.xpath('//ul[@class="top-list  fn-clear"]/li')
        for each_movie in movies:
            item = VideoDownloadItem()
            item['name'] = each_movie.xpath('./h5/a/@title').extract()[0]
            item['url'] = url_main + each_movie.xpath('./h5/a/@href').extract()[0]
            yield item
