import sys
import os
import subprocess
import math
import time

#########################
# Parametros de entrada.#
#########################

if len(sys.argv) >= 5:
	inputFile = sys.argv[1]
	outputFile = sys.argv[2]
	method = int(sys.argv[3])
	jump = int(sys.argv[4])
else:
	print 'Parametros incorrectos'
	sys.exit()

####################################
# Regenera los frames con metodo.  #
####################################

times = 10
prom_elapsed_time = 0

for t in xrange(1, times):
	start_time = time.time()
	process = subprocess.Popen('./tp '+inputFile+' '+outputFile+' '+str(method)+' '+str(jump), shell=True)
	process.wait()
	prom_elapsed_time = prom_elapsed_time + (time.time() - start_time)

prom_elapsed_time = prom_elapsed_time / 10

print "\nTiempo promedio 10 corridas algoritmo: "+str(prom_elapsed_time)+"\n"
