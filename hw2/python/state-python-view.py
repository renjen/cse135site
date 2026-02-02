#!/usr/bin/env python3
import os
import urllib.parse

cookie_header = os.environ.get("HTTP_COOKIE", "")
cookies = {}

for part in cookie_header.split(";"):
    part = part.strip()
    if "=" in part:
        k, v = part.split("=", 1)
        cookies[k] = v

state_raw = cookies.get("hw2state", "")
state_decoded = urllib.parse.unquote(state_raw)
state = urllib.parse.parse_qs(state_decoded)

name = state.get("name", ["(none)"])[0]
favorite = state.get("favorite", ["(none)"])[0]
saved_at = state.get("saved_at", ["(none)"])[0]

print("Cache-Control: no-cache")
print("Content-Type: text/html\n")

print("<!DOCTYPE html><html><head><title>State View (Python)</title></head><body>")
print("<h1>State Demo (Python) - View</h1><hr/>")

print(f"<p><b>Name:</b> {name}</p>")
print(f"<p><b>Favorite:</b> {favorite}</p>")
print(f"<p><b>Saved At:</b> {saved_at}</p>")

print('<form method="POST" action="state-python-clear.py">')
print('<input type="submit" value="Clear State" />')
print("</form>")

print('<p><a href="state-python-set.py">Back to Set State</a></p>')
print("</body></html>")
