'''
Created By ILMARE
@Date 2019-6-9
'''

from lib import lstm
from domain.lstmobject import lstm_data
from utils import propertiesutils as pu

def train_model():
    reader = pu.configreader(pu.configfile)
    model_path = reader[pu.SECTIONS.MODEL][pu.OPTIONS.RNN_MODEL]
    obj = lstm_data(reader[pu.SECTIONS.DATA][pu.OPTIONS.RNN_DATA], 25, 20, True)
    obj.pca_samples(8)
    model = lstm.lstm_model(hidden_size=128, num_layer=2, data_obj=obj,
                       keep_prob=0.8, l_rate=0.005, max_step=5000,
                       save_path=model_path, batch_size=256)
    model.train()

if __name__ == "__main__":
    train_model()