from __future__ import print_function
from __future__ import division

import sys
import os
import subprocess
import math
import time

import argparse

def PSNR(ecm):
	x = pow(255, 2) / ecm
	psnr = 10 * math.log(x, 10)
	return psnr

def ECM(frameIdeal, frameGenerado, height, width):
	suma = 0;
	for i in xrange(0, height-1):
	    for j in xrange(0, width-1):
		suma += pow((frameIdeal[i][j] - frameGenerado[i][j]), 2)

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

def parseArgs():
    metodos = ['vecinoMasCercano', 'interpolacionLineal', 'interpolacionPorSplines']

    parser = argparse.ArgumentParser(description = 'Relentizar un video')
    parser.add_argument('inputFile', type = str, help = 'Video de input')
    parser.add_argument('outputFile', type = str, help = 'Video de output')
    parser.add_argument('--jump', type = int, required = True, help = 'Cada relentizar el video')
    parser.add_argument('--method', required = True, choices = metodos + map(str, range(1, 4)), help = 'Metodo a usar (como cadena o entero)')
    parser.add_argument('--reset', type = int, help = 'Cada cuanto se resetea la ventana en interpolacion por splines')
    args = parser.parse_args()

    try:
	args.method = int(args.method)
    except ValueError:
	args.method = metodos.index(args.method)

    return args

def parseTextFile(f, jump):
    qFrames = int(next(f))
    height, width = [int(x) for x in next(f).split(',')]
    frameRate = float(next(f))

    frames = []

    n = 1
    pixels = []
    for line in f:
	    pixels.append(map(int, line.split(",")))
	    if (n % height) == 0:
		    frames.append(pixels)
		    pixels = []
	    n += 1

    return getFramesIdeales(frames, jump)

def main():
    args = parseArgs()

    subprocess.call(['python', 'tools/videoToTextfile.py', args.inputFile, 'originalVideo.txt', '1'])
    idealFrames = parseTextFile(open('originalVideo.txt'), args.jump)
    subprocess.call(['python', 'tools/videoToTextfile.py', args.inputFile, args.outputFile, str(args.jump)])

    times = 10
    prom_elapsed_time = 0

    for t in xrange(1, times):
	    start_time = time.time()
	    subprocess.call(['./tp', args.outputFile, 'out.txt', str(args.method), str(args.jump - 1)] + ([str(args.reset)] if args.reset else []))
	    prom_elapsed_time = prom_elapsed_time + (time.time() - start_time)

    prom_elapsed_time /= 10

    print('Tiempo promedio de 10 corridas del algoritmo: {}'.format(prom_elapsed_time), file = sys.stderr)

    generatedFrames = parseTextFile(open('out.txt'), args.jump)

    qFramesCompared = len(generatedFrames)
    totalEcm = 0

    height = len(idealFrames[0])
    width = len(idealFrames[0][0])
    for idealF, generatedF in zip(idealFrames, generatedFrames)[:-1]:
	    totalEcm = totalEcm + ECM(idealF, generatedF, height, width)

    promECM = totalEcm / qFramesCompared
    promPSNR = PSNR(promECM)

    print('ECM: {}'.format(promECM))
    print('PSNR: {}'.format(promPSNR))

    subprocess.call(['python', 'tools/textfileToVideo.py', 'out.txt', args.outputFile + ".avi"])

    os.remove(args.outputFile)
    os.remove("originalVideo.txt")
    os.remove("out.txt")

if __name__ == '__main__':
    main()
