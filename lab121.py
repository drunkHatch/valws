import matplotlib.pyplot as plt
import csv
import numpy as np

with open('data.csv','rt') as data:
    reader = csv.reader(data)
    value = []
    for i in reader:
        #print(i)
        #print(type(i[0]))
        data1 = int(i[0])
        value.append(data1)


x1 = range(1,len(value)+1)
y1 = value



plt.plot(x1,y1)
#plt.hist(value,20)

plt.show()

narray=numpy.array(value)
sum1=narray.sum()
narray2=narray*narray
sum2=narray2.sum()
mean=sum1/len(value)
var=sum2/(len(value)-mean**2)


print("done")
