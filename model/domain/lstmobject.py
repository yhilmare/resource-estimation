'''
Created By ILMARE
@Date 2018-5-28
'''

import numpy as np
import re
import csv
from sklearn.decomposition import PCA
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
    def __init__(self, samples, labels, tran_size):
        self._samples = samples
        self._labels = labels
        self._tran_size = tran_size
        line_count = self._samples.shape[0]
        self._samples = self._samples[0: line_count - line_count % tran_size]
        self._labels = self._labels[0: line_count - line_count % tran_size]
        dim_one = self._samples.shape[0]
        self._samples = np.reshape(self._samples, newshape=(
            dim_one // tran_size, tran_size, self._samples.shape[1]))
        self._labels = np.reshape(self._labels, newshape=(
            dim_one // tran_size, tran_size, self._labels.shape[1]))
    @property
    def samples(self):
        return self._samples
    @property
    def labels(self):
        return self._labels
    def next_batch(self, batch_size):
        total_batch = self._samples.shape[0]
        idxs = np.random.choice(range(total_batch), batch_size, replace=False)
        return self._samples[idxs], self._labels[idxs]

class lstm_data:
    def __init__(self, pre_path, transaction_size,
                 one_hot=False, min=-1, max=-1, label_dim=-1):
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", pre_path)
        assert test != None, Exception("path is invaild")
        if pre_path[-1] is not "\\" and pre_path[-1] is not "/":
            pre_path = "{0}/".format(pre_path)
        self._pre_path = pre_path
        self._train_path = "{0}{1}".format(pre_path, "train.csv")
        self._test_path = "{0}{1}".format(pre_path, "test.csv")
        self._tran_size = transaction_size
        if one_hot:
            assert min != -1 and max != -1 and label_dim != -1, Exception("dest_dim can not be -1")
        self._min = min
        self._max = max
        self._label_dim = label_dim
        self._one_hot = one_hot
        def parse_data(path):
            samples = []
            labels = []
            fp = open(path, "r")
            reader = csv.reader(fp)
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
                samples.append(line[0: -1])
                if self._one_hot:
                    labels.append(generate_label(np.log(int(line[-1])) if int(line[-1]) is not 0 else 0,
                                                 self._min, self._max, self._label_dim))
                else:
                    self._label_dim = 1
                    labels.append([np.log(int(line[-1])) if int(line[-1]) is not 0 else 0])
            fp.close()
            print("This is load test:", tmp_min, tmp_max, tmp_count, count, tmp_count / count)
            return np.array(samples, dtype=np.float32), np.array(labels, dtype=np.float32)
        self._train, self._train_labels = parse_data(self._train_path)
        self._test, self._test_labels = parse_data(self._test_path)
        self._dim = self._train.shape[1]
        self.init_samples()
    def init_samples(self):
        self._train_samples = data_obj(self._train, self._train_labels, self._tran_size)
        self._test_samples = data_obj(self._test, self._test_labels, self._tran_size)
    @property
    def file_path(self):
        return self._pre_path
    @property
    def one_hot(self):
        return self._one_hot
    class descriptor:
        def __init__(self, get):
            self.__get = get
        def __get__(self, instance, owner):
            return self.__get(instance)
    def __min_getter(self):
        return self._min
    min = descriptor(__min_getter)
    def __max_getter(self):
        return self._max
    max = descriptor(__max_getter)
    def __dest_dim_getter(self):
        return self._label_dim
    label_dim = descriptor(__dest_dim_getter)
    def __get_train(self):
        return self._train_samples
    train = descriptor(__get_train)
    def __get_test(self):
        return self._test_samples
    test = descriptor(__get_test)
    @property
    def transaction_size(self):
        return self._tran_size
    @property
    def sample_dim(self):
        return self._dim
    def pca_samples(self, n_components):
        pca = PCA(n_components=n_components)
        self._train = pca.fit_transform(self._train)
        self._test = pca.fit_transform(self._test)
        self._dim = n_components
        self.init_samples()

class lstm_data_core:
    def __init__(self, pre_path, transaction_size,
                 one_hot=False, min=-1, max=-1, label_dim=-1):
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", pre_path)
        assert test != None, Exception("path is invaild")
        if pre_path[-1] is not "\\" and pre_path[-1] is not "/":
            pre_path = "{0}/".format(pre_path)
        self._pre_path = pre_path
        self._train_path = "{0}{1}".format(pre_path, "train.csv")
        self._test_path = "{0}{1}".format(pre_path, "test.csv")
        self._tran_size = transaction_size
        if one_hot:
            assert min != -1 and max != -1 and label_dim != -1, Exception("dest_dim can not be -1")
        self._min = min
        self._max = max
        self._label_dim = label_dim
        self._one_hot = one_hot
        def parse_data(path):
            samples = []
            labels = []
            fp = open(path, "r")
            reader = csv.reader(fp)
            for line in reader:
                tmp_log = np.log(int(line[-1])) if int(line[-1]) is not 0 else 0
                tmp_lst = generate_label(tmp_log, self._min, self._max, self._label_dim)
                if tmp_lst[0] == 1 or tmp_lst[-1] == 1:
                    continue
                samples.append(line[0: -1])
                if self._one_hot:
                    labels.append(tmp_lst)
                else:
                    labels.append([tmp_log])
            fp.close()
            return np.array(samples, dtype=np.float32), np.array(labels, dtype=np.float32)
        self._train, self._train_labels = parse_data(self._train_path)
        self._test, self._test_labels = parse_data(self._test_path)
        self._dim = self._train.shape[1]
        self.init_samples()
    def init_samples(self):
        self._train_samples = data_obj(self._train, self._train_labels, self._tran_size)
        self._test_samples = data_obj(self._test, self._test_labels, self._tran_size)
    @property
    def file_path(self):
        return self._pre_path
    @property
    def one_hot(self):
        return self._one_hot
    class descriptor:
        def __init__(self, get):
            self.__get = get
        def __get__(self, instance, owner):
            return self.__get(instance)
    def __min_getter(self):
        return self._min
    min = descriptor(__min_getter)
    def __max_getter(self):
        return self._max
    max = descriptor(__max_getter)
    def __dest_dim_getter(self):
        return self._label_dim
    label_dim = descriptor(__dest_dim_getter)
    def __get_train(self):
        return self._train_samples
    train = descriptor(__get_train)
    def __get_test(self):
        return self._test_samples
    test = descriptor(__get_test)
    @property
    def transaction_size(self):
        return self._tran_size
    @property
    def sample_dim(self):
        return self._dim
    def pca_samples(self, n_components):
        pca = PCA(n_components=n_components)
        self._train = pca.fit_transform(self._train)
        self._test = pca.fit_transform(self._test)
        self._dim = n_components
        self.init_samples()

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    obj = lstm_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.RNN_DATA], 25,
                    False, min=0, max=20, label_dim=180)
    labels = obj.test.labels
    print(obj.test.labels.shape)
    print(obj.train.samples.shape)
    total = labels.shape[0] * labels.shape[1]
    count = 0;
    for num in labels.flatten():
        if num == 0:
            count += 1
    print(count / total)