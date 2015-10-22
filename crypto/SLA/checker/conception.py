def xor_strings(xs, ys):
    return "".join(chr(ord(x) ^ ord(y)) for x, y in zip(xs, ys))

def decrypt(plaintext, key):
    key = ''.join(key for i in range(len(plaintext) / len(key) + 1))[:len(plaintext)]

    return xor_strings(plaintext, key)

ct = "270d065203184014450a01453f622723184307401416520010061010165105435712404b060614004744465157145d11170e310b1745124d1202431b16546a303125095416154600541107174542005714544647125d001703111216505746034c444518"
ct = ct.decode("hex")

c1 = ct[:len(ct)/2]
c2 = ct[len(ct)/2:]

key_len = 7

assert c1+c2 == ct
assert len(ct) % key_len != 0

matrix = {}

k_index1 = 0
k_index2 = len(c1) % key_len

print k_index1, k_index2

ofst = 0
for i in range(k_index2, key_len):
    matrix[i] = chr(ord(c1[ofst]) ^ ord(c2[ofst]))
    print (ofst, i, matrix[i])
    ofst += 1

for i in range(0, k_index2):
    matrix[i] = chr(ord(c1[ofst]) ^ ord(c2[ofst]))
    print (ofst, i, matrix[i])
    ofst += 1

print matrix

for bru in range(256):
    key = [0 for i in range(key_len)]
    key[0] = bru

    f_next_i = k_index2
    f_next_v = None

    f_current_i = 0
    f_current_v = bru

    for i in range(key_len-1):
        f_next_v = ord(c1[i]) ^ ord(c2[i]) ^ f_current_v
        key[f_next_i] = f_next_v

        k = f_next_i
        f_current_i = f_next_i
        f_current_v = f_next_v

        f_next_i = (k + k_index2) % key_len
        f_next_v = None

    current_key = ''.join([chr(i) for i in key])
    print decrypt(c1, current_key)
