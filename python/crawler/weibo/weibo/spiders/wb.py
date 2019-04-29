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
    print()

def time_format(time_str):
    GMT_FORMAT = '%a %b %d %H:%M:%S +0800 %Y'
    ST_FORMAT = '%Y-%m-%d|%H:%M:%S'
    strtmp = datetime.strptime(time_str, GMT_FORMAT)
    return datetime.strftime(strtmp, ST_FORMAT)

class WbSpider(scrapy.Spider):
    name = 'wb'
    allowed_domains = ['m.weibo.cn']
    '''
    start_urls = [#'https://m.weibo.cn/api/container/getIndex?jumpfrom=wapv4&tip=1&type=uid&value=1549255637&containerid=1076031549255637', # dashijie
                  #'https://m.weibo.cn/api/container/getIndex?jumpfrom=wapv4&tip=1&type=uid&value=5829543885&containerid=1076035829543885', # chenyuqi
                  #'https://m.weibo.cn/api/container/getIndex?jumpfrom=wapv4&tip=1&type=uid&value=2219969573&containerid=1076032219969573', # zhuzhu
                  #'https://m.weibo.cn/api/container/getIndex?type=uid&value=1792673805&containerid=1076031792673805' #caoxiyue
                  'https://m.weibo.cn/profile/info?uid=1792673805',
                  'https://m.weibo.cn/profile/info?uid=5829543885',
                  'https://m.weibo.cn/profile/info?uid=2219969573',
                  'https://m.weibo.cn/profile/info?uid=1549255637'
                 ]
    '''
    start_urls = ['https://m.weibo.cn/profile/info?uid=2219969573']

    def parse(self, response):
        sites = json.loads(response.body_as_unicode())
        data = sites['data']
        user_agent = 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Mobile Safari/537.36'
        '''
        cookie = 'ALF=1558920577; SUBP=0033WrSXqPxfM725Ws9jqgMF55529P9D9W5YBsDvTBpnc.Wzy.URbl3d5JpX5K-hUgL.Fo2ceKBceo-fehe2dJLoI7LrdNHEMc8Xdsnt; MLOGIN=1; _T_WM=30689352240; SCF=Ao2y-itoCdlv6WiaobQC2b1eHkXH-4xrE5e-xvAIFRklozOvO6Mw5paYvg7svhBkCTYHhWk61SytRYZzYYx1RaU.; SUB=_2A25xx8rNDeRhGedI6lYX8ivJyz-IHXVTS9aFrDV6PUJbktAKLRnhkW1NV_5BqYzVOlkKWNcaJXFGho3EISrcT2Rp; SUHB=0U14es-oQF-YuM; SSOLoginState=1556331165; WEIBOCN_FROM=1110005030; XSRF-TOKEN=6b3239; M_WEIBOCN_PARAMS=oid%3D4362170832123773%26luicode%3D20000061%26lfid%3D4362170080831131%26uicode%3D20000061%26fid%3D4362170832123773'
        header = {
                    'User-Agent': user_agent,
                    'Cookie': cookie
             }
        '''
        # display in short mode
        mode = getattr(self, 'mode', None)
        if mode == 'latest':
            user_info = data['user']
            print(user_info['screen_name'])
            print("Followers: ", user_info['followers_count'])
            card = data['statuses']
            if 'isTop' in card[0]:
                if card[0]['isTop'] == 1 and card[1]['id'] > card[0]['id']:
                    index = 1
                else:
                    index = 0
            else:
                index = 0

            time_created = time_format(card[index]['created_at'])
            if card[index]['source'] != '':
                print(time_created, " from ", card[index]['source'])
            else:
                print(time_created)
            text_str = card[index]['text']
            print_nohtml(text_str)
            return

        if 'longTextContent' in data:
            print("******************************************************************************************************************************************")
            print("Full context: ")
            print_nohtml(data['longTextContent'])
        else:
            user_info = data['user']
            print()
            printx(user_info['screen_name'])
            print("Followers: ", user_info['followers_count'])
            card = data['statuses']
            num = len(card)
            for i in range(num):
                    item = WeiboItem()
                    print('------------------------------------------------------------------------------------------------------------------------------------------')
                    #print(card[i]['user']['screen_name'])
                    time_created = time_format(card[i]['created_at'])
                    if card[i]['source'] != '':
                        print(time_created, " from ", card[i]['source'])
                    else:
                        print(time_created)
                    # init the item
                    item['wb_name'] = user_info['screen_name']+'-'+time_created
                    item['pic_urls'] = []
                    item['video_urls'] = []
                    # process the text: delete the html elements
                    text_str = card[i]['text']
                    print_nohtml(text_str)

                    ispic = getattr(self, 'pic', None)
                    if ispic == '1':
                        # process pictures
                        if 'pics' in card[i]:
                            pics = card[i]['pics']
                            pic_num = len(pics)
                            for j in range(pic_num):
                                item['pic_urls'].append(pics[j]['large']['url']) 
                    isvideo = getattr(self, 'video', None)
                    if isvideo == '1':
                        # process videos
                        if 'page_info' in card[i]:
                            if 'media_info' in card[i]['page_info']:
                                media_info = card[i]['page_info']['media_info']
                                if len(media_info['stream_url_hd']) != 0:
                                    video_url = media_info['stream_url_hd']
                                elif len(media_info['stream_url']) != 0:
                                    video_url = media_info['stream_url']
                                item['video_urls'].append(video_url)
                    yield item
                    # process the long text, and there are some errors . comment it tentatively.
'''
                    seletext = Selector(text=text_str)
                    if card[i]['isLongText'] == 1:
                        #longtext_url = seletext.xpath('//a[text="全文"]/@href').get()
                        longtext_url = seletext.xpath('//a[text()="\u5168\u6587"]/@href').get()
                        if longtext_url is not None:
                            id = longtext_url.split('/')[2]
                            lurl = "https://m.weibo.cn/statuses/extend?id="+id
                            print("Will display the full tent later..., url: ", lurl)
                            #yield response.follow(lurl, self.parse, headers=header, dont_filter=True)
                            yield response.follow(lurl, self.parse, dont_filter=True)
'''
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
