'''
Created By ILMARE
@Date 2019-6-9
'''

from lib import lstm
from domain.lstmobject import lstm_data
from utils import propertiesutils as pu
from matplotlib import pyplot as plt
import matplotlib as mpl
import numpy as np

class lstm_evaluation:
    def __init__(self, model, data_obj):
        self._model = model
        if data_obj.one_hot:
            self._data_obj_digtial = lstm_data(data_obj.file_path, 25, False)
            self._data_obj_one_hot = data_obj
        else:
            self._data_obj_digtial = data_obj
            self._data_obj_one_hot = None
        self._model.load_model()
        self._labels, _ = self._model.test()
        mpl.rcParams["xtick.labelsize"] = 8
        mpl.rcParams["ytick.labelsize"] = 8
    def __predict(self):
        if self._data_obj_one_hot is None:
            return self._labels.flatten()
        else:
            return_mat = list();
            min = self._data_obj_one_hot.min
            max = self._data_obj_one_hot.max
            dest_dim = self._data_obj_one_hot.label_dim
            step = (max - min) / dest_dim
            for item in np.argmax(self._labels, 1):
                if item == 0:
                    return_mat.append(np.random.normal(min / 2.0, min / 2.0))
                elif item == dest_dim - 1:
                    return_mat.append(np.random.normal((max + max + 2.5) / 2.0, (2.5) / 2.0))
                else:
                    low = min + item * step
                    high = low + step
                    return_mat.append(np.random.normal((low + high) / 2.0, (high - low) / 2.0))
            return np.array(return_mat, dtype=np.float32)
    def plot(self, sort=False):
        if sort:
            label1 = sorted(self._data_obj_digtial.test.labels.flatten())
            label2 = sorted(self.__predict())
        else:
            label2 = self.__predict()
            label1 = self._data_obj_digtial.test.labels.flatten()
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
        if self._data_obj_one_hot is None:
            accuracy = -1
        else:
            y_one_hot = np.reshape(self._data_obj_one_hot.test.labels, self._labels.shape)
            accuracy = np.mean(np.equal(np.argmax(self._labels, 1),
                                        np.argmax(y_one_hot, 1)))
        real_label = np.power(np.e, self._data_obj_digtial.test.labels).flatten()
        predict_label = np.power(np.e, self.__predict())
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
        return "Label accuracy: {0:.3f}\nDigtial accuracy: {2}->{1:.3f}% | {4}->{3:.3f}% | {6}->{5:.3f}% | {8}->{7:.3f}%".format(
            accuracy, info_dict[0.1] * 100, 0.1, info_dict[0.2] * 100, 0.2, info_dict[0.3] * 100, 0.3, info_dict[0.35] * 100, 0.35)
    def __str__(self):
        return self.statistics_info()

def train_model():
    reader = pu.configreader(pu.configfile)
    model_path = reader[pu.SECTIONS.MODEL][pu.OPTIONS.RNN_MODEL]
    obj = lstm_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.RNN_DATA], 25,
                    True, min=0, max=11, label_dim=180)
    obj.pca_samples(8)
    model = lstm.lstm_model(hidden_size=128, num_layer=2, data_obj=obj,
                       keep_prob=0.8, l_rate=0.005, max_step=5000,
                       save_path=model_path, batch_size=256)
    model.train()

def test_model():
    reader = pu.configreader(pu.configfile)
    model_path = reader[pu.SECTIONS.MODEL][pu.OPTIONS.RNN_MODEL]
    obj = lstm_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.RNN_DATA], 25,
                    True, min=0, max=11, label_dim=180)
    obj.pca_samples(8)
    model = lstm.lstm_model(hidden_size=128, num_layer=2, data_obj=obj,
                       keep_prob=0.8, l_rate=0.005, max_step=5000,
                       save_path=model_path, batch_size=obj.test.samples.shape[0])
    model.load_model()
    pre, _ = model.test()
    print(pre.shape)

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    model_path = reader[pu.SECTIONS.MODEL][pu.OPTIONS.RNN_MODEL]
    obj = lstm_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.RNN_DATA], 25,
                    True, min=2, max=6, label_dim=102)
    # dest = np.reshape(obj.test.labels, newshape=[-1, 102])
    # dest = np.argmax(dest, 1)
    # result = dict()
    # for item in dest:
    #     if result.get(item, -1) == -1:
    #         result[item] = 1
    #     else:
    #         result[item] = result[item] + 1
    # print(result)
    # count = 0
    # for value in result.values():
    #     count += value
    # print(count)
    # count_1 = 0
    # for key, value in result.items():
    #     print(key, ": ", value / count)
    #     result[key] = value / count
    #     count_1 += (value / count)
    # print(count_1)
    obj.pca_samples(11)
    model = lstm.lstm_model(hidden_size=128, num_layer=2, data_obj=obj,
                            keep_prob=0.8, l_rate=0.005, max_step=5000,
                            save_path=model_path, batch_size=obj.test.samples.shape[0])
    test_obj = lstm_evaluation(model, obj)
    print(test_obj)
    test_obj.plot(True)