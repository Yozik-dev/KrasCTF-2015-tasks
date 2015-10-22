from Crypto.Cipher import AES
from Crypto import Random
import sys
import base64

items = {
    0: ("Hat",12),
    1: ("Boots", 3),
    2: ("Gloves", 20),
    3: ("Food", 10),
    4: ("Car", 4),
    5: ("Jacket", 30),
    6: ("A big and beautiful cat",35),
    7: ("The key for the task", 31337)
}

BS = 16
SECRET = '1his is a key123'
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS) 
unpad = lambda s : s[0:-ord(s[-1])]

def xor_strings(xs, ys):
    return "".join(chr(ord(x) ^ ord(y)) for x, y in zip(xs, ys))

def proof(text):
    iv = text[:32].decode('hex')
    txt = text[32:]
    
    price_1 = "3331333337".decode('hex') # 31337
    price_needed = "3030303030".decode('hex') # 0
    pad = "000000" + xor_strings(price_1, price_needed).encode('hex') + "0" * 16
    proofs = []
    xored = xor_strings(iv, pad.decode('hex')).encode('hex')
    proofs.append(xored + txt)
    return proofs

def decode(text):
    iv = text[:32].decode('hex')
    txt = text[32:].decode('hex')

    obj = AES.new(SECRET, AES.MODE_CBC, iv)
    plaintext = obj.decrypt(txt)
    return iv, plaintext

def encode(itemID):
    if not itemID in items:
        return "{error}"

    itemName = items[itemID][0]
    itemPrice = int(items[itemID][1])
    string = "{%d,%d,%s}" % ( itemID, itemPrice, itemName )

    iv = Random.new().read(AES.block_size);
    obj = AES.new(SECRET, AES.MODE_CBC, iv)
    message = pad(string)
    print "iv:  %s" % iv.encode('hex')
    return iv + obj.encrypt(message)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "sys.argv"
        exit()
    #res = "102e52f29fedb4de7fe80461626ac15fab9209c763f65044b389c9b1e9df0a6c18c9a3bb5de7c4b4bd04c6d3619b5f13"#encode(2).encode('hex')
    res = sys.argv[1]
    print "enc: %s" % res

    iv, plaintext = decode(res)
    print "iv_decode: %s" % iv.encode('hex')
    print "text_decode: %s" % plaintext.encode('hex')
    print "text_hex: %s" % unpad(plaintext).encode('hex')
    print "text_clear: %s" % unpad(plaintext)

    proofs = proof(res)
    for i, text in enumerate(proofs):
        print "proof_hex_%d: \t%s" % (i, text)
        print "proof: %s" % unpad(decode(text)[1])