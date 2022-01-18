def clac(adress,disknum):
    disk = adress % disknum
    offset = adress / disknum
    print(f"Disk is:\t{disk}|Offset is:\t{int(offset)}")

disks= int(input("Disks\t"))
while 1:
    addr = int(input("Adress\t"))
    clac(addr,disks)
