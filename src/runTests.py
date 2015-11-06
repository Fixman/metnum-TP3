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

def ECM(qPixels, frameIdeal, frameGenerado, width, height):
	suma = 0;
	for i in xrange(0, height):
		for j in xrange(0, width):
			suma += pow((frameIdeal[i,j] - frameGenerado[i,j]), 2) 
	return suma / qPixels

# Los frames ideales seran los que se quitaron del video
# original con la funcion quitarFramesPares
def getFramesIdeales(frames):
	idealFrames = []
	n = 0;
	for frame in frames:
		if (n % 2) == 1:
			idealFrames.append(frame)
		n = n + 1
	return idealFrames

#########################
# Parametros de entrada.#
#########################

if len(sys.argv) >= 4:
    inputFile = sys.argv[1]
    outputFile = sys.argv[2]
    method = int(sys.argv[3])
else:
	print 'Parametros incorrectos'
	sys.exit()

############################
# Genera archivo temporal. #
############################

sys.argv = ['tools/videoToTextfile.py', 'data/funnybaby.avi', 'originalVideo.txt', '1']
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

idealFrames = getFramesIdeales(frames)

sys.argv = ['tools/videoToTextfile.py', 'data/funnybaby.avi', 'cuttedVideo.txt', '2']
execfile('tools/videoToTextfile.py')

# os.remove("cuttedVideoTemp.txt")

# os.system('./tp temp.txt out.txt 0 1')
# process = subprocess.Popen('./tp cuttedVideo.txt out.txt 1 1', shell=True)
# process.wait()

# os.remove("cuttedVideo.txt")