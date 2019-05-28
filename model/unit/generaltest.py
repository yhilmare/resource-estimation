"""
created by ilmare
@Date 2019-4-16
"""
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import csv
from mpl_toolkits.mplot3d.axes3d import Axes3D
from sklearn.decomposition import PCA

def plot_data(file_path, max_points=500000):
    try:
        fp = open(file_path, "r")
        reader = csv.reader(fp)
        mpl.rcParams["xtick.labelsize"] = 8
        mpl.rcParams["ytick.labelsize"] = 8
        y = []
        for idx, line in enumerate(reader):
            y.append(int(line[-1]))
            if idx >= max_points:
                break
        x = np.arange(0, len(y))
        fig = plt.figure("test")
        ax = fig.add_subplot(111)
        y = np.array(y, dtype=np.float32)
        ax.plot(x, y, '.')
        plt.show()
    except Exception as e:
        print(e)
    finally:
        fp.close()

def pca_plot():
    try:
        fp = open(r"F:\resource_estimation\data\lr\train.csv", "r")
        reader = csv.reader(fp)
        mpl.rcParams["xtick.labelsize"] = 8
        mpl.rcParams["ytick.labelsize"] = 8
        y = []
        for line in reader:
            y.append(line[0: -1])
        y = np.array(y, dtype=np.float32)
        pca = PCA(n_components=3)
        new_y = pca.fit_transform(y)
        fig = plt.figure("test")
        ax = Axes3D(fig)
        ax.plot(new_y[:, 0], new_y[:, 1], new_y[:, 2], '.')
        plt.show()
        print(pca.explained_variance_ratio_)
    except Exception as e:
        print(e)
    finally:
        fp.close()

if __name__ == "__main__":
    try:
        fp = open(r"F:\resource_estimation\data\rnn\train.csv", "r")
        fp1 = open(r"F:\resource_estimation\data\bp1\train.csv", "r")
        reader = csv.reader(fp)
        mpl.rcParams["xtick.labelsize"] = 8
        mpl.rcParams["ytick.labelsize"] = 8
        y = []
        for idx, line in enumerate(reader):
            y.append(int(line[-1]))
            if idx >= 500000:
                break
        reader = csv.reader(fp1)
        y1 = []
        for idx, line in enumerate(reader):
            y1.append(int(line[-1]))
            if idx >= 500000:
                break
        x = np.arange(0, len(y))
        fig = plt.figure("test")
        ax = fig.add_subplot(311)
        y = np.array(y, dtype=np.float32)
        ax.plot(x, y, '.', label="concurrent")
        ax.legend()
        bx = fig.add_subplot(312)
        y1 = np.array(y1, dtype=np.float32)
        bx.plot(x, y1, '.', color="orange", label="non-concurrent")
        bx.legend()
        cx = fig.add_subplot(313)
        cx.plot(x, y, '.', label="concurrent")
        cx.plot(x, y1, '.', color="orange", label="non-concurrent")
        cx.legend()
        plt.show()
    except Exception as e:
        print(e)
    finally:
        fp.close()
        fp1.close()
    # plot_data(r"F:\resource_estimation\data\rnn1\train.csv")