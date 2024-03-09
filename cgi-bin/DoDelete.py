#!/usr/bin/env python

import sys
import os, shutil
from datetime import datetime
from datetime import timezone, timedelta

# 현재 시간을 가져옵니다.
current_time = datetime.now(timezone.utc)

# 날짜 포맷을 지정합니다.
date_string = current_time.strftime("%a, %d %b %Y %H:%M:%S GMT")

def removeFile(path):
    try:
        if os.path.isfile(path):
            os.remove(path)
        if os.path.isdir(path):
            shutil.rmtree(path)
        return "ok"
    except OSError as e:
        return "no"


# file_path = sys.stdin.read()
headers = {}
path_info = os.environ["PATH_TRANSLATED"]
for key, value in os.environ.items():
    if value == "":
        continue
    if key.startswith("HTTP_"):
        key_without_http = key[5:]  # "HTTP_" 부분을 제외한 나머지 부분
        headers[key_without_http] = value
    else:
        headers[key]  = value

data = removeFile(path_info)

headers["CONTENT_LENGTH"] = str(len(data))
headers["DATE"] = date_string
print("{0} {1} OK\r\n".format("Status:", 200), end='')
for key, value in headers.items():
    if key in ["HOST", "CONTENT_LENGTH", "CONTENT_TYPE", "DATE"]:
        print("{0}: {1}\r\n".format(key.replace('_', '-'), value.replace('_', '-')), end='')
print("\r\n", end='')
print(data, end='')