#!/usr/bin/env python

import cgi, sys, os

# value = 1
# while True:
#     if value == 1:
#         value = 0
#     else:
#         value = 1

path_info = os.environ.get("PATH_TRANSLATED", "No PATH_INFO")
query_string = os.environ.get("QUERY_STRING", "No QUERY_STRING")

# print(query_string, path_info)

try:
    if path_info != "No PATH_INFO" and query_string != "No QUERY_STRING":
        os.makedirs(os.path.dirname(path_info), exist_ok=True)
        file_path = path_info
        with open(file_path, 'w') as file:
            file.write(query_string)
except Exception as e:
    print(e)

for key, value in os.environ.items():
    if key.startswith("HTTP_"):
        key_without_http = key[5:]  # "HTTP_" 부분을 제외한 나머지 부분
        headers[key_without_http] = value
    else:
        headers[key]  = value
for key, value in headers.items():
    print("{0}: {1}".format(key, value))

try:
    file_path = path_info
    with open(file_path, 'r') as file:
        for line in file:
            print(line, end='')
except IOError as e:
    print(e)