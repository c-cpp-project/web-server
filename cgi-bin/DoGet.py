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
field_storage = cgi.FieldStorage(environ={"QUERY_STRING": query_string})

# print("this is query_string:" + query_string)

try:
    if not os.path.exists(targetDir):
        os.makedirs(targetDir)
    file_list = os.listdir(targetDir)
    file_name = str(len(file_list))
    path = targetDir + "/" + file_name + ".txt"
    with open(path, 'w') as file:
        for key in field_storage.keys():
            file.write("key : {0}, value : {1}\n".format(key, field_storage.getvalue(key)))
    file.close()
except Exception as e:
    print(e)

print ("<html>")
print ('<head>')
print ("<link rel='icon' href='data:,'>")
print ("<title>Hello - GET CGI Program</title>")
print ('</head>')
print ('<body>')

for key in field_storage.keys():
    print ("<h2>key is {0}, value is {1} </h2>".format(key, field_storage.getvalue(key)))

print ('</body>')
print ('</html>')