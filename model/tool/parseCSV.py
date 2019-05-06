'''
Created By ILMARE
@Date:2019-5-6
'''

import csv
import re

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


if __name__ == "__main__":
    file_path = r"F:\resource_estimation\data\originlog_1557110567.csv"
    output_path = r"F:\resource_estimation\data\data_1557110567.csv"
    try:
        fp = open(file_path, "r")
        reader = csv.reader(fp)
        fp1 = open(output_path, "w", newline="\n")
        writer = csv.writer(fp1)
        row = None
        for line in reader:
            if not checkLine(line):
                if row is not None:
                    list = []
                    for item in row.values():
                        list.append(item[0])
                        list.append(item[1])
                    list.append(end - begin)
                    writer.writerow(list)
                row = {}
                begin = int(line[-1])
                for table in table_name:
                    row[table] = [0, 0]
            row[line[2]][int(line[1])] += 1
            end = int(line[-1])
    except Exception as e:
        print(e)
    finally:
        fp.close()