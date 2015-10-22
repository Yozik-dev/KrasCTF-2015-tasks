#!/usr/bin/python
import sys
from create import ROWLEN, KEYLEN, KEY_MATRIX, KEY_MATRIX_INV, matmult


def decrypt(f, f2):
    f = f.read()
    assert len(f) % KEYLEN == 0

    f_len = len(f)

    for i in range(f_len / KEYLEN):
        block = f[i*KEYLEN:(i+1)*KEYLEN]

        matrix = [[0 for j in range(ROWLEN)] for jj in range(ROWLEN)]
        for j in range(ROWLEN):
            for jj in range(ROWLEN):
                matrix[j][jj] = ord(block[j*ROWLEN+jj])

        if i == 0:
            first_block_of_jpg = [[37, 80, 68], [70, 45, 49], [46, 53, 10]]
            first_block_of_jpg_inv = [[109, 52, 28], [50, 218, 179], [232, 39, 177]]
            x = matmult(first_block_of_jpg_inv, matrix)

            if x == KEY_MATRIX:
                print "[KEY_MATRIX == KEY_EVALUATED]", x == KEY_MATRIX
            else:
                print "[KEY_MATRIX != KEY_EVALUATED]", x, KEY_MATRIX
                exit()

        y = matmult(matrix, KEY_MATRIX_INV)
        f2.write(''.join([chr(item) for sublist in y for item in sublist]))

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "Usage: ./conception.py [flag.enc] [flag.pdf]"
        exit()

    try:
        f = open(sys.argv[1], "rb")
        f2 = open(sys.argv[2], "wb")

        decrypt(f, f2) 
    except:
        print "Something went wrong!"
