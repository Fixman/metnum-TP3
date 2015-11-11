import sys
import os
import subprocess

reset = ' '
if len(sys.argv) >= 5:
        inputFile = sys.argv[1]
        outputFile = sys.argv[2]
        method = int(sys.argv[3])
        jumps = int(sys.argv[4])

        if len(sys.argv) >= 6:
            reset = sys.argv[5]
else:
        print 'Parametros incorrectos'
        sys.exit()

process = subprocess.call('./tp '+inputFile+' '+outputFile+' '+str(method)+' '+str(jumps) + ' ' + reset, shell=True)
