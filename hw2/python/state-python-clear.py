#!/usr/bin/env python3

print("Cache-Control: no-cache")
print("Content-Type: text/html")
print("Set-Cookie: hw2state=; Path=/hw2/python; Expires=Thu, 01 Jan 1970 00:00:00 GMT; HttpOnly")
print("")

print("<!DOCTYPE html><html><head><title>State Cleared</title></head><body>")
print("<h1>State Cleared (Python)</h1><hr/>")
print('<p>State cleared.</p>')
print('<p><a href="state-python-view.py">Go back to View State</a></p>')
print("</body></html>")
