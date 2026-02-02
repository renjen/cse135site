#!/usr/bin/env python3
import os
from datetime import datetime

print("Cache-Control: no-cache")
print("Content-Type: text/html\n")

team = "Renee"
ip = os.environ.get("REMOTE_ADDR", "unknown")
now = datetime.now().strftime("%a %b %d %H:%M:%S %Y")

print("<!DOCTYPE html>")
print("<html><head><title>Hello Python CGI</title></head><body>")
print(f"<h1>Hello {team}</h1><hr/>")
print("<p>This page was made with the Python programming language</p>")
print(f"<p>This program was generated at: {now}</p>")
print(f"<p>Your current IP Address is: {ip}</p>")
print("</body></html>")

