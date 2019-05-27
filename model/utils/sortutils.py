'''
Created By ILMARE
@Date 2019-5-27
'''

import csv
import uuid
import os

__all__ = ["mergeSort"]

def mergeSort(pre_path, file_list, key=None):
    reader_lst = []
    fps_lst = []
    for filePath in file_list:
        fp = open(filePath, "r")
        reader = csv.reader(fp)
        reader_lst.append(reader)
        fps_lst.append(fp)
    mergeSort_1(fps_lst, reader_lst, pre_path, 0, len(reader_lst) - 1, key)
    for idx, fp in enumerate(fps_lst):
        fp.close()
        if idx != len(fps_lst) - 1:
            os.remove(fp.name)
    return fps_lst[-1].name

def mergeSort_1(fp_lst, lst, lst_path, begin, end, key=None):
    if begin < end:
        mid = (begin + end) // 2
        reader1 = mergeSort_1(fp_lst, lst, lst_path, begin, mid, key)
        reader2 = mergeSort_1(fp_lst, lst, lst_path, mid + 1, end, key)
        if reader1 is not None:
            if reader2 is not None:
                return merge(fp_lst, lst_path, reader1, reader2, key)
            else:
                return merge(fp_lst, lst_path, reader1, lst[end], key)
        else:
            if reader2 is not None:
                return merge(fp_lst, lst_path, lst[begin], reader2, key)
            else:
                return merge(fp_lst, lst_path, lst[begin], lst[end], key)
    return None

def merge(fp_lst, lst_path, reader1, reader2, key=None):
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
        if key is None:
            num_1 = int(first[-1])
            num_2 = int(second[-1])
        else:
            num_1 = key(first)
            num_2 = key(second)
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