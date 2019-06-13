'''
Created By ILMARE
@Date:2019-5-6
'''

import csv
import re
import os
import numpy as np
from utils import lineutils
from utils import propertiesutils as pu

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
    table_name = ["customer", "district", "history", "item", "new_orders", "order_line", "orders", "stock", "warehouse"]
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
            if not lineutils.checkLine(line):
                if row is not None:
                    list = []
                    for items in row.values():
                        list.append(items[0])
                        list.append(items[1])
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
    reader = pu.configreader(pu.configfile)
    generate_train_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.BP_DATA])