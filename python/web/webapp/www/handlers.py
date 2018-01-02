#!/usr/bin/env python3.5
from aiohttp import web
from coroweb import get
'''
@get('/')
def index(request):
    return web.Response(body=b'<h1>Hello world</h1>', content_type='text/html')
'''

@get('/')
def index(request):
    r = 'Hello world'
    return r


