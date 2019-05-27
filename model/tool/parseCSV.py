'''
Created By ILMARE
@Date:2019-5-6
'''

import csv
import re
import os
import numpy as np
from tool import sortlib

table_name = ["customer", "district", "history", "item", "new_orders", "order_line", "orders", "stock", "warehouse"]

thread_idx = None
transaction_idx = None

def checkLine(line):
    global thread_idx, transaction_idx
    msg = line[0]
    result = re.findall(r'[0-9]+', msg)
    assert len(result) == 2, Exception("number of parameters is wrong")
    th_idx = result[0]
    tr_idx = result[1]
    if th_idx != thread_idx or tr_idx != transaction_idx:
        thread_idx = th_idx
        transaction_idx = tr_idx
        return False
    return True

'''
针对非并发条件下事务运行log的原始数据处理
'''
def generate_train_data(list_path):
    test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", list_path)
    assert test != None, Exception("path is invaild")
    if list_path[-1] is not "\\" and list_path[-1] is not "/":
        list_path = "{0}/".format(list_path)
    items = os.listdir(list_path)
    train = "{0}train.csv".format(list_path)
    test = "{0}test.csv".format(list_path)
    fp1 = open(train, "w", newline="\n")
    writer = csv.writer(fp1)
    fp2 = open(test, "w", newline="\n")
    writer1 = csv.writer(fp2)
    for item in items:
        filePath = "{0}{1}".format(list_path, item)
        print("parsing file {0}".format(filePath))
        result = re.search("\w+[0-9]+\.csv", filePath)
        if result is None:
            continue
        fp = open(filePath, "r")
        reader = csv.reader(fp)
        row = None
        for line in reader:
            if not checkLine(line):
                if row is not None:
                    list = []
                    for item in row.values():
                        list.append(item[0])
                        list.append(item[1])
                    list.append(end - begin)
                    if np.random.randint(0, 10) <= 6:
                        writer.writerow(list)
                    else:
                        writer1.writerow(list)
                row = {}
                begin = int(line[-1])
                for table in table_name:
                    row[table] = [0, 0]
            row[line[2]][int(line[1])] += 1
            end = int(line[-1])
        fp.close()
    fp1.close()
    fp2.close()


if __name__ == "__main__":
    list_path = r"F:/resource_estimation/data/rnn"
    test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", list_path)
    assert test != None, Exception("path is invaild")
    if list_path[-1] is not "\\" and list_path[-1] is not "/":
        list_path = "{0}/".format(list_path)
    items = os.listdir(list_path)
    for item in items:
        filePath = "{0}{1}".format(list_path, item)
        result = re.search("\w+[0-9]+\.csv", filePath)
        if result is None or item.startswith("dest_origin"):
            continue
        result = re.search("[0-9]+", item)
        if result is None:
            continue
        start_time = int(result.group()) * 1000
        print("parsing file: ", filePath)
        fp = open(filePath, "r")
        reader = csv.reader(fp)
        tmp_lst = list()
        for line in reader:
            line[-1] = int(line[-1]) + start_time
            tmp_lst.append(line)
        tmp_lst = sorted(tmp_lst, key=lambda item: item[-1])
        fp.close()
        fp = open("{0}dest_{1}".format(list_path, item), "w", newline="\n")
        writer = csv.writer(fp)
        for line in tmp_lst:
            writer.writerow(line)
        fp.close()
    items = os.listdir(list_path)
    reader_lst = []
    fps_lst = []
    for item in items:
        filePath = "{0}{1}".format(list_path, item)
        result = re.search("\w+[0-9]+\.csv", filePath)
        if result is None or not item.startswith("dest_origin"):
            continue
        fp = open(filePath, "r")
        reader = csv.reader(fp)
        reader_lst.append(reader)
        fps_lst.append(fp)
    reader = sortlib.mergeSort_1(fps_lst, reader_lst, list_path, 0, len(reader_lst) - 1)
    for idx, fp in enumerate(fps_lst):
        fp.close()
        print("filename", fp.name)
        if idx != len(fps_lst) - 1:
            os.remove(fp.name)
    # generate_train_data(r"F:/resource_estimation/data")