flag = "The flag is KCTF{1b45e7cbcd1e4f12fa8cefe3e544f8e6}"
key = "secret!"

assert len(key) == 7

def xor_strings(xs, ys):
    return "".join(chr(ord(x) ^ ord(y)) for x, y in zip(xs, ys))

def encrypt(plaintext, key):
    plaintext = plaintext + plaintext # for a backup
    key = ''.join(key for i in range(len(plaintext) / len(key) + 1))[:len(plaintext)]

    return xor_strings(plaintext, key)
    
print encrypt(flag, key).encode("hex")