from hashlib import md5
from Crypto.Cipher import AES
from Crypto import Random

BS = 16
SECRET = '***'
flag = '***'

pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS) 
unpad = lambda s : s[0:-ord(s[-1])]

def xor_strings(xs, ys):
    return "".join(chr(ord(x) ^ ord(y)) for x, y in zip(xs, ys))

def decode_aes(text):
    iv = text[:32].decode('hex')
    txt = text[32:].decode('hex')

    obj = AES.new(SECRET, AES.MODE_CBC, iv)
    plaintext = obj.decrypt(txt)
    return iv, plaintext

def encode_aes(itemID):
    if not itemID in items:
        return "{error}"

    itemName = items[itemID][0]
    itemPrice = int(items[itemID][1])
    string = "{%d,%d,%s}" % ( itemID, itemPrice, itemName )

    iv = Random.new().read(AES.block_size);
    obj = AES.new(SECRET, AES.MODE_CBC, iv)
    message = pad(string)
    return iv + obj.encrypt(message)

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

def setup():
    request.send(showMenu("menu"))
    print "setup!"

def handle(request,client_address):
    params = {"items":[],"money":100}
    current = 0
    workStatus = 1

    def showMenu(action):
        txt_hello = "Hello, user\nYour money: %s\nYour items: %s\n\n"
        txt1 = "Menu:\n01) Bank\n02) Shop\n03) Exit\n> "
        txt2 = "You are in the bank of the USA."
        txt3 = "Thanks! Here is your secure (AES-CBC) purchase token: %s\n"
        txt4 = "You are in the USA's shop. Items here:\n" + '\n'.join(["%s: %s (%d)" % (key,items[key][0],items[key][1]) for key in items.keys()])
        txt5 = "\nYou can:\n01) Buy\n02) Get out of here\n> "
        txt6 = "\nYou can:\n01) Create purchase token\n02) Get out of here\n> "
        txt_item = "Ok. Input your token: "
        txt_token = "Ok. Input item, which U wanna buy: "
        data = (params["money"], ','.join(params["items"]))

        # return flag
        if "The key for the task" in params["items"]:
            return "The flag is %s" % flag

        if action == "menu":
            return txt_hello % data + txt1
        if action == "Bank":
            return txt_hello % data + txt2 + txt6
        if action == "shop":
            return txt_hello % data + txt4 + txt5
        if action == "tokenize":
            return txt_hello % data + txt_token
        if action == "token":
            token = encode_aes(int(inp_data)).encode("hex")
            return txt_hello % data + txt3 % token + txt2 + txt6
        if action == "item":
            return txt_hello % data + txt_item
        if action == "purchase":
            try:
                iv,a = decode_aes(inp_data)
                a = unpad(a)
                a = a[1:-1].split(',')

                if len(a) < 2: 
                    return txt_hello % data + "ERROR!\n" + txt4 + txt5
                
                itemId = int(a[0])
                money = int(a[1])

                if not itemId in items:
                    return txt_hello % data + "ERROR: no such item in the shop!\n" + txt4 + txt5

                if params["money"] < money:
                    return txt_hello % data + "ERROR: You don't have enough money!\n" + txt4 + txt5

                params["money"] -= money
                params["items"].append(a[2])
                data = (params["money"], ','.join(params["items"]))
                
                if "The key for the task" in params["items"]:
                    return "The flag is %s" % flag

                return txt_hello % data + txt4 + txt5
            except:
                return txt_hello % data + "\nERROR: wrong format!\n" + txt4 + txt5

    request.send(showMenu("menu"))
    while workStatus:
        inp_data = request.recv(1024).strip()
        if len(inp_data) < 1:
            request.shutdown(socket.SHUT_RDWR)
            return False
        print "{} wrote: {}".format(client_address[0],inp_data)

        if "01" in inp_data and current == 0:
            current = 1
            request.send(showMenu("Bank"))
        elif "02" in inp_data and current == 0:
            current = 2
            request.send(showMenu("shop"))
        elif current == 1:
            if "01" in inp_data:
                current = 3
                request.send(showMenu("tokenize"))
            elif "02" in inp_data:
                current = 0
                request.send(showMenu("menu"))
            else:
                request.send(showMenu("Bank"))
        elif current == 2:
            if "01" in inp_data:
                current = 4
                request.send(showMenu("item"))
            elif "02" in inp_data:
                current = 0
                request.send(showMenu("menu"))
            else:
                request.send(showMenu("shop"))
        elif "03" in inp_data and current == 0:
            request.shutdown(socket.SHUT_RDWR)
            return False
        elif current == 3:
            current = 1
            request.send(showMenu("token"))
        elif current == 4:
            current = 2
            request.send(showMenu("purchase"))
        else:
            current = 0
            request.send(showMenu("menu"))