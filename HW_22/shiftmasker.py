"""
I  7ff80b0520b7,3
I  7ff80b0520ba,3
I  7ff80b0520bd,3
I  7ff80b0520c0,3
I  7ff80b0520c3,3
I  7ff80b0520c6,5
 S 1048b8798,8
I  7ff80b0520fc,5
I  7ff80b052101,3
I  7ff80b052104,2
"""

import numpy as np




#def fill(lst):
#   for e in lst:
#       e = '0' + e


lines = ""

with open('adresses.txt') as f:
    lines = f.readlines()

lines2 = []

for e in lines:
    for idx,b in enumerate(e):
        if b.isdigit():
            lines2.append(int(e[idx:].split(",")[0].replace("\n","").upper(),16))
            break    

"""
2**12 => 4096 = PageSize
32 bit physicalAddress
32 - 12 => physicalPageNum
| 20 bit PPN | 12 bit Offset
"""


print(len("11111111111100000001011000001010010000010010111"))

#lines2 = fill(lines2)


for e in lines2:
    print(bin(e),'-')
    print(bin(e >> 12)) 
    pass








    

