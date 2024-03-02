#!/usr/bin/env python

import os
import sys
import mimetypes

path_info = os.environ.get("PATH_TRANSLATED", "")
length = os.environ.get("HTTP_CONTENT_LENGTH", "")
chunk_size = 1024 * 8

def fileUpload(path_info, length):
    data = b''
    while len(data) != length:
        chunk = sys.stdin.buffer.read(chunk_size)
        if chunk:
            data += chunk
    try:
        os.makedirs(os.path.dirname(path_info), exist_ok=True)
        file_path = path_info
        with open(file_path, 'wb') as file:
            file.write(data)
        file.close()
        file_size = os.path.getsize(file_path)
    except Exception as e:
        print(e)
        return (-1)
    return (file_size)


file_size = fileUpload(path_info, int(length))

headers = {}
if (file_size > 0):
    for key, value in os.environ.items():
        if key.startswith("HTTP_"):
            key_without_http = key[5:]  # "HTTP_" 부분을 제외한 나머지 부분
            headers[key_without_http] = value
        else:
            headers[key]  = value
    for key, value in headers.items():
        print("{0}: {1}".format(key, value))
        
    data = b''
    path = path_info
    mime_type, encoding = mimetypes.guess_type(path)
    try:
        with open(path, 'rb') as file:
            while True:
                chunk = file.read(chunk_size)
                if not chunk:
                    break
                data += chunk
        if mime_type and mime_type.startswith('text'):
            sys.stdout.write(data.decode('utf-8'))
        else:
            sys.stdout.buffer.write(data)
    except IOError as e:
        print(e)