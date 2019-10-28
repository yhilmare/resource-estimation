'''
Created By IL MARE
@Date 2019-5-6
'''
import tensorflow as tf
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from domain.bpobject import bp_data
from utils import propertiesutils as pu
import re

class bp_model:
    def __init__(self, batch_size, learning_rate,
                 iterator_num, data_obj, pca_dim, model_path):
        self._batch_size = batch_size
        self._lr = learning_rate
        self._iterator_num = iterator_num
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]{0,1}$", model_path)
        assert test != None, Exception("path is invaild")
        if model_path[-1] is not "\\" and model_path[-1] is not "/":
            model_path = "{0}/".format(model_path)
        self._model_path = model_path
        self._dest_dim = pca_dim
        self._data = data_obj
        self._data.pca_samples(pca_dim)
        self.define_network()
    def define_network(self):
        self._x = tf.placeholder(shape=[None, self._dest_dim], dtype=tf.float32)
        self._y = tf.placeholder(shape=[None, self._data.dest_dim], dtype=tf.float32)
        with tf.variable_scope("level0"):
            weight = tf.Variable(name="weight",
                                 initial_value=tf.random_normal(shape=[self._dest_dim, 128], dtype=tf.float32, stddev=0.1))
            bias = tf.Variable(name="bias",
                               initial_value=tf.ones(shape=[128], dtype=tf.float32))
            level_1 = tf.nn.sigmoid(tf.matmul(self._x, weight) + bias)
        with tf.variable_scope("level1"):
            weight1 = tf.Variable(name="weight1",
                                 initial_value=tf.random_normal(shape=[128, 256], dtype=tf.float32, stddev=0.1))
            bias1 = tf.Variable(name="bias1",
                               initial_value=tf.ones(shape=[256], dtype=tf.float32))

            level_2 = tf.nn.sigmoid(tf.matmul(level_1, weight1) + bias1)
        with tf.variable_scope("level2"):
            weight2 = tf.Variable(name="weight2",
                                 initial_value=tf.random_normal(shape=[256, 128], dtype=tf.float32, stddev=0.1))
            bias2 = tf.Variable(name="bias2",
                               initial_value=tf.ones(shape=[128], dtype=tf.float32))
            level_3 = tf.nn.sigmoid(tf.matmul(level_2, weight2) + bias2)
        with tf.variable_scope("level3"):
            weight3 = tf.Variable(name="weight3",
                                  initial_value=tf.random_normal(shape=[128, self._data.dest_dim], dtype=tf.float32, stddev=0.1))
            bias3 = tf.Variable(name="bias3",
                                initial_value=tf.ones(shape=[self._data.dest_dim], dtype=tf.float32))
            self._pre = tf.nn.softmax(tf.matmul(level_3, weight3) + bias3)
        self._loss = -tf.reduce_mean(tf.reduce_sum(self._y * tf.log(self._pre), reduction_indices=[1]))
        self._accuracy = tf.reduce_mean(tf.cast(tf.equal(tf.argmax(self._pre, 1), tf.argmax(self._y, 1)), dtype=np.float32))
        # self._loss = 0.5 * tf.reduce_sum(tf.abs(tf.subtract(self._y, self._pre)))
        # self._accuracy = tf.reduce_mean(tf.reduce_sum(tf.abs(self._pre - self._y) / self._y, reduction_indices=[1]))
        self._optimizer = tf.train.GradientDescentOptimizer(self._lr).minimize(self._loss)
    @property
    def dest_dim(self):
        return self._dest_dim
    def train(self, analyse=False):
        mpl.rcParams["xtick.labelsize"] = 8
        mpl.rcParams["ytick.labelsize"] = 8
        fig = plt.figure("Test")
        ax = fig.add_subplot(211)
        bx = fig.add_subplot(212)
        ax.grid(True)
        bx.grid(True)
        _loss_list = []
        _accuracy_list = []
        with tf.Session() as sess:
            sess.run(tf.global_variables_initializer())
            for i in range(1, self._iterator_num):
                train, label = self._data.train.next_batch(self._batch_size)
                feedt_dict = {
                    self._x: train,
                    self._y: label
                }
                _, _loss, _accuracy = sess.run([self._optimizer, self._loss, self._accuracy],
                                               feed_dict=feedt_dict)
                if analyse:
                    _loss_list.append(_loss)
                    _accuracy_list.append(_accuracy)
                if i % 1000 == 0:
                    print("iter:", i, ", loss:", _loss, ", accuracy:", _accuracy)
                    if analyse:
                        if i > 1000:
                            ax.plot(np.arange(i - 1001, i), _accuracy_list[i - 1001: i], color="r")
                            bx.plot(np.arange(i - 1001, i), _loss_list[i - 1001: i], color="b")
                        else:
                            ax.plot(np.arange(i - 1000, i), _accuracy_list[i - 1000: i], color="r")
                            bx.plot(np.arange(i - 1000, i), _loss_list[i - 1000: i], color="b")
                        plt.pause(0.1)
            tf.train.Saver().save(sess, "{0}model".format(self._model_path), global_step=self._iterator_num)
            print("accuracy:", sess.run(self._accuracy,
                                        feed_dict={
                                            self._x: self._data.train.samples,
                                            self._y: self._data.train.labels}))
            if analyse:
                plt.show()
    def load(self):
        self._sess = tf.Session()
        tf.train.Saver().restore(self._sess, tf.train.latest_checkpoint(self._model_path))
    def test(self):
        print("accuracy:", self._sess.run(self._accuracy,
                                          feed_dict={self._x: self._data.test.samples,
                                                     self._y: self._data.test.labels}))
    def predict(self, sample):
        return self._sess.run(self._pre, feed_dict={self._x: sample})

if __name__ == "__main__":
    reader = pu.configreader(pu.configfile)
    obj = bp_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.BP_DATA],
                  one_hot=True, min=2, max=6, dest_dim=102)
    model = bp_model(batch_size=256,
                     learning_rate=0.05,
                     iterator_num=100000, data_obj=obj, pca_dim=8,
                     model_path=reader[pu.SECTIONS.MODEL][pu.OPTIONS.BP_MODEL])
    model.train(True)
    # model.load()
    # model.test()
    # res = model.predict(obj.test.samples)
    # print(res.shape)
