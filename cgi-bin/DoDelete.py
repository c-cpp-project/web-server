#!/usr/bin/env python

import sys
import os

# file_path = sys.stdin.read()
file_path = sys.argv[1]

os.remove(file_path)

print ("<html>")
print ('<head>')
print ("<link rel='icon' href='data:,'>")
print ("<title>Hello - First CGI Program</title>")
print ('</head>')
print ('<body>')

for i in range(5):
    print ("<h2>File name is {0}, Delete done </h2>".format(file_path))

print ('</body>')
print ('</html>')
