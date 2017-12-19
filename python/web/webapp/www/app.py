#!/usr/bin/env python3.5
import logging; logging.basicConfig(level=logging.INFO)
import asyncio,os,json,time
from aiohttp import web
from datetime import datetime

def index(request):
    return web.Response(body=b'<h1>Hello world</h1>', content_type='text/html')

@asyncio.coroutine
def init(loop):
    app = web.Application(loop=loop)
    app.router.add_route('GET', '/', index)
    srv = yield from loop.create_server(app.make_handler(), '10.88.39.145', 9000)
    logging.info('Server started at http://10.88.39.145:9000...')
    return srv

loop = asyncio.get_event_loop()
loop.run_until_complete(init(loop))
loop.run_forever()

