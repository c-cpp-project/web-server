#!/usr/bin/env python

import sys
import os

# file_path = sys.stdin.read()
file_path = sys.argv[1]

print ("<html>")
print ('<head>')
print ("<link rel='icon' href='data:,'>")
print ("<title>Hello - Second CGI Program</title>")
print ('</head>')
print ('<body>')

try:
    os.remove(file_path)
    print("<h1>{0} successfully deleted.</h1>".format(file_path))
    for i in range(5):
        print ("<h2>File name is {0}, Delete done </h2>".format(file_path))
except OSError as e:
    print("<h1>{0} Error deleted.</h1>".format(file_path))

print ('</body>')
print ('</html>')
