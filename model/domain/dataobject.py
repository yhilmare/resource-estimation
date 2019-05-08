'''
Created By ILMARE
@Date 2019-5-7
'''

import csv
import re
import numpy as np
from sklearn.decomposition import PCA
import os

def generate_label(label):
    tmp_label = np.log(int(label))
    result = [0 for _ in range(10)]
    if tmp_label < 2:
        result[0] = 1
        return result
    elif tmp_label > 10:
        result[9] = 1
        return result
    idx = int(tmp_label - 2)
    result[idx] = 1
    return result

class data_obj:
    def __init__(self, data, labels):
        self._data = data
        self._labels = labels
    @property
    def samples(self):
        return self._data
    @property
    def labels(self):
        return self._labels
    def next_batch(self, num):
        high = len(self._data) - num
        idx = np.random.randint(0, high)
        return self._data[idx: idx + num], self._labels[idx: idx + num]


class tran_data:
    def __init__(self, filePath):
        self._filePath = filePath
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", filePath)
        assert test != None, Exception("the path is invaild")
        if filePath[-1] != '\\' and filePath[-1] != "/":
            filePath += "/"
        train_path = "{0}train.csv".format(filePath)
        fp = open(train_path, "r")
        reader = csv.reader(fp)
        _train = []
        _label = []
        for line in reader:
            _train.append(line[0: -1])
            _label.append(generate_label(line[-1]))
        self._train = np.array(_train, dtype=np.float32)
        self._train_label = np.array(_label, dtype=np.float32)
        fp.close()
        test_path = "{0}test.csv".format(filePath)
        fp = open(test_path, "r")
        reader = csv.reader(fp)
        _test = []
        _label = []
        for line in reader:
            _test.append(line[0: -1])
            _label.append(generate_label(line[-1]))
        self._test = np.array(_test, dtype=np.float32)
        self._test_label = np.array(_label, dtype=np.float32)
        fp.close()
        self._train_sample = data_obj(self._train, self._train_label)
        self._test_sample = data_obj(self._test, self._test_label)
    @property
    def train(self):
        return self._train_sample
    @property
    def test(self):
        return self._test_sample
    def pca_samples(self, n_components):
        pca = PCA(n_components=n_components)
        self._train = pca.fit_transform(self._train)
        self._test = pca.fit_transform(self._test)
        self._train_sample = data_obj(self._train, self._train_label)
        self._test_sample = data_obj(self._test, self._test_label)

if __name__ == "__main__":
    obj = tran_data(filePath = r"F:/resource_estimation/data/lr/")
    train, label = obj.test.next_batch(10)
    obj.pca_samples(10)
    train, label = obj.train.next_batch(10)
    print(train.shape)
    print(obj.test.samples.shape)
    print(obj.train.samples.shape)