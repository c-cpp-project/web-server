#!/usr/bin/env python

import sys
import os, shutil

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
path_info = os.environ["PATH_TRANSLATED"]
for key, value in os.environ.items():
    if key.startswith("HTTP_"):
        key_without_http = key[5:]  # "HTTP_" 부분을 제외한 나머지 부분
        headers[key_without_http] = value
    else:
        headers[key]  = value
for key, value in headers.items():
    print("{0}: {1}".format(key, value))
value = removeFile(path_info)