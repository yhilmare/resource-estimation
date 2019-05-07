'''
Created By ILMARE
@Date 2019-5-7
'''

import csv
import re
import numpy as np
from sklearn.decomposition import PCA

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


class tran_data:
    def __init__(self, filePath):
        self._filePath = filePath
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/].+.csv$", filePath)
        assert test != None, Exception("the path is invaild")
        fp = open(filePath, "r")
        reader = csv.reader(fp)
        _train = []
        _label = []
        for line in reader:
            _train.append(line[0: -1])
            _label.append(generate_label(line[-1]))
        self._train = np.array(_train, dtype=np.float32)
        self._label = np.array(_label, dtype=np.float32)
        fp.close()
    @property
    def train(self):
        return self._train
    @property
    def labels(self):
        return self._label
    def pca_samples(self, n_components):
        pca = PCA(n_components=n_components)
        self._train = pca.fit_transform(self._train)
    def next_batch(self, num):
        high = len(self._train) - num
        idx = np.random.randint(0, high)
        return self._train[idx: idx + num], self._label[idx: idx + num]

if __name__ == "__main__":
    obj = tran_data(filePath = r"F:\resource_estimation\data\train.csv")
    train, label = obj.next_batch(10)
    obj.pca_samples(10)
    train, label = obj.next_batch(10)
    print(train.shape)