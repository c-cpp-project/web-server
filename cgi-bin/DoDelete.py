#!/usr/bin/env python

import sys
import os

def removeFile(file_path):
    try:
        os.remove(file_path)
        return True
    except OSError as e:
        return False


# file_path = sys.stdin.read()
file_path = sys.argv[1]
removeFile(file_path)