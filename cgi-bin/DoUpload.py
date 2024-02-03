#!/usr/bin/env python

import os
import sys
import hashlib

def writeFile(src, dest, target):
    # SHA-256 해시 코드 생성
    sha256_hash = hashlib.sha256(target.encode()).hexdigest()
    try:
        files = os.listdir(src)

        for filename in files:
            source_path = os.path.join(src, filename)
            destination_path = os.path.join(dest, sha256_hash)

            # Copy
            with open(source_path, 'rb') as source_file:
                with open(destination_path, 'ab') as destination_file:
                    destination_file.write(source_file.read())

        # Delete All
        for filename in files:
            file_path = os.path.join(src, filename)
            os.remove(file_path)

        # print("Original files deleted from source directory.")

    except Exception as e:
        print("500")
    print("200")

# 임시파일을 순서대로 생성하자.
data = sys.stdin.read()
dest = sys.argv[1]

writeFile(data, src, dest)