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
foreign_proportions = 0.0
nondeterministic_proportions = 0.0
is_foreign = False
splits_num = 0
pso_args = ()

train_size = 0
test_size = 0

foreign_train_size = 0
foreign_test_size = 0

letters = []
test_set = []
train_set = []


def generate_letters():
    for i in range(0, class_num):
        letter_idx = i + 1 if is_foreign else i
        letter = []
        for j in range(0, feat_num):
            letter.append(random.uniform(min_los, max_los))
        letters.append((letter, letter_idx))
        for k in range(1, class_repeat):
            rep_letter = []
            for j in range(0, feat_num):
                rep_letter.append(min(max_los, max(min_los, random.gauss(letter[j], disturbance))))
            letters.append((rep_letter, letter_idx))


def create_letters_sets():
    for i in range(0, train_size):
        n = random.randint(0, len(letters) - 1)
        train_set.append(letters[n])
        del letters[n]

    for i in range(0, test_size):
        n = random.randint(0, len(letters) - 1)
        test_set.append(letters[n])
        del letters[n]

    for i in range(0, foreign_train_size):
        letter = []
        for j in range(0, feat_num):
            letter.append(random.uniform(min_los, max_los))
        train_set.append((letter, 0))

    for i in range(0, foreign_test_size):
        letter = []
        for j in range(0, feat_num):
            letter.append(random.uniform(min_los, max_los))
        test_set.append((letter, 0))


def gen_dat():
    global train_size, test_size, foreign_train_size, foreign_test_size
    all_s = class_num * class_repeat
    test_size = int(all_s * (test_proportions / 100.0))
    train_size = all_s - test_size

    if is_foreign:
        foreign_train_size = int(train_size * (foreign_proportions / 100.0))
        foreign_test_size = int(test_size * (foreign_proportions / 100.0))

    generate_letters()
    create_letters_sets()

    f = open(filename, 'w')

    f.write(str(class_num) + ', ' + str(feat_num) + ', ' + str(splits_num) + ', ' +
            str(train_size + foreign_train_size) + ', ' + str(test_size + foreign_test_size) + ', ' +
            str(min_los) + ', ' + str(max_los) + ', ' + str(nondeterministic_proportions) + ', \n')

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


def generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop,
                      _splits, _for_prop, _nondet_prop, _pso_args, _is_for):
    global class_num, class_repeat, feat_num, min_los, max_los, disturbance
    global test_proportions, splits_num, foreign_proportions, nondeterministic_proportions
    global pso_args, is_foreign

    class_num = int(_class_num)
    class_repeat = int(_class_repeat)
    feat_num = int(_feat_num)
    min_los = float(_min_los)
    max_los = float(_max_los)
    disturbance = float(_dist)
    test_proportions = float(_test_prop)
    splits_num = int(_splits)
    foreign_proportions = float(_for_prop)
    pso_args = _pso_args
    is_foreign = _is_for
    nondeterministic_proportions = float(_nondet_prop)

    gen_dat()


def generate_data_a1(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _pso_args):
    generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los,
                      _dist, _test_prop, _splits, 0.0, 0.0, _pso_args, False)
    return


def generate_data_a2(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _foreign_prop, _pso_args):
    generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los,
                      _dist, _test_prop, _splits, _foreign_prop, 0.0, _pso_args, True)
    return


def generate_data_a3(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _nondet_prop, _pso_args):
    generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los,
                      _dist, _test_prop, _splits, 0.0, _nondet_prop, _pso_args, False)
    return


def generate_data_a4(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _foreign_prop, _nondet_prop, _pso_args):
    generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los,
                      _dist, _test_prop, _splits, _foreign_prop, _nondet_prop, _pso_args, True)
    return


def generate_data_a5(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _pso_args):
    generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los,
                      _dist, _test_prop, _splits, 0.0, 0.0, _pso_args, False)
    return


def generate_data_a6(_class_num, _feat_num, _class_repeat, _min_los, _max_los, _dist, _test_prop, _splits, _foreign_prop, _pso_args):
    generate_all_data(_class_num, _feat_num, _class_repeat, _min_los, _max_los,
                      _dist, _test_prop, _splits, _foreign_prop, 0.0, _pso_args, True)
    return