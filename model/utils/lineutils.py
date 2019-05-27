'''
Created By ILMARE
@Date 2019-5-27
'''
import re

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