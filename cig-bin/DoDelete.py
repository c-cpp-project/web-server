#!/usr/bin/env python

import sys
import os

# file_path = sys.stdin.read()
file_path = sys.argv[1]

# Check if the file path is provided
if file_path:
    try:
        # Attempt to remove the file
        os.remove(file_path)
        print("204")
    except OSError as e:
        print("500")
else:
    print("404")
