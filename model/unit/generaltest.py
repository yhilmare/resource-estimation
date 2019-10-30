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
import re
from utils import propertiesutils as pu

def plot_data(file_path, max_points=500000):
    try:
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", file_path)
        assert test != None, Exception("path is invaild")
        if file_path[-1] is not "\\" and file_path[-1] is not "/":
            file_path = "{0}/".format(file_path)
        fp = open("{0}train.csv".format(file_path), "r")
        reader = csv.reader(fp)
        mpl.rcParams["xtick.labelsize"] = 8
        mpl.rcParams["ytick.labelsize"] = 8
        y = []
        count = 0
        for idx, line in enumerate(reader):
            if int(line[-1]) == 0:
                count += 1
            y.append(np.log(int(line[-1])) if int(line[-1]) != 0 else 0)
            if idx >= max_points:
                break
        print(count / len(y))
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

def pca_plot(filepath):
    try:
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", filepath)
        assert test != None, Exception("path is invaild")
        if filepath[-1] is not "\\" and filepath[-1] is not "/":
            filepath = "{0}/".format(filepath)
        fp = open(r"{0}train.csv".format(filepath), "r")
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
        # ax = fig.add_subplot(111)
        ax.plot(new_y[:, 0], new_y[:, 1], new_y[:, 2],'.')
        plt.show()
        print(pca.explained_variance_ratio_)
    except Exception as e:
        print(e)
    finally:
        fp.close()

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    file_path = reader[pu.SECTIONS.DATA][pu.OPTIONS.BP_DATA]
    pca_plot(file_path)
    # plot_data(file_path)