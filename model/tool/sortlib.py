'''
Created By ILMARE
@Date 2019-5-27
'''

import csv
import uuid

__all__ = ["mergeSort"]

def mergeSort(fiel_list):
    pass

def mergeSort_1(fp_lst, lst, lst_path, begin, end):
    if begin < end:
        mid = (begin + end) // 2
        reader1 = mergeSort_1(fp_lst, lst, lst_path, begin, mid)
        reader2 = mergeSort_1(fp_lst, lst, lst_path, mid + 1, end)
        if reader1 is not None:
            if reader2 is not None:
                return merge(fp_lst, lst_path, reader1, reader2)
            else:
                return merge(fp_lst, lst_path, reader1, lst[end])
        else:
            if reader2 is not None:
                return merge(fp_lst, lst_path, lst[begin], reader2)
            else:
                return merge(fp_lst, lst_path, lst[begin], lst[end])
    return None

def merge(fp_lst, lst_path, reader1, reader2):
    filePath = "{0}{1}.csv".format(lst_path, uuid.uuid4())
    fp = open(filePath, "w", newline="\n")
    writer = csv.writer(fp)
    try:
        first = reader1.__next__()
    except StopIteration:
        return finalMerge(None, reader2, writer, fp, fp_lst, filePath)
    try:
        second = reader2.__next__()
    except StopIteration:
        return finalMerge(first, reader1, writer, fp, fp_lst, filePath)
    while True:
        num_1 = int(first[-1])
        num_2 = int(second[-1])
        if num_1 < num_2:
            writer.writerow(first)
            try:
                first = reader1.__next__()
            except StopIteration:
                return finalMerge(second, reader2, writer, fp, fp_lst, filePath)
        else:
            writer.writerow(second)
            try:
                second = reader2.__next__()
            except StopIteration:
                return finalMerge(first, reader1, writer, fp, fp_lst, filePath)

def finalMerge(row, reader, writer, fp, fp_lst, file_path):
    if row is not None:
        writer.writerow(row)
    for line in reader:
        writer.writerow(line)
    fp.close()
    fp1 = open(file_path, "r")
    fp_lst.append(fp1)
    return csv.reader(fp1)