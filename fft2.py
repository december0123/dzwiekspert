#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack

samples = []
files = {"a": "note_a_samples", "noise": "noise_samples", "e": "nie_tykac_kurwa_e_samples", "tmp": "/tmp/samples"}
with open(files["e"]) as f:
    for line in f:
        samples.append(float(line.split()[0]))

sampling_rate = 16000
number_of_samples = len(samples)
samples = samples * np.hanning(number_of_samples)

fig, ax = plt.subplots(3, 1)
witam = number_of_samples / sampling_rate
time = np.arange(0, number_of_samples / sampling_rate, 1 / sampling_rate)
ax[0].plot(time, samples)
bins = 2**(number_of_samples - 1).bit_length()
samples_fft = np.fft.fft(samples, bins) # wysrodkowana fft
# wartosci mocy w kazdej czestotliwosci
# wektor jest znormalizowany ( wartosci x sa podzielone przez liczbe slupkow)
# oraz podzielone przez dwa, bo centrum wykresu jest przesuniete na 0
# nastepnie pomnozono przez czestotliwosc probkowania, aby uzyskac prawdziwy wykres
nVals = [x / bins * sampling_rate for x in range(bins)] 

ax[1].plot(nVals, abs(samples_fft))
plt.show()
