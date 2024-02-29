#!/usr/bin/env python

import cgi, sys, os

# value = 1
# while True:
#     if value == 1:
#         value = 0
#     else:
#         value = 1

targetDir = sys.argv[1]
query_string = sys.argv[2]
file_name = sys.argv[3]

# print("this is query_string:" + query_string)

try:
    if not os.path.exists(targetDir):
        os.makedirs(targetDir)
    fullpath = targetDir + "/" + file_name
    with open(fullpath, 'w') as file:
        file.write(query_string)
    file.close()
except Exception as e:
    print(e)

# try:
#     with open(fullpath, 'r') as file:
#         while True:
#             chunk = file.read(1024)
#             if not chunk:
#                 break
#             print(chunk, end='')
# except IOError as e:
#     print(e)