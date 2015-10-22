# -*- coding: utf-8 -*-
import re
import struct
import binascii
import sys


def extract(filename):
    data = open(filename).read()
    match = [m.start(0) for m in re.finditer('tEXt', data)]
    match = match[-1] # поиск последнего текстового чанка
    
    length_data = data[match-4:match] # его длинна
    length_data = struct.unpack('>l', length_data)
    length_data = length_data[0]
    
    length = length_data - 4 # реальная длинна, за минусом 'cat\0'
    chunk = data[match+8:match+8+length] # данные
    return binascii.unhexlify(chunk)
    
    
def main():
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    chunk = extract(input_file)
    out = open(output_file, 'wb')
    out.write(chunk)
    out.close()
    

main()

