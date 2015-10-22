#!/usr/bin/python

import sys
from position import Desk

N = 4
valid_matrix = [[i*N + j + 1 for j in range(N)] for i in range(N)]

def beautyfier(matrix):
    try:
        new_matrix_1 = map(lambda x: int(x), filter(lambda x: len(x), matrix.split(",")))
        if len(new_matrix_1) != N*N:
            return False

        e_index = new_matrix_1.index(N*N)
        e_x = e_index / 4
        e_y = e_index % 4

        if not e_index:
            return False

        new_matrix_2 = []
        for i in range(N):
            new_matrix_2.append([i for i in new_matrix_1[:N]])
            new_matrix_1 = new_matrix_1[N:]

        return new_matrix_2, e_x, e_y
    except:
        return False

matrix = "1,2,11,8,5,3,16,7,10,4,14,9,13,6,15,12" # 1143321432214332143214432143241112333211432214143322144321433221432141143332141332113341112332114332143321432111433211433321432134111233341112333
matrix, e_x, e_y = beautyfier(matrix)

if not matrix:
    print "Invalid matrix input"
    exit()

if __name__ == "__main__":
    assert len(sys.argv) == 2

    FLAG = sys.argv[1] # 210332

    assert len(FLAG) < 1024

    try:
        desk = Desk(4,4,matrix,e_x,e_y)

        for c in FLAG:
            desk.change_neighbour(ord(c) % 4 + 1)

        if desk.matrix == valid_matrix:
            print True
        else:
            print False
    except:
        print False