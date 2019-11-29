'''
Created at 2019-11-28
@Author: ILMARE
'''

from matplotlib import pyplot as plt
import matplotlib as mpl
import numpy as np
from utils import propertiesutils as pu
import re
import os
import csv

class diskUI:
    def __init__(self, pre_path, xtickLableSize=8, ytickLabelSize=8):
        self._pre_path = pre_path
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", pre_path)
        assert test != None, Exception("path is invaild")
        if pre_path[-1] is not "\\" and pre_path[-1] is not "/":
            self._pre_path = "{0}/".format(pre_path)
        mpl.rcParams["xtick.labelsize"] = xtickLableSize
        mpl.rcParams["ytick.labelsize"] = ytickLabelSize
    def __read_from_file(self, fileName):
        try:
            fp = open("{0}{1}".format(self._pre_path, fileName), "r")
            reader = csv.reader(fp)
            tmp_lst = []
            for item in reader:
                tmp_lst.append(item)
            tmp_lst = np.array(tmp_lst, dtype=np.float64)
            return tmp_lst
        except Exception as e:
            print(e)
        finally:
            fp.close()
    def plotSingle(self, type="record", fileName=None, labels=None):
        timestamp = 0
        if fileName is None:
            for item in os.listdir(self._pre_path):
                if re.match(".*{0}\w+\.csv".format(type), item) is None:
                    continue
                result = re.search(r"[0-9]+", item)
                if timestamp < int(result.group()):
                    timestamp = int(result.group())
                    fileName = item
        figure = plt.figure("disk-fig")
        ax = figure.add_subplot(111)
        tmp_lst = self.__read_from_file(fileName)
        ax.grid(True)
        for index in range(tmp_lst.shape[1] - 1):
            if labels is not None:
                ax.plot(tmp_lst[:, 0], tmp_lst[:, index + 1], label=labels[index])
            else:
                ax.plot(tmp_lst[:, 0], tmp_lst[:, index + 1])
        ax.legend()
        plt.show()
    def plotContrast(self, file1, file2, labels1=None, labels2=None):
        result = re.search(r"[0-9]+", file1)
        stamp1 = int(result.group())
        result = re.search(r"[0-9]+", file2)
        stamp2 = int(result.group())
        figure = plt.figure("disk-contrast")
        plt.xlabel("Time Stamp")
        plt.xlim(1574952024638729 - 10000000, 1574952024638729 + 1000000000)
        ax = figure.add_subplot(111)
        plot_list = self.__read_from_file(file1)
        ax.grid(True)
        ax.set_ylabel("Buffer Page")
        for index in range(plot_list.shape[1] - 1):
            if labels1 is not None:
                ax.plot(stamp1 + plot_list[:, 0], plot_list[:, index + 1], label=labels1[index])
            else:
                ax.plot(stamp1 + plot_list[:, 0], plot_list[:, index + 1])
        ax.legend()
        bx = ax.twinx()
        bx.set_ylabel("TPS")
        plot_list = self.__read_from_file(file2)
        bx.grid(True)
        for index in range(plot_list.shape[1] - 1):
            if labels1 is not None:
                bx.plot(stamp2 + plot_list[:, 0], plot_list[:, index + 1], color="olive", label=labels2[index])
            else:
                bx.plot(stamp2 + plot_list[:, 0], plot_list[:, index + 1])
        bx.legend()
        plt.show()
    def plotTotal(self, type="record", labels=None):
        plotList = []
        for item in os.listdir(self._pre_path):
            if re.match(".*{0}\w+\.csv".format(type), item) is None:
                continue
            result = re.search(r"[0-9]+", item)
            timestamp = int(result.group())
            try:
                fp = open("{0}{1}".format(self._pre_path, item), "r")
                reader = csv.reader(fp)
                for line in reader:
                    line[0] = int(line[0]) + timestamp
                    plotList.append(line)
            except Exception as e:
                print(e)
            finally:
                fp.close()
        plotList = sorted(plotList, reverse=True, key=lambda item: item[0])
        plotList = np.array(plotList, dtype=np.float64)
        fig = plt.figure("disk_fig")
        ax = fig.add_subplot(111)
        ax.grid(True)
        for index in range(plotList.shape[1] - 1):
            if labels is not None:
                ax.plot(plotList[:, 0], plotList[:, index + 1], label=labels[index])
            else:
                ax.plot(plotList[:, 0], plotList[:, index + 1])
        ax.legend()
        plt.show()

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    obj = diskUI(reader[pu.SECTIONS.DATA][pu.OPTIONS.DISK_DATA])
    obj.plotContrast("record_1574952024649769.csv", "tps_1574952024638729.csv", ["clean buffer", "dirties buffer", "total buffer"], ["live TPS"])
    # obj.plotTotal(type="record", labels=["clean buffer", "dirties buffer", "total buffer"])
    # obj.plotSingle(type="record", labels=["clean buffer", "dirties buffer", "total buffer"])
