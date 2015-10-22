#!/usr/bin/python
import sys

def matmult(a,b):
    zip_b = zip(*b)
    return [[sum(ele_a*ele_b for ele_a, ele_b in zip(row_a, col_b)) % 256
             for col_b in zip_b] for row_a in a]

ROWLEN = 3
KEYLEN = ROWLEN * ROWLEN

KEY_MATRIX = [[*, *, *], [*, *, *], [*, *, *]]

def encrypt(f, f2):
    f = f.read()
    if len(f) % KEYLEN:
        f = f + '\x00' * (KEYLEN - len(f) % KEYLEN)

    f_len = len(f)

    for i in range(f_len / KEYLEN):
        block = f[i*KEYLEN:(i+1)*KEYLEN]

        matrix = [[0 for j in range(ROWLEN)] for jj in range(ROWLEN)]
        for j in range(ROWLEN):
            for jj in range(ROWLEN):
                matrix[j][jj] = ord(block[j*ROWLEN+jj])

        y = matmult(matrix, KEY_MATRIX)
        f2.write(''.join([chr(item) for sublist in y for item in sublist]))

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "Usage: ./encrypt.py [flag.pdf] [flag.enc]"
        exit()

    try:
        f = open(sys.argv[1], "rb")
        f2 = open(sys.argv[2], "wb")

        encrypt(f, f2) 
    except:
        print "Something went wrong!"
