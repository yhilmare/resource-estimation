'''
Created By ILMARE
@Date 2019-5-28
'''

from tensorflow.contrib import rnn
import tensorflow as tf
import time
from domain.lstmobject import lstm_data, lstm_data_core
import numpy as np
import re
from matplotlib import pyplot as plt
import matplotlib as mpl
from utils import propertiesutils as pu

class lstm_model:
    def __init__(self, hidden_size, num_layer, data_obj, keep_prob,
                 l_rate, max_step, save_path, batch_size, sampling=False):
        if not sampling:
            self._batch_size = batch_size
            self._tran_size = data_obj.transaction_size
        else:
            self._batch_size = 1
            self._tran_size = 1
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", save_path)
        assert test != None, Exception("path is invaild")
        if save_path[-1] is not "\\" and save_path[-1] is not "/":
            save_path = "{0}/".format(save_path)
        self._save_path = save_path
        self._lr = l_rate
        self._max_step = max_step
        self._hidden_size = hidden_size
        self._num_layer = num_layer
        self._data_obj = data_obj
        self._keep_prob = keep_prob
        tf.reset_default_graph()
        self.build_lstm()
        self.define_loss()
        self.define_gradients()
    def build_lstm(self):
        def build_cell():
            cell = rnn.BasicLSTMCell(self._hidden_size, forget_bias=1.0, state_is_tuple=True)
            cell = rnn.DropoutWrapper(cell, output_keep_prob=self._keep_prob)
            return cell
        self._inputs = tf.placeholder(dtype=tf.float32,
                                      shape=[None, self._tran_size, self._data_obj.sample_dim])
        mul_cell = rnn.MultiRNNCell([build_cell() for _ in range(self._num_layer)],
                                    state_is_tuple=True)
        self._init_state = mul_cell.zero_state(self._batch_size, dtype=tf.float32)
        outputs, self._final_state = tf.nn.dynamic_rnn(mul_cell, self._inputs,
                                                       initial_state=self._init_state)
        # print(outputs.shape)
        outputs = tf.reshape(outputs, [-1, self._hidden_size])
        W = tf.Variable(tf.truncated_normal([self._hidden_size, self._data_obj.label_dim],
                                            stddev=0.1, dtype=tf.float32))
        bais = tf.Variable(tf.zeros([1, self._data_obj.label_dim],
                                    dtype=tf.float32), dtype=tf.float32)
        self._prediction = tf.nn.softmax(tf.matmul(outputs, W) + bais)
    def define_loss(self):
        self._y = tf.placeholder(dtype=tf.float32, shape=[None, self._tran_size, self._data_obj.label_dim])
        y_one_hot = tf.reshape(self._y, self._prediction.shape)
        self._loss = -tf.reduce_mean(tf.reduce_sum(y_one_hot * tf.log(self._prediction),
                                                   reduction_indices=[1]))
        self._accuracy = tf.reduce_mean(
            tf.cast(tf.equal(tf.argmax(self._prediction, 1), tf.argmax(y_one_hot, 1)), dtype=np.float32))
    def define_gradients(self):
        self._optimizer = tf.train.AdamOptimizer(self._lr).minimize(self._loss)
        # vars = tf.trainable_variables()
        # grads, _ = tf.clip_by_global_norm(tf.gradients(self._loss, vars), 3)
        # optimizer = tf.train.AdamOptimizer(self._lr)
        # self._optimizer = optimizer.apply_gradients(zip(grads, vars))
    def train(self):
        assert self._tran_size != 1 and self._batch_size != -1, Exception("Sample need to be False")
        fig = plt.figure("cross-entropy")
        mpl.rcParams['xtick.labelsize'] = 16
        mpl.rcParams['ytick.labelsize'] = 16
        mpl.rcParams["font.size"] = 16
        ax = fig.add_subplot(211)
        ax.grid(True)
        ax.set_ylabel("Loss")
        ax.set_xlabel("Times")
        bx = fig.add_subplot(212)
        bx.set_ylabel("Accuracy")
        bx.set_xlabel("Times")
        bx.grid(True)
        with tf.Session() as sess:
            loss_lst = []
            acc_lst = []
            sess.run(tf.global_variables_initializer())
            state = sess.run(self._init_state)
            start = time.clock()
            for step in range(1, self._max_step):
                train, labels = self._data_obj.train.next_batch(self._batch_size)
                feed = {self._inputs: train, self._y: labels, self._init_state:state}
                loss, _, state, acc = sess.run([self._loss, self._optimizer,
                                           self._final_state, self._accuracy], feed_dict=feed)
                loss_lst.append(loss)
                acc_lst.append(acc)
                if step % 100 == 0:
                    end = time.clock()
                    interval = end - start
                    print("Iter: {0:d}/{2:d}, Loss: {1:.3f}, Accuracy: {5:.3f}, Iter_speed: {3:.3f} sec/10 items, Remaining time: {4:.3f} sec".format(
                        step, loss, self._max_step, interval, ((self._max_step - step) / 100) * interval, acc))
                    if step > 100:
                        ax.plot(np.arange(step - 101, step), loss_lst[step - 101: step], color="r")
                        bx.plot(np.arange(step - 101, step), acc_lst[step - 101: step], color="b")
                    else:
                        ax.plot(np.arange(step - 100, step), loss_lst[step - 100: step], color="r")
                        bx.plot(np.arange(step - 100, step), acc_lst[step - 100: step], color="b")
                    plt.pause(0.1)
                    start = time.clock()
            plt.show()
            tf.train.Saver().save(sess, "{0}model".format(self._save_path), global_step=step)
    def load_model(self):
        sess = tf.Session()
        tf.train.Saver().restore(sess, tf.train.latest_checkpoint(self._save_path))
        self._sess = sess
    def test(self):
        state = self._sess.run(self._init_state)
        acc, pre = self._sess.run([self._accuracy, self._prediction], feed_dict={self._inputs: self._data_obj.test.samples,
                                                        self._y: self._data_obj.test.labels,
                                                        self._init_state: state})
        return pre, acc


if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    model_path = reader[pu.SECTIONS.MODEL][pu.OPTIONS.RNN_MODEL]
    obj = lstm_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.RNN_DATA], 25,
                    True, min=4, max=11, label_dim=102)
    obj.pca_samples(11)
    model = lstm_model(hidden_size=128, num_layer=2, data_obj=obj,
                       keep_prob=0.8, l_rate=0.005, max_step=10000,
                       save_path=model_path, batch_size=256)
    model.train()
    # pre, acc = model.test()
    # print(pre.shape)

