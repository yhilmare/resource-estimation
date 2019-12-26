'''
Created By IL MARE
@Date 2019-12-19
'''

from lib import lstm
from domain.lstmobject import lstm_data
from utils import propertiesutils as pu
from matplotlib import pyplot as plt
import matplotlib as mpl
import numpy as np

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    model_path = reader[pu.SECTIONS.MODEL][pu.OPTIONS.RNN_MODEL]
    obj = lstm_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.RNN_DATA], 25)
    print(obj.test.labels.shape)
    dest = np.reshape(obj.test.labels, newshape=[-1])
    start = 4
    end = 11
    num = 2000
    step = (end - start) / num
    count = [0 for _ in range(num)]
    x = np.arange(start, end, step)
    print(len(count), len(x))
    # dest = np.random.normal(6.5, 0.1, num)
    for item in dest:
        index = int((item - (start - step / 2.0)) // step)
        if index < 0 or index >= num:
            continue
        count[index] += 1
    mpl.rcParams["font.size"] = 16
    mpl.rcParams["xtick.labelsize"] = 16
    mpl.rcParams["ytick.labelsize"] = 16
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_ylabel("Count")
    ax.set_xlabel("Number")
    ax.plot(x, count, '-', label="count")
    ax.grid(True)
    plt.show()