
import numpy as np
import random
import matplotlib.pyplot as plt
plt.style.use('fivethirtyeight')

"""
What fraction of randomly-generated virtual addresses
are valid,as a function of the value of the bounds register?

func(bounds) = Anteil virtual ?

Make a graph from running with different random seeds, 
with limit values ranging from 0 up to the maximum size of
the address space.


"""


spaceSize = 1000
physicalSize = 1000  # 8 * spaceSize
uList = [0] * physicalSize

for i in range(100):
    random.seed(i)

    for j in range(physicalSize):
        limit = j
        virtual = int(spaceSize * random.random())
        if (virtual < limit):
            uList[j] += 1


# plot
fig = plt.figure

x = np.linspace(1, physicalSize, physicalSize)

y = [u / 100 for u in uList]
model = np.polyfit(x, y, 1)

predict = (np.poly1d(model))
max = spaceSize
predict(spaceSize)

plt.plot([predict(x_) for x_ in x])

plt.plot(x, y, 'o', alpha=0.1)
plt.ylim(0, 1)
plt.margins(0)
plt.xlabel('Limit')
plt.ylabel('Valid fraction (Average)')
#plt.savefig('valid_fraction.png', dpi=227)
plt.show()
