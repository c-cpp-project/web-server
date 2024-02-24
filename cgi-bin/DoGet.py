# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
# first_name = form.getvalue('first_name')
# last_name = form.getvalue('last_name')

# value = 1
# while True:
#     if value == 1:
#         value = 0
#     else:
#         value = 1

print ("<html>")
print ('<head>')
print ("<link rel='icon' href='data:,'>")
print ("<title>Hello - GET CGI Program</title>")
print ('</head>')
print ('<body>')

for key in form.keys():
    print ("<h2>key is {0}, value is {1} </h2>".format(key, form.getvalue(key)))

print ('</body>')
print ('</html>')