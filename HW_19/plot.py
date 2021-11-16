import matplotlib.pyplot as plt
import os
import numpy as np
from optparse import OptionParser
import matplotlib

numTrials = 100

parser = OptionParser()

parser.add_option("-l","--numTrials",dest="numTrials",help="set number of trials")

(options,args) = parser.parse_args()

options = vars(options)

numTrials = options["numTrials"]

pageSize = []

for v in [2**x for x in range(0,16)]:
    stream = os.popen(f"./tlb -n {v} -l {numTrials}")
    res = stream.read()
    pageSize.append(res)



resultstatsliste = []

evaluierteList = []

for idx,eintrag in enumerate(pageSize):
    eintrag = eintrag.split(";")
    pageSizeList = []
    for wert in eintrag:
        if wert != ' ':
            wert = int(wert)
            pageSizeList.append(wert)
            #resultstatsliste.append((2**idx,wert))
    resultstatsliste.append(pageSizeList)


for idx,teilliste in enumerate(resultstatsliste):
    teilliste = sorted(teilliste)
    median = np.median(teilliste)
    mean = np.mean([x for x in teilliste if x < 1000],dtype=np.int32)
    evaluierteList.append((2**idx,median,mean))


fig, ax = plt.subplots()
ax.set_title(f"TLB Size Measurement Trials={numTrials}")
ax.set_xscale('log')
ax.set_xlabel("Number of Pages")
ax.set_ylabel("Time per Acess (ns)")
ax.set_xticks(([2**x for x in range(0,16)]))
ax.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
ax.plot([x[0] for x in evaluierteList],[x[1] for x in evaluierteList],'o--',label='med');
ax.plot([x[0] for x in evaluierteList],[x[2] for x in evaluierteList],'o--',label='mean');
ax.legend(['med','mean'])
plt.show()
fig.savefig(f'plot -l{numTrials} (macOS).png')


