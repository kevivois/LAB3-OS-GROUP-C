import random; # using random lib
import numpy; # using numpy lib
from scipy.stats import expon
import csv;
def gen_csv(length):
    data = []
    for i in range(0,length):
        line = []
        # priority
        line.append(i+1)
        line.append(get_arrival_time(length))
        line.append(get_execution_time(length))
        line.append(rdm_priority())
        data.append(line)
        # tete de neuille
    filename = "cmake-build-debug/data.csv"
    with open(filename, mode='w', newline='') as file:
        for row in data:
            file.write(" ".join(str(r) for r in row) + "\n")
    print("gen_csv")
def rdm_priority():
    priorities = [1, 2, 3]
    weigths = [0.05,0.25,0.7]
    return random.choices(priorities,weigths)[0]
def get_arrival_time(rate=5):
    return round(numpy.random.poisson(lam=rate))
def get_execution_time(lgth,scale=20,sm=10):
    return round(expon.rvs(scale,lgth) + sm)

gen_csv(15)
