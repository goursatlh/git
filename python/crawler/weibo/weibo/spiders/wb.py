# -*- coding: utf-8 -*-
import scrapy
import json
from scrapy.selector import Selector
import re
import textwrap
from weibo.items import WeiboItem
from datetime import datetime

format_begin = "\033[1;32;43m"
format_begin_2 = "\033[1;39m"
format_begin_3 = "\033[5;32m"
format_end  = "\033[0m"

def printx(para):
    print(format_begin, para, format_end, sep='') # sepatator: sep="", end=""
def printy(*para):
    print(format_begin_2, para, format_end, sep='') # sepatator: sep="", end=""
def print_flash(*para):
    print(format_begin_3, para, format_end, sep='') # sepatator: sep="", end=""

def print_nohtml(text_str):
    s2 = re.sub(r'<br />', '\n', text_str)
    s2 = re.sub(r'<.*?>', '', s2)
    s2 = s2.replace(' ','')
    s2 = textwrap.fill(s2, width=70)
    print(s2)

def time_format(time_str):
    GMT_FORMAT = '%a %b %d %H:%M:%S +0800 %Y'
    ST_FORMAT = '%Y-%m-%d|%H:%M:%S'
    strtmp = datetime.strptime(time_str, GMT_FORMAT)
    return datetime.strftime(strtmp, ST_FORMAT)

