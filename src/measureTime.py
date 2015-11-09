import sys
import os
import subprocess

if len(sys.argv) >= 5:
	inputFile = sys.argv[1]
	outputFile = sys.argv[2]
	method = int(sys.argv[3])
	jumps = int(sys.argv[4])
else:
	print 'Parametros incorrectos'
	sys.exit()

for t in range(1, jumps+1):
    process = subprocess.call('./tp '+inputFile+' '+outputFile+' '+str(method)+' '+str(t), shell=True)
