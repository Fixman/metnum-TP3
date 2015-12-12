from __future__ import print_function
from __future__ import division

import argparse
import math
import os
import subprocess
import sys
import time

# Correr ``python compare.py --help'' para mas informacion.

# Parece que Python 2 no tiene subprocess.DEVNULL
DEVNULL = open(os.devnull, 'w+')

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

    parser = argparse.ArgumentParser(description = 'Calcula parametros cualitativos de un metodo en un video comparandolo con el resultado de sacar algunos frames y alentarlo.')
    parser.add_argument('inputFile', type = str, help = 'Video de input.')
    parser.add_argument('outputFile', type = str, help = 'Video de output.')
    parser.add_argument('--jump', type = int, required = True, help = 'Cuanto alentizar el video.')
    parser.add_argument('--method', required = True, choices = metodos + map(str, range(1, 4)), help = 'Metodo a usar (como cadena o entero).')
    parser.add_argument('--reset', type = int, help = 'Cada cuanto se resetea la ventana en interpolacion por splines.')
    parser.add_argument('--meatureTime', action = 'store_true', help = 'Correr el algoritmo 10 veces y medir el tiempo promedio de corrida.')
    parser.add_argument('--mode', choices = {'averages', 'frame'} , default = 'averages', help = 'Imprimir ECM frame a frame, o ECM promedio.')
    args = parser.parse_args()

    try:
	args.method = int(args.method)
    except ValueError:
	args.method = metodos.index(args.method) + 1

    args.textOutput = args.outputFile.replace('.avi', '.txt', 1).replace('.mp4', '.txt', 1)

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

def runProgram(inputFile, outputFile, method, jump, reset):
    start_time = time.time()
    subprocess.call(['./tp', inputFile, outputFile, method, jump] + ([str(reset)] if reset else []))
    return time.time() - start_time

def main():
    args = parseArgs()

    print('Convirtiendo {} a texto.'.format(args.inputFile), file = sys.stderr)
    subprocess.call(['python', 'tools/videoToTextfile.py', args.inputFile, 'originalVideo.txt', '1'], stdout = DEVNULL)
    idealFrames = parseTextFile(open('originalVideo.txt'), args.jump)

    print('Convirtiendo 1 de cada {} frames de {} a texto.'.format(args.jump, args.inputFile), file = sys.stderr)
    subprocess.call(['python', 'tools/videoToTextfile.py', args.inputFile, args.textOutput, str(args.jump)], stdout = DEVNULL)

    if not args.meatureTime:
	runProgram(args.textOutput, 'out.txt', str(args.method), str(args.jump - 1), args.reset)
    else:
	elapsed_time = sum(runProgram(args.outputFile, 'out.txt', str(args.method), str(args.jump - 1), args.reset) for x in range(10))
	print('Tiempo promedio de 10 corridas del algoritmo: {} segundos'.format(elapsed_time / 10))

    generatedFrames = parseTextFile(open('out.txt'), args.jump)

    height = len(idealFrames[0])
    width = len(idealFrames[0][0])
    qFramesCompared = len(generatedFrames)

    errores = (ECM(idealF, generatedF, height, width) for idealF, generatedF in zip(idealFrames, generatedFrames)[:-1])

    if args.mode == 'averages':
        promECM = sum(errores) / qFramesCompared
        promPSNR = PSNR(promECM)

        print('ECM: {}'.format(promECM))
        print('PSNR: {}'.format(promPSNR))
    elif args.mode == 'frame':
        print('frame,ECM')
        for e, f in enumerate(errores):
            print('{},{}'.format(e, f))
    else:
        raise AssertionError('Modo invalido.')

    print('Convirtiendo texto a {}'.format(args.outputFile))
    subprocess.call(['python', 'tools/textfileToVideo.py', 'out.txt', args.outputFile], stdout = DEVNULL)

if __name__ == '__main__':
    main()
