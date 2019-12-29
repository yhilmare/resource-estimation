'''
Created By IL MARE
@Date 2019-6-13
'''
import numpy as np

if __name__ == "__main__":
    lst = [38231359488,214973267968,42887155712,25252599808,16302467072]
    lst = np.asarray(lst, dtype=np.int64)
    print(np.std(lst) / lst)