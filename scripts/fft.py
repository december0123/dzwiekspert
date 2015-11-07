#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack

samples = []
files = {"a": "note_a_samples", "noise": "noise_samples", "e": "nie_tykac_kurwa_e_samples", "tmp": "/tmp/samples"}
with open(files["tmp"]) as f:
    for line in f:
        samples.append(float(line.split()[0]))

sampling_rate = 16000
number_of_samples = len(samples)
samples = samples * np.hanning(number_of_samples)

fig, ax = plt.subplots(2, 1)
time = np.arange(0, number_of_samples / sampling_rate, 1 / sampling_rate)
#time = np.arange(0, 4096, 1)
ax[0].plot(time, samples)

# dodatna czesc fft, tylko czesc rzeczywista, wychodzi polowa sampli
samples_fft = np.fft.rfft(samples)

samples_fft_upto_nyquist_freq = samples_fft[:len(samples_fft) / 2]

# wektor czestotliwosci, od 0 do polowy czestotliwosci probkowania (nyquist)
f = np.linspace(0, sampling_rate / 2, len(samples_fft_upto_nyquist_freq))
ax[1].stem(f, abs(samples_fft_upto_nyquist_freq), 'r')
plt.show()
