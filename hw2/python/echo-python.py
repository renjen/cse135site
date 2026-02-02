#!/usr/bin/env python3
import os
import sys
import json
import urllib.parse
from datetime import datetime

method = os.environ.get("REQUEST_METHOD", "UNKNOWN")
content_type = os.environ.get("CONTENT_TYPE", "")
length = int(os.environ.get("CONTENT_LENGTH", 0))
raw_data = ""

if method in ["POST", "PUT", "DELETE"]:
    raw_data = sys.stdin.read(length)
elif method == "GET":
    raw_data = os.environ.get("QUERY_STRING", "")

params = {}

if "application/json" in content_type and raw_data:
    try:
        params = json.loads(raw_data)
    except:
        params = {"error": "Invalid JSON"}
else:
    params = urllib.parse.parse_qs(raw_data)

print("Cache-Control: no-cache")
print("Content-Type: text/html\n")

ip = os.environ.get("REMOTE_ADDR", "unknown")
agent = os.environ.get("HTTP_USER_AGENT", "unknown")
now = datetime.now().strftime("%a %b %d %H:%M:%S %Y")

print("<!DOCTYPE html><html><head><title>Echo Python</title></head><body>")
print("<h1>Echo (Python)</h1><hr/>")
print(f"<p><b>Method:</b> {method}</p>")
print(f"<p><b>Content-Type:</b> {content_type}</p>")
print(f"<p><b>Generated at:</b> {now}</p>")
print(f"<p><b>Your IP:</b> {ip}</p>")
print(f"<p><b>User Agent:</b> {agent}</p>")

print("<h2>Parameters</h2><ul>")

if isinstance(params, dict):
    for key, value in params.items():
        if isinstance(value, list):
            for v in value:
                print(f"<li>{key} = {v}</li>")
        else:
            print(f"<li>{key} = {value}</li>")

print("</ul></body></html>")

