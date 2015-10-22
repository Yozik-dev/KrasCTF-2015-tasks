import struct
import binascii
import sys


def generate(keyword, text):
    output = 'tEXt'
    output += keyword
    output += chr(0)
    output += text
    
    length = len(output)-4
    crc = binascii.crc32(output)
    
    output = struct.pack(">I", length) + output + struct.pack(">l", crc)
    return output
    
    
def file_to_hex(filename):
    data = open(filename).read()
    return binascii.hexlify(data)
    
    
def main():
    input_file = sys.argv[1]
    png_file = sys.argv[2]
    output_file = sys.argv[3]
    
    input_data = open(input_file).read()
    first_part = input_data[:-12]
    last_part = input_data[-12:]
    
    insert = file_to_hex(png_file)
    insert = generate('cat', insert)
    
    output_data = first_part + insert + last_part
    out = open(output_file, 'wb')
    out.write(output_data)
    out.close()
    

main()

