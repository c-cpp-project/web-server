#!/usr/bin/env python

import os
import sys

data = sys.stdin.buffer.read()
targetDir = sys.argv[1]
filename = sys.argv[2]

data = data.replace(b'\r\n', b'')

def fileUpload(data, targetDir, filename):
    try:
        if not os.path.exists(targetDir):
            os.makedirs(targetDir)
        name = len(os.listdir(targetDir))
        path = targetDir + "/" + filename
        # filename = targetDir + "/" + str(name)
        with open(path, 'wb') as file:
            file.write(data)
    except Exception as e:
        print(e)
        return (False)
    return (True)

if (fileUpload(data, targetDir, filename)):
    print ("<html>")
    print ('<head>')
    print ("<link rel='icon' href='data:,'>")
    print ("<title>Hello - UPLOAD CGI Program</title>")
    print ('</head>')
    print ('<body>')

    print("<h2> Result Success </h2>")
    print("<h2> Result Success </h2>")
    print("<h2> Result Success </h2>")
    print("<h2> Result Success </h2>")
    print("<h2> Result Success </h2>")

    print ('</body>')
    print ('</html>')
else:
    print("500")