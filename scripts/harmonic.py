#!/usr/bin/python

from numpy import polyfit, arange

def parabolic(f, x):
    """Quadratic interpolation for estimating the true position of an
    inter-sample maximum when nearby samples are known.
   
    f is a vector and x is an index for that vector.
   
    Returns (vx, vy), the coordinates of the vertex of a parabola that goes
    through point x and its two neighbors.
   
    Example:
    Defining a vector f with a local maximum at index 3 (= 6), find local
    maximum if points 2, 3, and 4 actually defined a parabola.
   
    In [3]: f = [2, 3, 1, 6, 4, 2, 3, 1]
   
    In [4]: parabolic(f, argmax(f))
    Out[4]: (3.2142857142857144, 6.1607142857142856)
   
    """
    xv = 1/2. * (f[x-1] - f[x+1]) / (f[x-1] - 2 * f[x] + f[x+1]) + x
    yv = f[x] - 1/4. * (f[x-1] - f[x+1]) * (xv - x)
    return (xv, yv)


from numpy.fft import rfft, irfft
from numpy import argmax, sqrt, mean, diff, log
from matplotlib.mlab import find
from scipy.signal import blackmanharris, fftconvolve
signal = []
files = {"a": "note_a_samples", "noise": "noise_samples", "e": "nie_tykac_kurwa_e_samples", "tmp": "/tmp/fft_samples"}
with open(files["tmp"]) as f:
    for line in f:
        signal.append(float(line.split()[0]))

def freq_from_HPS(signal, fs):
    """
    Estimate frequency using harmonic product spectrum (HPS)
    
    """
    windowed = signal * blackmanharris(len(signal))

    from pylab import subplot, plot, log, copy, show

    #harmonic product spectrum:
    c = abs(rfft(windowed))
    maxharms = 8
    subplot(maxharms,1,1)
    plot(log(c))
    for x in range(2,maxharms):
        a = copy(c[::x]) #Should average or maximum instead of decimating
        # max(c[::x],c[1::x],c[2::x],...)
        c = c[:len(a)]
        i = argmax(abs(c))
        true_i = parabolic(abs(c), i)[0]
        print('Pass %d: %f Hz' % (x, fs * true_i / len(windowed)))
        c *= a
        subplot(maxharms,1,x)
        plot(log(c))
    show()

freq_from_HPS(signal, 16000)
