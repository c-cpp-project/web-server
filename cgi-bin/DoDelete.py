#!/usr/bin/env python

import sys
import os, shutil

def removeFile(file_path):
    try:
        if os.path.isfile(file_path):
            os.remove(file_path)
        if os.path.isdir(folder_path):
            shutil.rmtree(folder_path)
        return True
    except OSError as e:
        return False


# file_path = sys.stdin.read()
file_path = sys.argv[1]
removeFile(file_path)