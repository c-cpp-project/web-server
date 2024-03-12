#!/usr/bin/env python

import cgi, sys, os

from datetime import datetime
from datetime import timezone, timedelta

# value = 1
# while True:
#     if value == 1:
#         value = 2
#     elif value == 2:
#         value = 1


# 현재 시간을 가져옵니다.
current_time = datetime.now(timezone.ut)

# 날짜 포맷을 지정합니다.
date_string = current_time.strftime("%a, %d %b %Y %H:%M:%S GMT")
path_info = os.environ.get("PATH_TRANSLATED", "No PATH_INFO")
query_string = os.environ.get("QUERY_STRING", "No QUERY_STRING")

# print(query_string, path_info)
headers = {}
try:
    if path_info != "No PATH_INFO" and query_string != "No QUERY_STRING":
        os.makedirs(os.path.dirname(path_info), exist_ok=True)
        file_path = path_info
        with open(file_path, 'w') as file:
            file.write(query_string)
        file_size = os.path.getsize(file_path)
except Exception as e:
    print(e)

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
print("{0} {1} OK\r\n".format("Status:", 200), end='')
for key, value in headers.items():
    if key == "CONTENT_TYPE":
        value = "text/plain"
    if key in ["HOST", "CONTENT_LENGTH", "CONTENT_TYPE", "DATE"]:
        print("{0}: {1}\r\n".format(key.replace('_', '-'), value.replace('_', '-')), end='')
print("\r\n", end='')
sys.stdout.flush()

try:
    file_path = path_info
    with open(file_path, 'r') as file:
        for line in file:
            print(line, end='')
except IOError as e:
    print(e)