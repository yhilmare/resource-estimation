'''
Created By IL MARE
@Date 2019-5-6
'''
import tensorflow as tf
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from domain.dataobject import tran_data
from tensorflow.examples.tutorials.mnist import input_data

class lr_model:
    def __init__(self, batch_size, learning_rate,
                 iterator_num, file_path, model_path):
        self._batch_size = batch_size
        self._lr = learning_rate
        self._iterator_num = iterator_num
        self._model_path = model_path
        # mnist = input_data.read_data_sets(r"F:\tensorflow\MNIST_DATA", one_hot=True)
        obj = tran_data(file_path)
        obj.pca_samples(3)
        self._data = obj
        weight = tf.Variable(name="weight",
                             initial_value=tf.random_normal(shape=[3, 10], dtype=tf.float32, stddev=0.1))
        bias = tf.Variable(name="bias",
                           initial_value=tf.ones(shape=[10], dtype=tf.float32))
        self._x = tf.placeholder(shape=[None, 3], dtype=tf.float32)
        self._y = tf.placeholder(shape=[None, 10], dtype=tf.float32)
        self._pre = tf.nn.softmax(tf.nn.sigmoid(tf.matmul(self._x, weight)) + bias)
        self._loss = -tf.reduce_mean(tf.reduce_sum(self._y * tf.log(self._pre), reduction_indices=[1]))
        self._accuracy = tf.reduce_mean(tf.cast(tf.equal(tf.argmax(self._pre, 1), tf.argmax(self._y, 1)), dtype=np.float32))
        # self._loss = 0.5 * tf.reduce_sum(tf.abs(tf.subtract(self._y, self._pre)))
        # self._accuracy = tf.reduce_mean(tf.reduce_sum(tf.abs(self._pre - self._y) / self._y, reduction_indices=[1]))
        self._optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(self._loss)
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
            for i in range(self._iterator_num):
                train, label = self._data.next_batch(self._batch_size)
                feedt_dict = {
                    self._x: train,
                    self._y: label
                }
                _, _loss, _accuracy = sess.run([self._optimizer, self._loss, self._accuracy], feed_dict=feedt_dict)
                if analyse:
                    _loss_list.append(_loss)
                    _accuracy_list.append(_accuracy)
                if (i + 1) % 1000 == 0:
                    print("iter:", i, ", loss:", _loss, ", accuracy:", _accuracy)
                    if analyse:
                        ax.plot(np.arange(0, i + 1), _accuracy_list, linewidth=0.6, color="r")
                        bx.plot(np.arange(0, i + 1), _loss_list, linewidth=0.6, color="b")
                        plt.pause(0.1)
            # tf.train.Saver().save(sess, "{0}model".format(self._model_path), global_step=self._iterator_num)
            print("accuracy:", sess.run(self._accuracy, feed_dict={self._x: self._data.train, self._y: self._data.labels}))
            if analyse:
                plt.show()
    def test(self):
        sess = tf.Session()
        tf.train.Saver().restore(sess, tf.train.latest_checkpoint(self._model_path))
        print("accuracy:", sess.run(self._accuracy, feed_dict={self._x: self._mnist.test.images, self._y: self._mnist.test.labels}))

if __name__ == "__main__":
    model = lr_model(batch_size=256,
                     learning_rate=0.05,
                     iterator_num=30000,
                     file_path=r"F:/resource_estimation/data/train.csv",
                     model_path=r'F:/tensorflow/softmax_model/')
    model.train(True)
