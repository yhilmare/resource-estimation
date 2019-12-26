'''
Created By ILMARE
@Date:2019-5-8
'''

from lib.bpnn import bp_model
from domain.bpobject import bp_data
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from utils import propertiesutils as pu

class bp_evaluation:
    def __init__(self, model, data_obj):
        self._model = model
        if data_obj.one_hot:
            self._data_obj_digtial = bp_data(data_obj.file_path, one_hot=False)
            self._data_obj_one_hot = data_obj
        else:
            self._data_obj_digtial = data_obj
            self._data_obj_one_hot = None
        dest_dim = self._model.dest_dim;
        self._data_obj_digtial.pca_samples(dest_dim)
        if data_obj.one_hot:
            self._data_obj_one_hot.pca_samples(dest_dim)
        self._model.load()
        self._labels = self._model.predict(self._data_obj_digtial.test.samples)
        mpl.rcParams["xtick.labelsize"] = 8
        mpl.rcParams["ytick.labelsize"] = 8
    def __predict(self):
        if self._data_obj_one_hot is None:
            return self._labels.flatten()
        else:
            return_mat = list();
            min = self._data_obj_one_hot.min
            max = self._data_obj_one_hot.max
            dest_dim = self._data_obj_one_hot.dest_dim
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
            label1 = self._data_obj_digtial.test.labels.flatten()
            label2 = self.__predict()
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
            accuracy = np.mean(np.equal(np.argmax(self._labels, 1),
                                        np.argmax(self._data_obj_one_hot.test.labels, 1)))
        real_label = np.power(np.e, self._data_obj_digtial.test.labels).flatten()
        predict_label = np.power(np.e, self.__predict())
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
        return "Label accuracy: {0:.3f}, Mean Squared Error: {9:.3f}\nDigtial accuracy: {2}->{1:.3f}% | {4}->{3:.3f}% | {6}->{5:.3f}% | {8}->{7:.3f}%".format(
            accuracy, info_dict[0.1] * 100, 0.1, info_dict[0.2] * 100, 0.2, info_dict[0.3] * 100, 0.3, info_dict[0.35] * 100, 0.35, squared_error_ratio)
    def __str__(self):
        return self.statistics_info()

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    obj = bp_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.BP_DATA],
                  one_hot=True, min=7, max=11, dest_dim=102)
    model = bp_model(batch_size=256,
                     learning_rate=0.05,
                     iterator_num=100000, data_obj=obj, pca_dim=8,
                     model_path=reader[pu.SECTIONS.MODEL][pu.OPTIONS.BP_MODEL])
    test_obj = bp_evaluation(model, obj)
    print(test_obj)
    test_obj.plot()
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
