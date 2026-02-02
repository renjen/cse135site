#!/usr/bin/env python3
import os
import json
from datetime import datetime

ip = os.environ.get("REMOTE_ADDR", "unknown")
now = datetime.now().strftime("%a %b %d %H:%M:%S %Y")

data = {
    "message": "Hello Renee",
    "language": "Python",
    "generated_at": now,
    "ip": ip
}

print("Cache-Control: no-cache")
print("Content-Type: application/json\n")
print(json.dumps(data, indent=2))
