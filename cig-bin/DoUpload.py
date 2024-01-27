#!/usr/bin/env python

import cgitb
import os
import sys
import hashlib

def copy_and_paste(src, dest, target):
    # SHA-256 해시 코드 생성
    sha256_hash = hashlib.sha256(target.encode()).hexdigest()
    try:
        os.makedirs(dest, exist_ok=True)
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

def writeFile(data, src, dest):
    # data == "": 함수 종료

    # data != "0": tmp 파일 생성

    # data == "0": tmp 파일이 있다면, 모두 삭제 후에 하나의 파일에 모아서 전송한다.
    if data == "":
        return 
    elif data != "0":
        file_count = len(os.listdir(src)) + 1
        file_name = str("tmp_" + file_count)
        with open(file_name, 'w') as new_file:
            new_file.write(data)
    else:
        target = len(os.listdir(dest))
        copy_and_paste(src, dest, str(target))
        print("200")

# 임시파일을 순서대로 생성하자.
data = sys.stdin.read()
src = "../tmp"
dest = sys.argv[1]

writeFile(data, src, dest)