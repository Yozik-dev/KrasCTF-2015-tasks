#!/usr/bin/python

import random
import re
import sqlite3
import base64
from Crypto.PublicKey import RSA

DATABASE = '../root/db/main.db'
key64 = open("../config/rsa_1024_priv.pem", 'rb').read()
rsa_object = RSA.importKey(key64)

def sign(text):
    return rsa_object.sign(int(text), None)[0]

if __name__ == "__main__":

    primes = open("primes.txt").read().split('\r\n')
    new_goods = {}
    key_goods = []
    N = 5

    goods = filter(lambda x: len(x), open("list.txt").read().split('\n'))
    for id, el in enumerate(goods):
        cost = random.randint(0,1000)
        new_goods[id] = (id, str(cost), el, sign(str(id)+str(cost)))

    for i in range(N):
        key_id = random.randint(200,999)
        a = re.compile(str(key_id) + "\d+")
        key = filter(a.match, primes)

        if len(key) > 10:
            if key[10][3] != "0":
                key_goods.append(int(key[10]))

    key_cost = reduce(lambda x,y: x*y, key_goods, 1)
    print key_goods, "=>", key_cost

    if str(key_cost)[3] == "0":
        print "Try again!"
        exit()

    key_id = int(str(key_cost)[:3])
    key_new_cost = str(key_cost)[3:]

    for el in key_goods:
        frst = int(str(el)[:3])
        scnd = str(el)[3:]

        k = new_goods[frst]
        print k, "=>",
        new_goods[frst] = (k[0], scnd, k[2], sign(str(k[0])+str(scnd)))
        print new_goods[int(str(el)[:3])]

        if frst == key_id:
            print "Try again!"
            exit()

    k = new_goods[key_id]
    new_goods[key_id] = (k[0], key_new_cost, "KEY ELEMENT FOR GOING ABROAD", sign(str(k[0])+str(key_new_cost)))

    print new_goods[key_id]

    #try:
    con = sqlite3.connect(DATABASE)
    cur = con.cursor()    
    cur.execute('DELETE FROM items')

    cur = con.cursor()

    for key, item in new_goods.iteritems():
        cur.execute("INSERT INTO items VALUES(?,?,?,?)", map(str,item))


    con.commit()

    #except:
    #    print "DB error"
    #    exit()