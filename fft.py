#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack

Fs = 500       # sampling rate, Hz, must be integer
Ts = 1.0/Fs
t = np.arange(0, 1, Ts)
ff = 5.0        # sine frequency, Hz, may be float
y = []
with open("/tmp/samples") as f:
    for line in f:
        y.append(float(line.split()[0]))
y = y[:Fs]
y = np.sin(2 * np.pi * ff * t) + 0.5 * np.sin(2 * np.pi * 15 * t)
y = y * np.hamming(Fs)
n = len(y)
k = np.arange(n)
T = n/Fs
frq = k/T
frq = frq[:n/2]
Y = np.fft.fft(y)/n
Y = Y[:n/2]

Y = abs(Y)
m = max(abs(Y))
print(m)
print(Y.argmax())


fig, ax = plt.subplots(2, 1)
ax[0].plot(t,y)
ax[0].set_xlabel('Time')
ax[0].set_ylabel('Amplitude')
ax[1].plot(frq,abs(Y),'r') # plotting the spectrum
ax[1].set_xlabel('Freq (Hz)')
ax[1].set_ylabel('|Y(freq)|')
plt.show()

"""

# generate samples, note conversion to float32 array
samples = np.sin(2*np.pi*np.arange(fs*duration)*f/fs)

y = []
with open("/tmp/samples") as f:
    for line in f:
        y.append(float(line.split()[0]))


# Number of samplepoints
N = 1500
# sample spacing
T = 1.0 / 800
x = np.linspace(0.0, N*T, N)
#y = np.sin(50.0 * 2.0*np.pi*x) + 0.5*np.sin(80.0 * 2.0*np.pi*x)
y = samples
yf = scipy.fftpack.fft(y)
xf = np.linspace(0.0, 1.0/(2.0*T), N/2)

fig, ax = plt.subplots()
ax.plot(xf, 2.0/N * np.abs(yf[0:N/2]))
plt.show()
"""
