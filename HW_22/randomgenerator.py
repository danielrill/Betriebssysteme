import numpy as np
import matplotlib.pyplot as plt
from optparse import OptionParser

range_ = 10
probability = 1


parser = OptionParser()

parser.add_option("-r","--range",dest="range",help="range of numbers")
parser.add_option("-p","--probability",dest="prob",help="probability of locality")

(options,args) = parser.parse_args()

options = vars(options)

range_ = int(options["range"])
probability = float(options["prob"])



# 100% -> 0/1
# 0% -> 0/20



numbers = np.random.normal(0,(1.5/probability),size=(range_))

numbers = [np.abs(int(x)) for x in numbers]

out = ""

for idx,n in enumerate(numbers):
    out += str(numbers[idx])
    out += ","

print(out)


