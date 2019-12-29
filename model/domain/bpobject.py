'''
Created By ILMARE
@Date 2019-5-7
'''

import csv
import re
import numpy as np
from sklearn.decomposition import PCA
import os
from utils import propertiesutils as pu

def generate_label(digtal_label, min, max, dest_dim):
    assert dest_dim > 2, Exception("wrong dim")
    step = (max - min) / (dest_dim - 2)
    dest_lst = [0 for _ in range(dest_dim)]
    if digtal_label < min:
        dest_lst[0] = 1
    elif digtal_label > max:
        dest_lst[dest_dim - 1] = 1
    else:
        idx = int((digtal_label - min) // step) + 1
        assert idx >= 0 and idx < dest_dim, Exception("wrong digtal")
        dest_lst[idx] = 1
    return dest_lst

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


class bp_data:
    def __init__(self, filePath, one_hot, min=-1,max=-1,dest_dim=-1):
        self._filePath = filePath
        self._one_hot = one_hot
        self._dest_dim = dest_dim
        self._min = min
        self._max = max
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", filePath)
        assert test != None, Exception("the path is invaild")
        if filePath[-1] != '\\' and filePath[-1] != "/":
            self._filePath += "/"
        assert os.path.exists(self._filePath), Exception("the path is not exist")
        self.load_train()
        self.load_test()
        self.init_samples()
    def load_train(self):
        train_path = "{0}train.csv".format(self._filePath)
        fp = open(train_path, "r")
        reader = csv.reader(fp)
        _train = []
        _label = []
        tmp_min = 10000
        tmp_max = 0
        tmp_count = 0
        count = 0
        for line in reader:
            tmp = int(line[-1])
            if tmp_min > tmp:
                tmp_min = tmp
            if tmp_max < tmp:
                tmp_max = tmp
            tmp_count += tmp
            count += 1
            _train.append(line[0: -1])
            if self._one_hot:
                assert self._dest_dim != -1, Exception("dest_dim can not be -1")
                _label.append(generate_label(np.log(int(line[-1])) if int(line[-1]) != 0 else 0,
                                             self._min, self._max, self._dest_dim))
            else:
                _label.append([np.log(int(line[-1])) if int(line[-1]) != 0 else 0])
        self._train = np.array(_train, dtype=np.float32)
        self._train_label = np.array(_label, dtype=np.float32)
        print("This is load train:", tmp_min, tmp_max, tmp_count, count)
        fp.close()
    def load_test(self):
        test_path = "{0}test.csv".format(self._filePath)
        fp = open(test_path, "r")
        reader = csv.reader(fp)
        _test = []
        _label = []
        tmp_min = 10000
        tmp_max = 0
        tmp_count = 0
        count = 0
        for line in reader:
            tmp = int(line[-1])
            if tmp_min > tmp:
                tmp_min = tmp
            if tmp_max < tmp:
                tmp_max = tmp
            tmp_count += tmp
            count += 1
            _test.append(line[0: -1])
            if self._one_hot:
                assert self._dest_dim != -1, Exception("dest_dim can not be -1")
                _label.append(generate_label(np.log(int(line[-1])) if int(line[-1]) != 0 else 0,
                                             self._min, self._max, self._dest_dim))
            else:
                _label.append([np.log(int(line[-1])) if int(line[-1]) != 0 else 0])
        self._test = np.array(_test, dtype=np.float32)
        self._test_label = np.array(_label, dtype=np.float32)
        print("This is load test:", tmp_min, tmp_max, tmp_count, count)
        fp.close()
    def init_samples(self):
        self._train_sample = data_obj(self._train, self._train_label)
        self._test_sample = data_obj(self._test, self._test_label)
    class bp_data_descriptor:
        def __init__(self, getter):
            self.__getter = getter
        def __get__(self, instance, owner):
            return self.__getter(instance)
    def __min_getter(self):
        return self._min
    min = bp_data_descriptor(__min_getter)
    def __max_getter(self):
        return self._max
    max = bp_data_descriptor(__max_getter)
    def __dest_dim_getter(self):
        return self._dest_dim
    dest_dim = bp_data_descriptor(__dest_dim_getter)
    @property
    def train(self):
        return self._train_sample
    @property
    def test(self):
        return self._test_sample
    @property
    def file_path(self):
        return self._filePath
    @property
    def one_hot(self):
        return self._one_hot
    def pca_samples(self, n_components):
        pca = PCA(n_components=n_components)
        self._train = pca.fit_transform(self._train)
        self._test = pca.fit_transform(self._test)
        self.init_samples()

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    obj = bp_data(filePath=reader[pu.SECTIONS.DATA][pu.OPTIONS.BP_DATA], one_hot=True, min=0, max=9, dest_dim=180)
    train, label = obj.test.next_batch(10)
    obj.pca_samples(10)
    train, label = obj.train.next_batch(100)
    print(label.shape)
    print(obj.test.samples.shape)
    print(obj.train.samples.shape)