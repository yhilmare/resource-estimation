'''
Created By ILMARE
@Date:2019-5-8
'''

from lib.lr import lr_model
from domain.dataobject import tran_data
import matplotlib.pyplot as plt
import os
import numpy as np

if __name__ == "__main__":
    obj = tran_data(r"F:/resource_estimation/data/lr/")
    model = lr_model(batch_size=256,
                     learning_rate=0.05,
                     iterator_num=30000, data_obj=obj,
                     model_path=r'F:/resource_estimation/model/lr/')
    model.load()
    labels = model.preidct(obj.test.samples)
    y = []
    for item in np.argmax(labels, 1):
        if item == 0:
            y.append(np.random.normal(1, 1))
        elif item == 9:
            y.append(np.random.normal(11, 1))
        else:
            y.append(np.random.normal(item + 1 + 0.5, 0.5))
    obj1 = tran_data(r"F:/resource_estimation/data/lr/", one_hot=False)
    y1 = obj1.test.labels
    fig = plt.figure("test")
    ax = fig.add_subplot(111)
    ax.plot(np.arange(0, len(y1)), y1, '.')
    ax.plot(np.arange(0, len(y)), y, '.')
    plt.show()