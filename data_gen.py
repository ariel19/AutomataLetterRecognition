import random

filename = "example.dat"

splits_num = 5
symbol_class_num = 26
feature_num = 20
input_size = 3000
repeat = 1000
test_input_size = 10000

max_values = []
letters = []
corrects = []
test_letters = []


def create_max_values():
    for i in range(0, feature_num):
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
    for i in range(test_input_size):
        letter = []
        for j in range(0, feature_num):
            letter.append(random.uniform(0, max_values[j]))
        test_letters.append(letter)

create_max_values()
generate_letters()
generate_more_letters()
generate_test_letters()

f = open(filename, 'w')

f.write(str(splits_num) + ', ' + str(symbol_class_num) + ', ' +
        str(feature_num) + ', ' + str(input_size) + ', ' + str(repeat) + ', ' +
        str(test_input_size) + ', \n\n')

for k in range(0, feature_num):
    f.write(str(max_values[k]) + ', ')

f.write('\n\n')

for k in range(0, input_size):
    f.write(str(corrects[k]) + '\n')
    for j in range(0, feature_num):
        f.write(str(letters[k][j]) + ', ')
    f.write('\n\n')

for letter in test_letters:
    for feature in letter:
        f.write(str(feature) + ', ')
    f.write('\n\n')

f.close()
