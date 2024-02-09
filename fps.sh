#!/bin/bash 
cd ~/LockIn/
cmake .
make
# rm -f fps_data.csv
./test2
cd ~/thirdeye/src/test
python3 plot.py