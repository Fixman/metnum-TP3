import sys
import os
import subprocess
import math
import shutil

#########################
# Funciones Auxiliares  #
#########################

def PSNR(ecm):
	psnr = 10 * math.log(pow(255, 2) / ecm, 10)
	return psnr

def ECM(frameIdeal, frameGenerado, width, height):
	suma = 0;
	for i in xrange(0, height):
		for j in xrange(0, width):
			suma += pow((frameIdeal[i,j] - frameGenerado[i,j]), 2) 
	return suma / (height * width)

# Los frames ideales seran los que se quitaron del video
# original con la funcion quitarFramesPares
def getFramesIdeales(frames, jump):
	idealFrames = []
	n = 0;
	for frame in frames:
		if (n % jump) > 0:
			idealFrames.append(frame)
		n = n + 1
	return idealFrames

#########################
# Parametros de entrada.#
#########################

if len(sys.argv) >= 5:
    inputFile = sys.argv[1]
    outputFile = sys.argv[2]
    jump = int(sys.argv[3])
    method = int(sys.argv[4])
else:
	print 'Parametros incorrectos'
	sys.exit()

############################
# Genera archivo temporal. #
############################

sys.argv = ['tools/videoToTextfile.py', inputFile, 'originalVideo.txt', '1']
execfile('tools/videoToTextfile.py')

f = open("originalVideo.txt","r")
qFrames = float(f.readline())
height,width = map(float, f.readline().split(","))
frameRate = float(f.readline())

####################
# Prepara frames.  #
####################

frames = []

n = 1
pixels = []
for line in f:
	pixels.append(map(int, line.split(",")))
	if (n % height) == 0:
		frames.append(pixels)
		pixels = []
	n = n + 1

idealFrames = getFramesIdeales(frames, jump)

####################################
# Quita frames de video original.  #
####################################

sys.argv = ['tools/videoToTextfile.py', inputFile, outputFile, jump]
execfile('tools/videoToTextfile.py')

####################################
# Regenera los frames con metodo.  #
####################################

process = subprocess.Popen('./tp '+outputFile+' out.txt '+str(method)+' '+str(jump-1), shell=True)
process.wait()

f2 = open("out.txt","r")
qFrames = float(f2.readline())
height,width = map(float, f2.readline().split(","))
frameRate = float(f2.readline())

####################
# Prepara frames.  #
####################

frames2 = []

m = 1
pixels2 = []
for line in f2:
	pixels2.append(map(int, line.split(",")))
	if (m % height) == 0:
		frames2.append(pixels2)
		pixels2 = []
	m = m + 1

generatedFrames = getFramesIdeales(frames2, jump)

print frames[0]
print frames2[0]

####################################
# Convierte a video el generado.   #
####################################

sys.argv = ['tools/textfileToVideo.py', 'out.txt', "out.avi"]
execfile('tools/textfileToVideo.py')

################################
# Remueve archivos generados.  #
################################

os.remove(outputFile)
os.remove("originalVideo.txt")
os.remove("out.txt")