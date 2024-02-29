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

def writeChunkedData(data, chunk_size=1024):
    total_size = len(data)
    start_index = 0

    while start_index < total_size:
        end_index = start_index + chunk_size
        chunk = data[start_index:end_index]

        while True:
            try:
                sys.stdout.buffer.write(chunk)
                sys.stdout.flush()
                break  # Writing successful, exit the loop
            except BlockingIOError:
                time.sleep(1)  # Sleep for 1 second (adjust as needed)

        start_index = end_index


file_size = fileUpload(targetDir, filename, length)

data = b''
path = targetDir + "/" + filename
try:
    with open(path, 'rb') as file:
        while True:
            chunk = file.read(chunk_size)
            if not chunk:
                break
            data += chunk
except IOError as e:
    print(e)

writeChunkedData(data)