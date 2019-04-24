# -*- coding: utf-8 -*-
import scrapy
from xiaohua.items import XiaohuaItem

class XhSpider(scrapy.Spider):
    name = 'xh'
    allowed_domains = ['xiaohuar.com']
    start_urls = ['http://www.xiaohuar.com/list-1-0.html']

    def parse(self, response):
        pages = response.xpath('//div/div[@id="page"]/a[@href]/@href')
        for url in response.xpath('//div[@class="item masonry_brick"]/div/div/a/img'):
            item = XiaohuaItem()
            item['name'] = url.xpath('.//@alt').get()
            addr = url.xpath('.//@src').get()
            if addr.find('www') == -1 and addr.find('http') == -1:
                addr = response.urljoin(addr)
            item['addr'] = addr
            print(item['name'], item['addr'])
