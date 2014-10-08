import random

filename = "example.dat"

splits_num = 5
symbol_class_num = 26
feature_num = 20
input_size = 3000
repeat = 1000

letter_diff = 0.1

max_values = []
letters = []


def create_max_values():
    for i in range(0, feature_num):
        max_values.append(random.uniform(1, 100))


def generate_letters():
    for i in range(0, symbol_class_num):
        letter = []
        for j in range(0, feature_num):
            letter.append(random.uniform(1, max_values[j]))
        letters.append(letter)


def generate_more_letters():
    while len(letters) < input_size:
        letter = []
        base_l = random.randint(0, symbol_class_num - 1)
        for j in range(0, feature_num):
            base_l_feat = letters[base_l][j]
            #base_l_max_diff = letter_diff * max[fea]
            #letter.append(random.uniform(, letters[base_l][j])
        letters.append(letter)

create_max_values()

f = open(filename, 'w')

f.write(str(splits_num) + ', ' + str(symbol_class_num) + ', ' +
        str(feature_num) + ', ' + str(input_size) + ', ' + str(repeat) + ', \n\n')

for k in range(0, feature_num):
    f.write(str(max_values[k]) + ', ')

f.write('\n\n')



f.close()
