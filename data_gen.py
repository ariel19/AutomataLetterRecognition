#!/usr/bin/env python

import random

filename = 'input.dat'

class_num = 0
feat_num = 0
class_repeat = 0
min_los = 0
max_los = 0
disturbance = 0.0
test_proportions = 0.0
splits_num = 0
pso_args = ()

train_size = 0
test_size = 0

letters = []
test_set = []
train_set = []


def generate_letters():
    for i in range(0, class_num):
        letter = []
        for j in range(0, feat_num):
            letter.append(random.uniform(min_los, max_los))
        letters.append((letter, i))
        for k in range(1, class_repeat):
            rep_letter = []
            for j in range(0, feat_num):
                rep_letter.append(min(max_los, max(min_los, random.gauss(letter[j], disturbance))))
            letters.append((rep_letter, i))


def create_letters_sets():
    for i in range(0, train_size):
        n = random.randint(0, len(letters) - 1)
        train_set.append(letters[n])
        del letters[n]

    for i in range(0, test_size):
        n = random.randint(0, len(letters) - 1)
        test_set.append(letters[n])
        del letters[n]


def gen_dat():
    global train_size, test_size
    all_s = class_num * class_repeat
    test_size = int(all_s * (test_proportions / 100.0))
    train_size = all_s - test_size

    generate_letters()
    create_letters_sets()

    f = open(filename, 'w')

    f.write(str(class_num) + ', ' + str(feat_num) + ', ' + str(splits_num) + ', ' + str(train_size) + ', ' +
            str(test_size) + ', ' + str(min_los) + ', ' + str(max_los) + ', \n')

    for a in pso_args:
        f.write(str(a) + ', ')
    f.write('\n\n')

    for l in train_set:
        f.write(str(l[1]) + '\n')
        for ft in l[0]:
            f.write(str(ft) + ', ')
        f.write('\n\n')

    for l in test_set:
        f.write(str(l[1]) + '\n')
        for ft in l[0]:
            f.write(str(ft) + ', ')
        f.write('\n\n')

    f.close()


def generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _pso_args):
    global class_num, class_repeat, feat_num, min_los, max_los, disturbance, test_proportions, splits_num
    global pso_args

    class_num = int(_class_num)
    class_repeat = int(_class_repeat)
    feat_num = int(_feat_num)
    min_los = float(_min_los)
    max_los = float(_max_los)
    disturbance = float(_dist)
    test_proportions = float(_test_prop)
    splits_num = int(_splits)
    pso_args = _pso_args

    gen_dat()


def generate_data_a1(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _pso_args):
    generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _pso_args)
    return


def generate_data_a2():
    return


def generate_data_a3():
    return


def generate_data_a4():
    return


def generate_data_a5():
    return


def generate_data_a6():
    return