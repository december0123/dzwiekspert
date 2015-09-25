#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack
"""

Fs = 8000       # sampling rate, Hz, must be integer
Ts = 1.0/Fs
t = np.arange(0, 1, Ts)
ff = 5.0        # sine frequency, Hz, may be float
y = []
with open("/tmp/samples") as f:
    for line in f:
        y.append(float(line.split()[0]))
y = y[:Fs]
# y = np.sin(2 * np.pi * ff * t) + 0.5 * np.sin(2 * np.pi * 15 * t)
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


y = []
with open("samples") as f:
    for line in f:
        y.append(float(line.split()[0]))

frequency = 220 # czestotliwosc sygnalu
sampling_rate = 8000.0 # czestotliwosc probkowania, powinna byc przynajmniej 2x wieksza niz czestotliwosc
cycles = 10 # liczba cykli do wygenerowania
t = np.arange(0, cycles / frequency, 1.0 / sampling_rate) # podstawa czasowa w ktorej mamy sygnal
x = np.sin(2 * np.pi * frequency * t) # wartosci funkcji w kazdej chwili czasu
x = x * np.hamming(len(x))

N = 4096 # liczba probek, potega 2
X = np.fft.fftshift(np.fft.fft(x, N)) # wysrodkowana fft

# wartosci mocy w kazdej czestotliwosci
# wektor jest znormalizowany ( wartosci x sa podzielone przez liczbe probek)
# oraz podzielone przez dwa, bo centrum wykresu jest przesuniete na 0
# nastepnie pomnozono przez czestotliwosc probkowania, aby uzyskac prawdziwy wykres
nVals = [x / N / 2 * sampling_rate for x in range(-N, N, 2)] 

Px = X * np.conjugate(X) / (N * len(x))
fig, ax = plt.subplots(3, 1)
ax[0].plot(t, x)
ax[1].plot(nVals, abs(X), 'r')
nVals = [x / N * sampling_rate for x in np.arange(0, N / 2)] 
print(len(nVals))
print(len(Px[0:N/2]))
ax[2].plot(nVals, abs(Px[N/2:]), 'g')
plt.show()
