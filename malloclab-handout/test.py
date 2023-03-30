import random
imax = 2
jmax = 16
minsize = 255*4096
maxsize = 256*4096
# step = 1

print(f"0\n{imax*jmax}\n{2*imax*jmax}\n0")

for i in range(imax):
    
    for j in range(jmax):
        size = random.randrange(minsize,maxsize)
        print(f"a {i*jmax+j} {size}")
    list1 = []
    for j in range(jmax):
        list1.append(i*jmax+j)
    random.shuffle(list1)
    for each in list1:
        print(f"f {each}")