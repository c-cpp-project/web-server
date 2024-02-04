#!/usr/bin/env python

import sys
import os

def removeFile(file_path):
    try:
        os.remove(file_path)
        return True
    except OSError as e:
        return False


# file_path = sys.stdin.read()
file_path = sys.argv[1]

if (removeFile(file_path)):
    print ("<html>")
    print ('<head>')
    print ("<link rel='icon' href='data:,'>")
    print ("<title>Hello - DELETE CGI Program</title>")
    print ('</head>')
    print ('<body>')

    print("<h1>{0} successfully deleted.</h1>".format(file_path))
    for i in range(5):
        print ("<h2>File name is {0}, Delete done </h2>".format(file_path))

    print ('</body>')
    print ('</html>')
else
    print("500")