#!/bin/bash
sqlite3 ../root/db/main.db < ../root/db/schema.sql
openssl genrsa -out rsa_1024_priv.pem 1024
openssl rsa -pubout -in rsa_1024_priv.pem -out ../root/static/rsa_1024_pub.pem