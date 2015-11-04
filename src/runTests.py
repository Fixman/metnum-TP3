import sys
import os
import subprocess
import math

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

def quitarFramesPares(frames):
	newFrames = []
	for frame in frames:
		newFrames.append(frame[::2])
	return newFrames

# Los frames ideales seran los que se quitaron del video
# original con la funcion quitarFramesPares
def getFramesIdeales(frames):
	idealFrames = []
	for frame in frames:
		idealFrames.append(frame[1::2])
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

sys.argv = ['tools/videoToTextfile.py', 'data/funnybaby.avi', 'temp.txt', '1']
execfile('tools/videoToTextfile.py')

f = open("temp.txt","r")
qFrames = f.readline()
height,width = f.readline().split(",")
frameRate = f.readline()

frames = []

for line in f:
	frames.append(map(int, line.split(",")))

####################
# Prepara frames.  #
####################

cuttedVideo = quitarFramesPares(frames)
cuttedFrames = qFrames / 2
originalFramesCutted = getFramesIdeales(frames)



os.remove("temp.txt")