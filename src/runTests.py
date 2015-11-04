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
qFrames = float(f.readline())
height,width = map(float, f.readline().split(","))
frameRate = float(f.readline())

frames = []

for line in f:
	frames.append(map(int, line.split(",")))

####################
# Prepara frames.  #
####################

cuttedVideo = quitarFramesPares(frames)
cuttedFrames = qFrames / 2
originalFramesCutted = getFramesIdeales(frames)

tf = open('cuttedVideo.txt', 'w+')
tf.write(str(cuttedFrames)+"\n")
tf.write(str(height)+","+str(width)+"\n")
tf.write(str(frameRate)+"\n")

for frame in cuttedVideo:
	tf.write(",".join(str(pixel) for pixel in frame)+"\n")

# os.system('./tp temp.txt out.txt 0 1')
process = subprocess.call('./tp cuttedVideo.txt out.txt 1 1', shell=True)

os.remove("temp.txt")