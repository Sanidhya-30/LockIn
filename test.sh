#!/bin/bash 
cd ~/LockIn/
cmake .
make
./test
# cd ~/thirdeye/src/test
# python3 plot.py
