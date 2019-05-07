"""
created by ilmare
@Date 2019-4-16
"""
from domain.dataobject import tran_data
from sklearn.decomposition import PCA
from sklearn.cluster import DBSCAN
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d.axes3d import Axes3D

if __name__ == "__main__":
    obj = tran_data(r"F:\resource_estimation\data\train.csv")
    train = obj.train
    labels = DBSCAN(eps=0.3, min_samples=10).fit_predict(train)
    new_x = PCA(n_components=3).fit_transform(train)
    result = {}
    for label in labels:
        if result.get(label) is None:
            result[label] = 0
        else:
            result[label] += 1
    print(result)
