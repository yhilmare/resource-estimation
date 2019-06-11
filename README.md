# 一体化资源开销预测系统

本仓库是针对数据库并发查询开销进行预测的，包含了数据库负载生成，数据库压力测试，数据库资源开销预测等等多个部分。目前针对的数据库是`postgresql`，截止到目前为止仅仅做了有限的工作。

## 数据库负载的生成以及压力测试

本系统针对的预测场景是单点关系型数据库在并发访问工况下各种资源的消耗预测，因此在负载上选取面向事务的`tpcc`测试。由于`Percona-lab`仅有针对`Mysql`的tpcc测试，因此针对`Postgresql`的测试就需要自己编写，之所以不使用现有的benchmark工具是因为在测试过程中需要收集事务执行的诸多信息，只有在具体的代码中才能更好地，更灵活地收集这些信息。本系统针对`Postgresql`的`tpcc`测试遵守`tpcc`测试规范，具体的测试代码采用c++编写，代码详见`/load`目录。

使用者可以通过克隆仓库到本地获取源代码，本仓库的c++部分可以使用`cmake`进行编译，且`win`和`Linux`平台通用，具体的编译方式如下：

```Shell
root@lab4:/home/yanghang/test# cmake .
-- The C compiler identification is GNU 5.4.0
...
-- Build files have been written to: /home/yanghang/test
root@lab4:/home/yanghang/test# make
Scanning dependencies of target load
[  4%] Building CXX object CMakeFiles/load.dir/main.cpp.obj
[  8%] Building CXX object CMakeFiles/load.dir/lib/pg_lib/pg_connection.cpp.obj
...
[100%] Linking CXX executable load
[100%] Built target load
```
编译而成的`load`程序可以在线数据导入，也可以生成数据文件然后通过`COPY`命令批量导入。导入数据后使用者可以利用`load`程序进行`tpcc`测试并且可以设定并发度等参数。程序在进行并发访问数据库的同时还会生成并发事务执行的`log`信息保存在指定的目录下。

## 预测事务执行的时间开销

### BP网络预测

这一步是建立在事务执行的`log`信息上的，预测代码保存在`/model`目录下，关于具体的依赖请直接在`requirement.txt`中查看。预测模型首先对事务执行的`log`进行解析，得到能够输入到模型中的输入。这个输入是九个二元组组成的一个十八维向量，每个元组对应一个`tpcc`表，元组中的第一位代表该事务对该表的读次数，第二位代表对该表进的写次数，每一个向量的`label`则是该事务的执行时间（毫秒）。通过解析事务执行的`log`可以大概看出事务执行的时间分布是非常零散的，从200毫秒到8000毫秒都有。为了避免模型的输出太过分散，对所有的执行时间都取`e`的对数。接下来开始对执行时间进行直接回归预测，下图是非并发条件下事务执行时间预测的损失和准确率：

![](https://github.com/yhswjtuILMARE/Resource_estimation/blob/master/pic/bp/loss_accuracy_for_predictin_time.png)

若直接对执行时间进行回归预测效果是不好的，损失和准确率都一直在一个水平上震荡，没有表现出收敛的趋势，这可能是由于模型太过简单造成的。为了解决这个问题，我们可以考虑更粗粒度的预测，也就是说将事务执行时间的自然底数对数进行区间划分，模型只预测事务执行时间自然底数对数可能落入的区间，这样就把一个回归问题转化成了分类问题。在这里，考虑到模型的泛化能力和事务执行时间的散布，选取了十个区间。以下是非并发条件下执行时间区间的预测的准确率和损失：

![](https://github.com/yhswjtuILMARE/Resource_estimation/blob/master/pic/bp/loss_accuracy_without_concurrent.png)

模型收敛是非常好的，最终准确率达到0.88左右。接下来，使用测试集测试模型并且输出预测的事务执行时间。预测值和测试集的实际值对比如下：

![](https://github.com/yhswjtuILMARE/Resource_estimation/blob/master/pic/bp/prediction_for_testset.png)

为了更清楚地观察模型的预测能力，我们将执行时间排序，预测值与实际值的对比如下：

![](https://github.com/yhswjtuILMARE/Resource_estimation/blob/master/pic/bp/prediction_aganist_real.png)

最后，评价一下该模型：

```shell
Label accuracy: 0.882
Digtial accuracy: 0.1->12.989% | 0.2->25.822% | 0.3->37.402% | 0.35->43.140%
```
该模型的执行时间的区间预测准确率达到了0.882，在映射到真实的执行时间上效果就差很多，可以看到有12%的样本预测误差在0.1以内，43%的样本预测误差在0.35以内。当然，以上模型仅仅是针对非并发条件下的事务执行时间预测。如果是并发条件下呢？为此，我们使用多线程模拟并发工况，模型对并发状态下的事务执行时间预测效果如下：

![](https://github.com/yhswjtuILMARE/Resource_estimation/blob/master/pic/bp/loss_accuracy_for%20concurrent.png)

并发条件下模型表现要差很多，非常不稳定。这也说明了事务的执行时间不仅仅与事务本身的性质相关，在并发条件下事务与事务之间的耦合关系也是一个重大因素。
