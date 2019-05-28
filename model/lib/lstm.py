'''
Created By ILMARE
@Date 2019-5-28
'''

from tensorflow.contrib import rnn
import tensorflow as tf
import time
import os
from domain.lstmobject import lstm_data
import shelve
from matplotlib import pyplot as plt
import matplotlib as mpl
import numpy as np

def pick_top_n(preds, vocab_size, top_n=5):
    p = np.squeeze(preds)
    p[np.argsort(p)[:-top_n]] = 0
    p = p / np.sum(p)
    c = np.random.choice(vocab_size, 1, p=p)[0]
    return c

class lstm_model:
    def __init__(self, hidden_size, num_layer, corpus, keep_prob,
                 embedding_size, lr, max_step, save_path, sampling=False):
        if not sampling:
            self._num_seq = corpus.num_seq
            self._num_step = corpus.num_step
        else:
            self._num_seq = 1
            self._num_step = 1
        self._save_path = save_path
        self._lr = lr
        self._max_step = max_step
        self._embedding_size = embedding_size
        self._hidden_size = hidden_size
        self._num_layer = num_layer
        self._corpus = corpus
        self._keep_prob = keep_prob
        tf.reset_default_graph()
        self.init_inputs()
        self.build_lstm()
        self.define_loss()
        self.define_gradients()
    def init_inputs(self):
        self._x = tf.placeholder(dtype=tf.int32,
                                      shape=[self._num_seq, self._num_step])
        with tf.device("/cpu:0"):
            embedding = tf.get_variable("embedding",
                                        shape=[self._corpus.word_num,
                                               self._embedding_size], dtype=tf.float32)
            self._inputs = tf.nn.embedding_lookup(embedding, self._x)
    def build_lstm(self):
        def build_cell():
            cell = rnn.BasicLSTMCell(self._hidden_size, forget_bias=1.0, state_is_tuple=True)
            cell = rnn.DropoutWrapper(cell, output_keep_prob=self._keep_prob)
            return cell
        mul_cell = rnn.MultiRNNCell([build_cell() for _ in range(self._num_layer)],
                                    state_is_tuple=True)
        self._init_state = mul_cell.zero_state(self._num_seq, dtype=tf.float32)
        outputs, self._final_state = tf.nn.dynamic_rnn(mul_cell, self._inputs,
                                                       initial_state=self._init_state)
        print(outputs.shape)
        outputs = tf.reshape(outputs, [-1, self._hidden_size])
        W = tf.Variable(tf.truncated_normal([self._hidden_size, self._corpus.word_num],
                                            stddev=0.1, dtype=tf.float32))
        bais = tf.Variable(tf.zeros([1, self._corpus.word_num],
                                    dtype=tf.float32), dtype=tf.float32)
        self._prediction = tf.nn.softmax(tf.matmul(outputs, W) + bais)
    def define_loss(self):
        self._y = tf.placeholder(dtype=tf.int32, shape=[self._num_seq, self._num_step])
        y_one_hot = tf.reshape(tf.one_hot(self._y, self._corpus.word_num), self._prediction.shape)
        self._loss = -tf.reduce_mean(tf.reduce_sum(y_one_hot * tf.log(self._prediction),
                                                   reduction_indices=[1]))
    def define_gradients(self):
        vars = tf.trainable_variables()
        grads, _ = tf.clip_by_global_norm(tf.gradients(self._loss, vars), 3)
        optimizer = tf.train.AdamOptimizer(self._lr)
        self._optimizer = optimizer.apply_gradients(zip(grads, vars))
    def train(self):
        with tf.Session() as sess:
            return_mat = []
            sess.run(tf.global_variables_initializer())
            state = sess.run(self._init_state)
            step = 0
            start = time.clock()
            for x, y in self._corpus.generate_batch():
                feed = {self._x: x,
                        self._y: y,
                        self._init_state:state}
                loss, _, state = sess.run([self._loss, self._optimizer,
                                           self._final_state], feed_dict = feed)
                return_mat.append(loss)
                step += 1
                if step % 10 == 0:
                    end = time.clock()
                    interval = end - start
                    yield return_mat
                    print("迭代次数：{0:d}/{2:d}，当前损失：{1:.3f}，迭代速度：{3:.3f} 秒/十次，约需要{4:.3f}秒"
                          .format(step, loss, self._max_step, interval, ((self._max_step - step) / 10) * interval))
                    start = time.clock()
                if step == self._max_step:
                    break
            tf.train.Saver().save(sess, "{0}model".format(self._save_path), global_step=step)
    def load_model(self):
        sess = tf.Session()
        tf.train.Saver().restore(sess, tf.train.latest_checkpoint(self._save_path))
        self._sess = sess
    def sampling(self, init_str, max_sample=30):
        sample = [c for c in init_str]
        pre = np.ones((self._corpus.word_num, ))
        state = self._sess.run(self._init_state)
        for c in sample:
            feed = {self._x: np.reshape(c, [1, 1]),
                    self._init_state: state}
            pre, state = self._sess.run([self._prediction, self._final_state],
                                 feed_dict=feed)
        c = pick_top_n(pre, self._corpus.word_num)
        sample.append(c)
        for count in range(max_sample):
            x = np.zeros([1, 1])
            x[0][0] = c
            feed = {self._x: x,
                    self._init_state: state}
            pre, state = self._sess.run([self._prediction, self._final_state],
                                        feed_dict=feed)
            c = pick_top_n(pre, self._corpus.word_num)
            sample.append(c)
        return sample

path = r"F:/tensorflow/rnn/data/novel.txt"
save_path = r"F:/tensorflow/rnn/model/"

def train_model():
    obj = lstm_data(path, 50, 50, 3000)
    with shelve.open("{0}corpus".format(save_path)) as fp:
        fp["obj"] = obj
    model = lstm_model(hidden_size=128, num_layer=2,
                     corpus=obj, keep_prob=1.0,
                     embedding_size=128, max_step=5000,
                     lr=0.005, save_path=save_path)
    result = []
    fig = plt.figure("cross-entropy")
    mpl.rcParams['xtick.labelsize'] = 8
    mpl.rcParams['ytick.labelsize'] = 8
    ax = fig.add_subplot(111)
    # ax.grid(True)
    for return_mat in model.train():
        result.extend(return_mat)
        # x = np.arange((len(return_mat)))
        # y = np.array(return_mat)
        # ax.plot(x, y, linewidth=0.8, color="b")
        # plt.pause(0.1)
    x = np.arange((len(return_mat)))
    y = np.array(return_mat)
    ax.plot(x, y, linewidth=0.8, color="b")
    plt.show()

def test_model(init_str, max_sample=200):
    obj = None
    assert os.path.exists("{0}corpus.bak".format(save_path)), "找不到文件"
    with shelve.open("{0}corpus".format(save_path)) as fp:
        obj = fp["obj"]
    model = lstm_model(hidden_size=128, num_layer=2,
                     corpus=obj, keep_prob=1.0,
                     embedding_size=128, max_step=5000,
                     lr=0.005, save_path=save_path, sampling=True)
    model.load_model()
    sample = model.sampling(obj.sentence_to_int(init_str), max_sample)
    print(obj.int_to_sentence(sample))

if __name__ == "__main__":
    obj = lstm_data(path, 50, 50, 3000)
    model = lstm_model(hidden_size=128, num_layer=2,
                       corpus=obj, keep_prob=1.0,
                       embedding_size=128, max_step=5000,
                       lr=0.005, save_path=save_path)
