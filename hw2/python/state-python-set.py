#!/usr/bin/env python3
import os
import sys
import urllib.parse
from datetime import datetime

method = os.environ.get("REQUEST_METHOD", "GET")
length = int(os.environ.get("CONTENT_LENGTH", 0))

data = ""
if method == "POST":
    data = sys.stdin.read(length)

params = urllib.parse.parse_qs(data)

name = params.get("name", [""])[0]
favorite = params.get("favorite", [""])[0]

now = datetime.now().strftime("%a %b %d %H:%M:%S %Y")
cookie_value = urllib.parse.quote(f"name={name}&favorite={favorite}&saved_at={now}")

print("Cache-Control: no-cache")
print("Content-Type: text/html")

# Only set cookie if user submitted data
if method == "POST":
    print(f"Set-Cookie: hw2state={cookie_value}; Path=/hw2/python; HttpOnly")

print("")  # end headers

print("<!DOCTYPE html><html><head><title>State Set (Python)</title></head><body>")
print("<h1>State Demo (Python) - Set</h1><hr/>")
print("<p>Enter values to save (server-side cookie):</p>")

print('<form method="POST" action="state-python-set.py">')
print('Name: <input type="text" name="name" /> <br/><br/>')
print('Favorite: <input type="text" name="favorite" /> <br/><br/>')
print('<input type="submit" value="Save State" />')
print("</form>")

print('<p><a href="state-python-view.py">Go to View State</a></p>')
print("</body></html>")
