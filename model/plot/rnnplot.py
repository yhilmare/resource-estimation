'''
Created By IL MARE
@Date 2019-6-13
'''
from matplotlib import pyplot as plt
import matplotlib as mpl
import numpy as np


if __name__ == "__main__":
    mpl.rcParams["font.size"] = 16
    mpl.rcParams["xtick.labelsize"] = 16
    mpl.rcParams["ytick.labelsize"] = 16
    figure = plt.figure("disk-fig")
    ax = figure.add_subplot(111)
    ax.grid(True)
    y = np.array([77.253, 140.449, 171.248, 164.269, 177.426, 193.405, 170.153, 293.676, 346.198], dtype=np.float32)
    y1 = np.array([81.3725, 130.792, 154.164, 138.523, 161.094, 175.482, 145.928, 252.921, 321.555], dtype=np.float32)
    err = np.abs(y - y1) / y
    print(np.std(err), np.mean(err))
    x = np.arange(0.1, 1, step=0.1)
    print(x)
    ax.plot(x, y, "-", label="actual")
    ax.plot(x, y1, "*", label="predict")
    ax.set_xlabel("Ratio of New Order Trans")
    ax.set_ylabel("Avg of pages flushed / sec")
    ax.legend()
    plt.show()