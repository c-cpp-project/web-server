#!/usr/bin/env python

import os
import sys

data = sys.stdin.read()
targetDir = sys.argv[1]
kind = sys.argv[2]

while (data.find("\r\n") != -1):
    data = data.replace("\r\n", "")

def fileUpload(data, targetDir, kind):
    try:
        if not os.path.exists(targetDir):
            os.makedirs(targetDir)
        name = len(os.listdir(targetDir))
        if "html" in kind:
            extension = ".html"
        elif "css"  in kind:
            extension = ".css"
        elif "txt" in kind:
            extension = ".txt"
        else:
            extension = ""
        filename = targetDir + "/" + str(name) + extension
        with open(filename, 'w') as file:
            file.write(data)
    except Exception as e:
        return (False)
    return (True)

if (fileUpload(data, targetDir, kind)):
    print ("<html>")
    print ('<head>')
    print ("<link rel='icon' href='data:,'>")
    print ("<title>Hello - UPLOAD CGI Program</title>")
    print ('</head>')
    print ('<body>')

    print("<h2> Result {0} </h2>".format(result))
    print("<h2> Result {0} </h2>".format(result))
    print("<h2> Result {0} </h2>".format(result))
    print("<h2> Result {0} </h2>".format(result))
    print("<h2> Result {0} </h2>".format(result))

    print ('</body>')
    print ('</html>')
else:
    print("500")