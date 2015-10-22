flag = "[CENSORED]"
key = "[CENSORED]"

assert len(key) == 7

def xor_strings(xs, ys):
    return "".join(chr(ord(x) ^ ord(y)) for x, y in zip(xs, ys))

def encrypt(plaintext, key):
    plaintext = plaintext + plaintext # for a backup
    key = ''.join(key for i in range(len(plaintext) / len(key) + 1))[:len(plaintext)]

    return xor_strings(plaintext, key)
    
print encrypt(flag, key).encode("hex")

#270d065203184014450a01453f622723184307401416520010061010165105435712404b060614004744465157145d11170e310b1745124d1202431b16546a303125095416154600541107174542005714544647125d001703111216505746034c444518