#!/usr/bin/env python

import random

filename = "example.dat"

splits_num = 4
symbol_class_num = 10
feature_num = 5
input_size = 300
rejected_size = 100
repeat = 100
test_input_size = 1000
test_rejected_part = 0.2

max_values = []
letters = []
corrects = []
test_letters = []
corrects_test = []


def create_max_values():
    for i in range(0, feature_num):
        #max_values.append(100)
        max_values.append(random.uniform(1, 100))


def generate_letters():
    for i in range(0, symbol_class_num):
        letter = []
        for j in range(0, feature_num):
            letter.append(random.uniform(0, max_values[j]))
        letters.append(letter)
        corrects.append(i)


def generate_more_letters():
    while len(letters) < input_size:
        letter = []
        base_l = random.randint(0, symbol_class_num - 1)
        for j in range(0, feature_num):
            base_l_feat = letters[base_l][j]
            new_feat = random.gauss(base_l_feat, 1)
            letter.append(min(max_values[j], max(0, new_feat)))
        letters.append(letter)
        corrects.append(base_l)


def generate_test_letters():
    test_rej_size = int(test_input_size * test_rejected_part) if rejected_size != 0 else 0
    for i in range(test_rej_size):
        corrects_test.append(0)
        letter = []
        for j in range(0, feature_num):
            letter.append(random.uniform(0, max_values[j]))
        test_letters.append(letter)

    while len(test_letters) < test_input_size:
        letter = []
        l_num = random.randint(0, symbol_class_num - 1)
        corrects_test.append(l_num + (1 if rejected_size != 0 else 0))
        for j in range(0, feature_num):
            base_l_feat = letters[l_num][j]
            new_feat = random.gauss(base_l_feat, 1)
            letter.append(min(max_values[j], max(0, new_feat)))
        test_letters.append(letter)

create_max_values()
generate_letters()
generate_more_letters()
generate_test_letters()

f = open(filename, 'w')

f.write(('1' if rejected_size != 0 else '0') + ',' + str(splits_num) + ', ' + str(symbol_class_num) + ', ' +
        str(feature_num) + ', ' + str(input_size + rejected_size) + ', ' + str(repeat) + ', ' +
        str(test_input_size) + ', \n\n')

for k in range(0, feature_num):
    f.write(str(max_values[k]) + ', ')

f.write('\n\n')

for k in range(0, input_size):
    f.write(str(corrects[k] + (1 if rejected_size != 0 else 0)) + '\n')
    for j in range(0, feature_num):
        f.write(str(letters[k][j]) + ', ')
    f.write('\n\n')

for k in range(0, rejected_size):
    f.write('0\n')
    for j in range(0, feature_num):
        f.write(str(random.uniform(0, max_values[j])) + ', ')
    f.write('\n\n')

for k in range(0, test_input_size):
    f.write(str(corrects_test[k]) + '\n')
    for feature in test_letters[k]:
        f.write(str(feature) + ', ')
    f.write('\n\n')

f.close()
