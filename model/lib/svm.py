'''
Created By ILMARE
@Date 2019-12-27
'''
from sklearn import svm
import numpy as np
from domain.bpobject import bp_data
from utils import propertiesutils as pu
from matplotlib import pyplot as plt
from sklearn.externals import joblib

class svm_model:
    def __init__(self, data_obj, pca_dim, kernal="rbf"):
        self._kernal = kernal
        self._data = data_obj
        self._data.pca_samples(pca_dim)
    @property
    def dest_dim(self):
        return self._dest_dim
    def define_model(self):
        self._svc = svm.SVR(kernel=self._kernal, max_iter=10000)
    def train(self):
        self.define_model()
        self._svc.fit(self._data.train.samples, self._data.train.labels.flatten())
        joblib.dump(self._svc, "/home/ilmare/Desktop/resource_estimation/model/svm/svm_rnn_rbf.pkl")
    def load(self):
        self._svc = joblib.load("/home/ilmare/Desktop/resource_estimation/model/svm/svm_rnn_rbf.pkl")
    def test(self):
        print("accuracy:", self._svc.score(self._data.test.samples, self._data.test.labels.flatten()))
    def predict(self, sample):
        return self._svc.predict(sample)
    def plot(self, sort=False):
        if sort:
            label1 = sorted(self._data.test.labels.flatten())
            label2 = sorted(self.predict(self._data.test.samples))
        else:
            label1 = self._data.test.labels.flatten()
            label2 = self.predict(self._data.test.samples)
        fig = plt.figure()
        ax = fig.add_subplot(311)
        ax.plot(np.arange(0, len(label1)), label1, '.' if not sort else "-", label="real execution time")
        ax.legend()
        bx = fig.add_subplot(312)
        bx.plot(np.arange(0, len(label2)), label2, '.' if not sort else "-", color="darkorange", label="predictive execution time")
        bx.legend()
        cx = fig.add_subplot(313)
        cx.plot(np.arange(0, len(label1)), label1, '.' if not sort else "-", label="real execution time")
        cx.plot(np.arange(0, len(label2)), label2, '.' if not sort else "-", color="darkorange", label="predictive execution time")
        cx.legend()
        plt.show()
    def statistics_info(self):
        real_label = np.power(np.e, self._data.test.labels).flatten()
        predict_label = np.power(np.e, self.predict(self._data.test.samples))
        squared_error_ratio = np.mean(np.power((real_label - predict_label), 2))
        err_ratio = sorted(np.abs(predict_label - real_label) / real_label)
        length = len(err_ratio)
        info_dict = dict()
        for idx, num in enumerate(err_ratio):
            if num > 0.1 and info_dict.get(0.1) is None:
                info_dict[0.1] = (idx + 1) / length
            if num > 0.2 and info_dict.get(0.2) is None:
                info_dict[0.2] = (idx + 1) / length
            if num > 0.3 and info_dict.get(0.3) is None:
                info_dict[0.3] = (idx + 1) / length
            if num > 0.35 and info_dict.get(0.35) is None:
                info_dict[0.35] = (idx + 1) / length
        return "Mean Squared Error: {8:.3f}\nDigtial accuracy: {1}->{0:.3f}% | {3}->{2:.3f}% | {5}->{4:.3f}% | {7}->{6:.3f}%".format(
            info_dict[0.1] * 100, 0.1, info_dict[0.2] * 100, 0.2, info_dict[0.3] * 100, 0.3, info_dict[0.35] * 100, 0.35, squared_error_ratio)
    def __str__(self):
        return self.statistics_info()

if __name__ == "__main__":
    # X = np.array([[-1, -1], [-2, -1], [1, 1], [2, 1]])
    # y = np.array([1.00001, 1, 2, 2])
    # clf = svm.SVR()
    # clf.fit(X, y)
    # print(clf.predict([[-0.8, -1]]))
    reader = pu.configreader(pu.configfile)
    obj = bp_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.RNN_DATA], one_hot=False)
    model = svm_model(obj, 11)
    model.train()
    # model.load()
    # model.test()
    print(model)