class WbSpider(scrapy.Spider):
    name = 'wb'
    page_index = 1
    allowed_domains = ['m.weibo.cn']
    start_urls = [#'https://m.weibo.cn/api/container/getIndex?jumpfrom=wapv4&tip=1&type=uid&value=1549255637&containerid=1076031549255637', # dashijie
                  #'https://m.weibo.cn/api/container/getIndex?jumpfrom=wapv4&tip=1&type=uid&value=5829543885&containerid=1076035829543885', # chenyuqi
                  #'https://m.weibo.cn/api/container/getIndex?jumpfrom=wapv4&tip=1&type=uid&value=2219969573&containerid=1076032219969573', # zhuzhu
                  #'https://m.weibo.cn/api/container/getIndex?type=uid&value=1792673805&containerid=1076031792673805' #caoxiyue
                  #'https://m.weibo.cn/profile/info?uid=1792673805',
                  #'https://m.weibo.cn/profile/info?uid=5829543885',
                  #'https://m.weibo.cn/profile/info?uid=2219969573',
                  #'https://m.weibo.cn/profile/info?uid=1549255637'
                  #'https://m.weibo.cn/api/container/getIndex?uid=1549255637&containerid=1076031549255637&page=1',
                  #'https://m.weibo.cn/api/container/getIndex?uid=5829543885&containerid=1076035829543885&page=1',
                  #'https://m.weibo.cn/api/container/getIndex?uid=2219969573&containerid=1076032219969573&page=1',
                  #'https://m.weibo.cn/api/container/getIndex?uid=1792673805&containerid=1076031792673805&page=1',
                  'https://m.weibo.cn/api/container/getIndex?uid=6049590367&containerid=1076036049590367&page=1'
                 ]

    def start_requests(self):
        uid = getattr(self, 'uid', None)
        if uid is not None:
            url = 'https://m.weibo.cn/api/container/getIndex?uid='+uid+'&containerid=107603'+uid+'&page=1'
            yield scrapy.Request(url, self.parse, meta={'start_url':url})
        else:
            for url in self.start_urls:
                yield scrapy.Request(url, self.parse, meta={'start_url':url})

    def parse(self, response):
        current_url = response.meta['start_url']
        print("repsonse from ", current_url)

        sites = json.loads(response.body_as_unicode())
        data = sites['data']
        latest_index = 0
        latest_id = 0

        # get the display mode
        mode = getattr(self, 'mode', None)

        if 'longTextContent' in data:
            print('------------------------------------------------------------------------------------')
            print(response.meta['screen_name'])
            print(response.meta['created_at'])
            #print("Full context: ")
            print_nohtml(data['longTextContent'])
        else:
            #user_info = data['user']
            #print()
            #printx(user_info['screen_name'])
            #print("Followers: ", user_info['followers_count'])
            cards = data['cards']
            num = len(cards)
            for i in range(num):
                if cards[i]['card_type'] == 9:
                    card = cards[i]['mblog']

                    if mode == "latest":
                        if "isTop" in card:
                            latest_idnex = i
                            latest_id = int(card['id'])
                            continue
                        else:
                            if int(card['id']) > latest_id:
                                latest_index = i
                                latest_id = int(card['id'])
                        if i == num-1:
                            card = cards[latest_index]['mblog']
                            print(card['user']['screen_name'])
                            print("Followers: ", card['user']['followers_count'])
                            time_created = card['created_at']
                            if card['source'] != '':
                                print(time_created, " from ", card['source'])
                            else:
                                print(time_created)
                            text_str = card['text']
                            print_nohtml(text_str)
                            return
                        else:
                            continue

                    print('------------------------------------------------------------------------------------')
                    user_info = card['user']['screen_name']
                    time_created = card['created_at']
                    #time_created = time_format(card[i]['created_at'])
                    if card['source'] != '':
                        #print(time_created, " from ", card['source'])
                        other_info = time_created + ' from ' + card['source']
                    else:
                        other_info = time_created
                    # process the text: delete the html elements
                    text_str = card['text']

                    # process the long text.
                    if card['isLongText'] == 1:
                        seletext = Selector(text=text_str)
                        longtext_url = seletext.xpath('//a[text()="\u5168\u6587"]/@href').get()
                        print("long text url: ", longtext_url)
                        if longtext_url is not None:
                            id = longtext_url.split('/')[2]
                            lurl = "https://m.weibo.cn/statuses/extend?id="+id
                            print("Will display the full tent later..., url: ", lurl)
                            yield response.follow(lurl, self.parse, 
                                  meta={'screen_name': user_info, 'start_url': lurl, 'created_at': other_info}, 
                                  dont_filter=True)
                    else:
                        print(user_info)
                        print(other_info)
                        print_nohtml(text_str)

                    # init the item to process the picture or video url
                    item = WeiboItem()
                    item['wb_name'] = card['user']['screen_name']+'-'+time_created
                    item['pic_urls'] = []
                    item['video_urls'] = []

                    # process pictures
                    ispic = getattr(self, 'pic', None)
                    if ispic == '1':
                        if 'pics' in card:
                            pics = card['pics']
                            pic_num = len(pics)
                            for j in range(pic_num):
                                item['pic_urls'].append(pics[j]['large']['url'])
                            yield item

                    # process videos
                    isvideo = getattr(self, 'video', None)
                    if isvideo == '1':
                        if 'page_info' in card:
                            if 'media_info' in card['page_info']:
                                media_info = card['page_info']['media_info']
                                if len(media_info['stream_url_hd']) != 0:
                                    video_url = media_info['stream_url_hd']
                                elif len(media_info['stream_url']) != 0:
                                    video_url = media_info['stream_url']
                                item['video_urls'].append(video_url)
                                item['video_name'] = card['page_info']['title']
                                yield item

                    if mode == 'all':
                        if i == num-1:
                            current_page = int(current_url.split('=')[-1])
                            max_page = getattr(self, 'page', None)
                            if max_page is not None:
                                if current_page == int(max_page):
                                    return
                            pos = current_url.find('page')
                            nextpage_url = current_url[:(pos+5)]+str(current_page+1)
                            #print('send next page url ', nextpage_url)
                            yield response.follow(nextpage_url, self.parse, dont_filter=True, meta={'start_url':nextpage_url})

'''
            print()
            card = data['cards']
            num = len(card)
            for i in range(num):
                if card[i]['card_type'] == 9:
                    if i == 0:
                        printy('This is your idol: ', card[i]['mblog']['user']['screen_name'])
                    print('------------------------------------------------------------------------------------------------------------------------------------------')
                    print(card[i]['mblog']['user']['screen_name'], card[i]['mblog']['created_at'])

                    # process the text: delete the html elements
                    text_str = card[i]['mblog']['text']
                    #s2 = re.sub(r'<.*?>','', text_str)
                    #s2 = s2.replace(' ','')
                    print_nohtml(text_str)
                    #print(text_str)

                    # process the long text, and there are some errors . comment it tentatively.
                    seletext = Selector(text=text_str)
                    if card[i]['mblog']['isLongText'] == 1:
                        #longtext_url = seletext.xpath('//a[text="全文"]/@href').get()
                        longtext_url = seletext.xpath('//a[text()="\u5168\u6587"]/@href').get()
                        if longtext_url is not None:
                            id = longtext_url.split('/')[2]
                            lurl = "https://m.weibo.cn/statuses/extend?id="+id
                            print("Will display the full tent later..., url: ", lurl)
                            yield response.follow(lurl, self.parse, headers=header, dont_filter=True)
                    #print()
'''
