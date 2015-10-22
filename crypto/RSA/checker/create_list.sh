#!/bin/bash

cat products_list.txt | sort -u > list.txt
python staff_generation.py