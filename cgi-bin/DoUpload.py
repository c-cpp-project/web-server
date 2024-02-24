#!/usr/bin/env python

import os
import sys

targetDir = sys.argv[1]
filename = sys.argv[2]
file_size = 0

def fileUpload(targetDir, filename):
    data = b''
    while True:
        chunk = sys.stdin.buffer.read(1028)
        if not chunk:
            break
        data += chunk
    # data = data.replace(b'\r\n', b'')
    # print(len(data), len(data[:-2]))
    data = data[:-2]
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

file_size = fileUpload(targetDir, filename)

if (file_size >= 0):
    print ("<html>")
    print ('<head>')
    print ("<link rel='icon' href='data:,'>")
    print ("<title>Hello - UPLOAD CGI Program</title>")
    print ('</head>')
    print ('<body>')

    print(file_size)
    print("<h2> Result Success </h2>")
    print("<h2> Result Success </h2>")
    print("<h2> Result Success </h2>")
    print("<h2> Result Success </h2>")
    print("<h2> Result Success </h2>")

    print ('</body>')
    print ('</html>')
else:
    print("500")