import time as tm

filename1 = './band_siesta'
filename2 = './kpnt-path' 

f1 = open(filename1, 'r')
f2 = open(filename2, 'w')

while True :
    line1 = f1.readline()
    if not line1: break

