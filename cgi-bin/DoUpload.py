#!/usr/bin/env python

import os
import sys

targetDir = sys.argv[1]
filename = sys.argv[2]
length = int(sys.argv[3])
file_size = 0
chunk_size = 1024 * 1024

def fileUpload(targetDir, filename, length):
    data = b''
    while len(data) != length:
        chunk = sys.stdin.buffer.read(chunk_size)
        if chunk:
            data += chunk
    try:
        if not os.path.exists(targetDir):
            os.makedirs(targetDir)
        path = targetDir + "/" + filename
        with open(path, 'wb') as file:
            file.write(data)
        file_size = os.path.getsize(path)
    except Exception as e:
        print(e)
        return (-1)
    return (file_size)


file_size = fileUpload(targetDir, filename, length)