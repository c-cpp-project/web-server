#!/usr/bin/env python

import os
import sys
import mimetypes
from datetime import datetime
from datetime import timezone, timedelta

# 현재 시간을 가져옵니다.
current_time = datetime.now(timezone.utc)

# 날짜 포맷을 지정합니다.
date_string = current_time.strftime("%a, %d %b %Y %H:%M:%S GMT")

path_info = os.environ.get("PATH_TRANSLATED", "")
length = os.environ.get("CONTENT_LENGTH", "")
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
if file_size > 0:
    for key, value in os.environ.items():
        if value == "":
            continue
        if key.startswith("HTTP_"):
            key_without_http = key[5:]  # "HTTP_" 부분을 제외한 나머지 부분
            headers[key_without_http] = value
        else:
            headers[key]  = value
    headers["CONTENT_LENGTH"] = str(file_size)
    headers["DATE"] = date_string
    print("{0} {1} OK\r\n".format(headers["SERVER_PROTOCOL"], 200), end='')
    for key, value in headers.items():
        if key in ["HOST", "CONTENT_LENGTH", "CONTENT_TYPE", "DATE"]:
            print("{0}: {1}\r\n".format(key.replace('_', '-'), value.replace('_', '-')), end='')
    print("\r\n", end='')
        
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