'''
Created By ILMARE
@Date 2018-5-28
'''

import collections
import numpy as np

class lstm_data:
    '''
    该对象用于构造语料库，参数解释：
    file_path:语料库所在位置
    num_seq:一个batch中所包含的句子数
    num_step:一个句子中包含的词的数目
    max_size:统计语料库中出现频度前max_size的字或词
    '''
    def __init__(self, file_path, num_seq=10, num_step=10, max_size=3500):
        self._file_path = file_path
        with open(self._file_path, "r", encoding="utf_8") as fp:
            self._buffer = fp.read()
        self._count = collections.Counter(self._buffer).most_common(max_size)
        self._word_to_int = dict()
        for word, _ in self._count:
            self._word_to_int[word] = len(self._word_to_int)
        self._int_to_word = dict(zip(self._word_to_int.values(), self._word_to_int.keys()))
        self._batch_size = num_seq * num_step
        num_batch = len(self._buffer) // self._batch_size
        self._buffer = self._buffer[: num_batch * self._batch_size]
        self._num_seq = num_seq
        self._num_step = num_step
    @property
    def num_seq(self):
        return self._num_seq
    @property
    def num_step(self):
        return self._num_step
    @property
    def file_path(self):
        return self._file_path
    @property
    def word_num(self):
        return len(self._int_to_word) + 1
    @property
    def batch_size(self):
        return self._batch_size
    @property
    def words(self):
        return self._buffer
    def sentence_to_int(self, sentence):
        return_mat = []
        for word in sentence:
            return_mat.append(self.word_to_int(word))
        return np.array(return_mat)
    def int_to_sentence(self, row):
        return_mat = []
        for index in row:
            return_mat.append(self.int_to_word(index))
        return "".join(return_mat)
    def word_to_int(self, word):
        return self._word_to_int.get(word, len(self._int_to_word))
    def int_to_word(self, index):
        return self._int_to_word.get(index, "<unk>")
    def text_to_attr(self):
        return_mat = []
        for _word in self._buffer:
            return_mat.append(self.word_to_int(_word))
        return np.array(return_mat)
    def attr_to_text(self, attr):
        return_mat = []
        for _attr in attr:
            return_mat.append(self.int_to_word(_attr))
        return return_mat
    def generate_batch(self):
        attrs = self.text_to_attr()
        attrs = np.reshape(attrs, [self.num_seq, -1])
        while True:
            np.random.shuffle(attrs)
            for index in range(0, attrs.shape[1], self.num_step):
                x = attrs[:, index: index + self.num_step]
                y = np.zeros_like(x)
                y[:, :-1], y[:, -1] = x[:, 1:], x[:, 0]
                yield x, y