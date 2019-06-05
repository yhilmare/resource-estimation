'''
Created By ILMARE
@Date 2019-5-27
'''

import csv
import re
import os
from utils import sortutils, lineutils

'''
针对多个并发事务执行log根据其执行时间先后归并排序
'''
def mergeOriginLog(list_path):
    test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", list_path)
    assert test != None, Exception("path is invaild")
    if list_path[-1] is not "\\" and list_path[-1] is not "/":
        list_path = "{0}/".format(list_path)
    items = os.listdir(list_path)
    f_list = []
    for item in items:
        filePath = "{0}{1}".format(list_path, item)
        result = re.search("\w+[0-9]+\.csv", filePath)
        if result is None or item.startswith("dest_origin"):
            continue
        result = re.search("[0-9]+", item)
        start_time = int(result.group())
        print("ordering file: ", filePath)
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
        f_list.append(fp.name)
        fp.close()
    return sortutils.mergeSort(list_path, f_list, key=lambda item: int(item[-1]))

'''
得出rnn模型下的训练数据（临时）以及测试数据（临时）
'''
def generate_temp_samples(pre_path, file_name):
    test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", pre_path)
    assert test != None, Exception("path is invaild")
    if pre_path[-1] is not "\\" and pre_path[-1] is not "/":
        pre_path = "{0}/".format(pre_path)
    fp = open(file_name, "r")
    reader = csv.reader(fp)
    train_fp = open(r"{0}{1}".format(pre_path, "train_pre.csv"), "w", newline="\n")
    test_fp = open(r"{0}{1}".format(pre_path, "test_pre.csv"), "w", newline="\n")
    train_writer = csv.writer(train_fp)
    test_writer = csv.writer(test_fp)
    last = 0
    for idx, line in enumerate(reader):
        row = list(line)
        row[-1] = int(row[-1]) - last
        if idx <= 1350000:
            train_writer.writerow(row)
        else:
            test_writer.writerow(row)
        last = int(line[-1])
    train_fp.close()
    test_fp.close()
    fp.close()

'''
产生正式的训练数据和正式的测试数据
'''

def generate_samples(pre_path):
    test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", pre_path)
    assert test != None, Exception("path is invaild")
    if pre_path[-1] is not "\\" and pre_path[-1] is not "/":
        pre_path = "{0}/".format(pre_path)
    def sub_parse(file_path, dest_path):
        fp = open(file_path, "r")
        src_reader = csv.reader(fp)
        fp1 = open(dest_path, "w", newline="\n")
        dest_reader = csv.writer(fp1)
        table_name = ["customer", "district", "history", "item", "new_orders", "order_line", "orders", "stock",
                      "warehouse"]
        print("parsing file: ", file_path)
        for line in src_reader:
            execution_time = int(line[-1])
            thread_id = line[0][0: line[0].index("@")]
            row = {}
            for name in table_name:
                row[name] = [0, 0]
            row[line[2]][int(line[1])] += int(line[3])
            row_list = [thread_id]
            for num1, num2 in row.values():
                row_list.append(num1)
                row_list.append(num2)
            row_list.append(execution_time)
            dest_reader.writerow(row_list)
        print("Done: ", file_path)
        fp.close()
        fp1.close()
    sub_parse("{0}{1}".format(pre_path, "train_pre.csv"), "{0}{1}".format(pre_path, "train.csv"))
    sub_parse("{0}{1}".format(pre_path, "test_pre.csv"), "{0}{1}".format(pre_path, "test.csv"))


if __name__ == "__main__":
    pre_path = r"F:/resource_estimation/data/rnn/"
    order_path = mergeOriginLog(pre_path)
    generate_temp_samples(pre_path, order_path)
    generate_samples(pre_path)


