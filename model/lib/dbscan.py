"""
created by ilmare
@Date 2019-4-16
"""
from domain.bpobject import bp_data
from sklearn.cluster import DBSCAN
from utils import propertiesutils as pu

class DBSCAN_model:
    def __init__(self, eps, min_samples, data_obj):
        self._model = DBSCAN(eps=eps, min_samples=min_samples)
        self._data_obj = data_obj
    def clustering(self):
        self._model = self._model.fit(self._data_obj.train.samples)
    @property
    def labels(self):
        return self._model.fit_predict(self._data_obj.train.samples)

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    obj = bp_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.BP_DATA])
    model = DBSCAN_model(0.3, 20, obj)
    model.clustering()
    labels = model.labels
    result = {}
    for label in labels:
        if result.get(label) is None:
            result[label] = 1
        else:
            result[label] += 1
    print(result)
