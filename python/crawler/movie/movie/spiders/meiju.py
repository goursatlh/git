# -*- coding: utf-8 -*-
import scrapy


class MeijuSpider(scrapy.Spider):
    name = 'meiju'
    allowed_domains = ['meiju.com']
    start_urls = ['http://www.meijutt.com/new100.html']

    def parse(self, response):
        down_urls = response.css('ul li input[class="down_url"]::attr(value)').getall()
        if down_urls is not None:
            for down_url in down_urls:
                print(down_url)

        sub_urls = response.css('ul li h5 a::attr(href)').getall()
        if sub_urls is not None: 
            for url in sub_urls:
                url = response.urljoin(url)
                print(url)
                yield scrapy.Requset(url, callback=parse)
