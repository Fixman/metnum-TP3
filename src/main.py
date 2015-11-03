import sys
import os
import subprocess
import math

#########################
# Parametros de entrada.#
#########################

def PSNR(ecm):
	psnr = 10 * math.log(pow(255, 2) / ecm, 10)

    return psnr

def ECM(x):
    return x

if len(sys.argv) >= 4:
    inputFile = sys.argv[1]
    outputFile = sys.argv[2]
    method = int(sys.argv[3])
else:
	print 'Parametros incorrectos'
	sys.exit()

# retcode = subprocess.call(['./tools/videoToTextfile.py', 'data/funnybaby.avi', 'funnytest.txt', '1'])

sys.argv = ['tools/videoToTextfile.py', 'data/funnybaby.avi', 'funnytest.txt', '1']
execfile('tools/videoToTextfile.py')

# file = open(textFilename,"r")