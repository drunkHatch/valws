import matplotlib.pyplot as plt
import csv
import numpy as np

with open('data.csv','rt') as data:
    reader = csv.reader(data)
    value = []
    for i in reader:
        #print(i)
        #print(type(i[0]))
        data = int(i[0])
        value.append(data)


x1 = range(1,len(value)+1)
y1 = value



plt.plot(x1,y1)
#plt.hist(value,20)

plt.show()

print("done")
