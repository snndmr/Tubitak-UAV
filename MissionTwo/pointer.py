import os
import json


class Pointer:
    def __init__(self):
        self.id = 0
        self.data = dict()

    def get_data(self):
        return self.data

    def set_data(self, data):
        self.data = data

    def add_data(self, value):
        self.id += 1
        self.data[self.id] = value

    def write_to_file(self):
        with open('data.json', 'w') as outfile:
            json.dump(self.data, outfile, indent=2)

    def read_from_file(self):
        if os.path.exists('./data.json'):
            with open('data.json') as json_file:
                self.set_data(json.load(json_file))

    def show_specific_data(self, index):
        print(' Point  : {}'.format(index))
        print(' --------------')
        print(' Feature: {}'.format(self.data[index]['Feature']))
        print(' Alt    : {}'.format(self.data[index]['Alt']))
        print(' Lat    : {}'.format(self.data[index]['Lat']))
        print(' Lon    : {}'.format(self.data[index]['Lon']))
        print(' Speed  : {}'.format(self.data[index]['Speed']))
        print('')

    def show_all_data(self):
        for index in self.data:
            self.show_specific_data(index)
