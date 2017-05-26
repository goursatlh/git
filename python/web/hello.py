#!/usr/bin/env python3
def application(environ, start_response):
    print(environ)
    start_response('200 OK', [('Content-Type', 'text/html')])
    return [b'<h1>Hello, Web</h1>'] 
