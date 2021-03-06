#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack
sampling_rate = 16000
fig, ax = plt.subplots(3, 1)
"""
samples = []
files = {"a": "note_a_samples", "noise": "noise_samples", "e": "nie_tykac_kurwa_e_samples", "tmp": "/tmp/samples"}
with open("/tmp/samples") as f:
    for line in f:
        samples.append(float(line.split()[0]))

number_of_samples = len(samples)
samples = samples * np.hanning(number_of_samples)

time = np.arange(0, number_of_samples / sampling_rate, 1 / sampling_rate)
ax[0].plot(time, samples)

# 2**x >= number_of_samples
bins = 2**(number_of_samples - 1).bit_length()

# dodatna czesc fft, tylko czesc rzeczywista, wychodzi polowa sampli
samples_fft = np.fft.rfft(samples)
"""

samples_fft = []
with open("/tmp/fft") as f:
    for line in f:
        samples_fft.append(float(line.split()[0]))
samples_fft = np.array(samples_fft)

# samples_fft_upto_nyquist_freq = samples_fft[:len(samples_fft) / 2]
samples_fft_no_mirror = samples_fft[int(len(samples_fft) / 2) : ]
samples_fft_upto_nyquist_freq = samples_fft_no_mirror[:int(len(samples_fft_no_mirror) / 2)]

# wektor czestotliwosci, od 0 do polowy czestotliwosci probkowania (nyquist)
f = np.linspace(0, sampling_rate / 2, len(samples_fft_upto_nyquist_freq))

ax[1].stem(f, abs(samples_fft_upto_nyquist_freq), 'r')
#ax[1].stem(abs(samples_fft))
plt.show()
