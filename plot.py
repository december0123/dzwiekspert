#!/usr/bin/python

import matplotlib.pyplot as plt

import numpy as np
samples = []
with open("/tmp/fft") as f:
    for line in f:
        samples.append(float(line.split()[0]))

t = [i for i in np.arange(0, 8000, 2.5)]
fig, ax = plt.subplots(2, 1)
ax[0].stem(t, samples)

# dodatna czesc fft, tylko czesc rzeczywista, wychodzi polowa sampli
plt.show()
