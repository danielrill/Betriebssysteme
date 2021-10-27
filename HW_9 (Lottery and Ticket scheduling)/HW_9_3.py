#! /usr/bin/env python3

import numpy as np
import os
import matplotlib.pyplot as plt
plt.style.use('fivethirtyeight')


def Done():
    out = []
    for i in range(1000):
        randval = np.random.randint(1, 150)
        tmp = os.popen(
            f"python3 lottery.py -l 100:100,100:100 -s {randval} -c").read()
        out.append(tmp)
    return np.array(out)


tmplist = Done()


def search(tl):
    list0 = []
    list1 = []
    res = {}
    for e in np.array(tl):
        e1 = e.split("\n")
        for e2 in e1:
            if e2.startswith("--> JOB 1"):
                time1 = e2.split(" ")
                list1.append(time1[-1])
            if e2.startswith("--> JOB 0"):
                time0 = e2.split(" ")
                list0.append(time0[-1])

    res['ZERO'] = np.array(list0)
    res['ONE'] = np.array(list1)
    return res


zero = sorted(search(tmplist)['ZERO'])
one = sorted(search(tmplist)['ONE'])


zeromax = zero.count('200')
onemax = one.count('200')
fig, ax = plt.subplots(2, figsize=(10, 4))
ax[0].hist(zero, bins=30, color='blue', alpha=0.5)
ax[0].set_title(f"Bei 100 Iterationen wird {zeromax}x das Maximum erreicht")
ax[1].hist(one, bins=30, color='red', alpha=0.5)
ax[1].set_title(f"Bei 100 Iterationen wird {onemax}x das Maximum erreicht")
