'''
Created by ILMARE
@Date 2019-6-13
'''

import os
import configparser
import re

class SECTIONS:
    MODEL="tf_model"
    DATA="data_file"

class OPTIONS:
    RNN_MODEL="rnn_model_dir"
    BP_MODEL="bp_model_dir"
    RNN_DATA="rnn_data_dir"
    BP_DATA="bp_data_dir"

class config_reader_sections:
    def __init__(self, section_name, config_parser):
        self._section_name = section_name
        self._config_parser = config_parser
        self._items = self._config_parser.items(section_name)
        self._options = self._config_parser.options(section_name)
    class config_reader_sections_descriptor:
        def __init__(self, getter):
            self.__getter = getter
        def __get__(self, instance, cls):
            return self.__getter(instance)
    def __options_getter(self):
        return self._options
    def __items_getter(self):
        return self._items
    options = config_reader_sections_descriptor(__options_getter)
    items = config_reader_sections_descriptor(__items_getter)
    def __getitem__(self, item):
        return self._config_parser.get(self._section_name, item)
    def __str__(self):
        return "[config_reader_sections at %d]" % (id(self))

class configreader:
    def __init__(self, config_file):
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]\w+\.conf$", config_file)
        assert test != None, Exception("the path is invaild")
        assert os.path.exists(config_file), FileNotFoundError("There is no such file")
        self._config_file = config_file
        self._config_parser = configparser.ConfigParser()
        self._config_parser.read(self._config_file)
    class config_file_descriptor:
        def __init__(self, getter, setter):
            self._getter = getter
            self._setter = setter
        def __get__(self, instance, cls):
            return self._getter(instance)
        def __set__(self, instance, value):
            self._setter(instance, value)
    def __config_file_setter(self, value):
        test = re.match(r"^([a-zA-Z]:){0,1}([\\/][a-zA-Z0-9_-]+)+[\\/]\w+\.conf$", value)
        assert os.path.exists(value), FileNotFoundError("There is no such file")
        assert test != None, Exception("the path is invaild")
        self._config_file = value
        self._config_parser.read(value)
    def __config_file_getter(self):
        return self._config_file
    config_file = config_file_descriptor(__config_file_getter, __config_file_setter)
    @property
    def sections(self):
        return self._config_parser.sections()
    def __getitem__(self, item):
        return config_reader_sections(item, self._config_parser)
    def __str__(self):
        return "[configreader at 0x%d]" % (id(self))

# configfile = r"F:\repos\Resource_estimation\model\conf\setting.conf"

configfile = r"/Users/yh_swjtu/Desktop/repos/Resource_estimation/model/conf/setting_mac.conf"

if __name__ == "__main__":
    reader = configreader(configfile)
    print(reader.config_file)
    print(reader.sections)
    print(reader[SECTIONS.MODEL][OPTIONS.RNN_MODEL])
