# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
# first_name = form.getvalue('first_name')
# last_name = form.getvalue('last_name')

print ("<html>")
print ('<head>')
print ("<title>Hello - Second CGI Program</title>")
print ('</head>')
print ('<body>')

for key in form.keys():
    print ("<h2>key is {0}, value is {1} </h2>".format(key, form.getvalue(key)))

print ('</body>')
print ('</html>')