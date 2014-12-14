#!/usr/bin/env python

import sys
import os
import math
import data_gen


def print_err(s):
    print s
    sys.exit(1)


def error_no_arg(a):
    print_err('Error: Argument \'-' + a + '\' not found!')


def error_wrong_arg(a):
    print_err('Error: Wrong argument \'-' + a + '\'!')


def error_invalid_arg(a, v):
    print_err('Error: Invalid argument \'-' + a + '\' value \'' + v + '\'!')


def check_if_values_in(s, args, valid):
    if s in args.keys():
        if args[s] not in valid:
            error_invalid_arg(s, args[s])


def check_if_file_exists(s, args):
    if s in args.keys():
        if not os.path.isfile(args[s]):
            print_err('Error: File ' + args[s] + ' not found!')


def parse_if_not_eq(s, to_check, val, fa, av):
    if av[to_check] == val:
        if fa[s] != -1:
            error_wrong_arg(s)
    else:
        if fa[s] == -1:
            error_no_arg(s)
        else:
            av[s] = sys.argv[fa[s]]


def parse_if_not_czyt(s, fa, av):
    parse_if_not_eq(s, 'wejscieTyp', 'czyt', fa, av)


def parse_or_set_def(s, def_val, fa, av):
    if fa[s] != -1:
        av[s] = sys.argv[fa[s]]
    else:
        av[s] = def_val


def check_is_int(val):
    try:
        int(val)
    except ValueError:
        error_invalid_arg('', val)


def check_is_float(val):
    try:
        float(val)
    except ValueError:
        error_invalid_arg('', val)


def parse_args(fa):
    av = {}

    s = 'etap'
    if fa[s] == -1:
        error_no_arg(s)
    av[s] = sys.argv[fa[s]]

    s = 'wejscieTyp'
    if fa[s] == -1:
        av[s] = 'gen'
    else:
        av[s] = sys.argv[fa[s]]

    parse_if_not_eq('sciezkaTrain', 'wejscieTyp', 'gen', fa, av)

    s = 'sciezkaTest'
    if fa[s] != -1:
        if av['wejscieTyp'] == 'gen' or fa['procRozmTest'] != -1:
            error_wrong_arg(s)
        else:
            av[s] = sys.argv[fa[s]]

    s = 'procRozmTest'
    if fa[s] == -1:
        if 'sciezkaTest' not in av.keys():
            error_no_arg(s)
    else:
        if 'sciezkaTest' in av.keys():
            error_wrong_arg(s)
        else:
            av[s] = sys.argv[fa[s]]

    s = 'sciezkaOutputKlas'
    if fa[s] != -1:
        av[s] = sys.argv[fa[s]]

    s = 'sciezkaOutputErr'
    if fa[s] != -1:
        av[s] = sys.argv[fa[s]]

    parse_if_not_czyt('iloscKlas', fa, av)
    parse_if_not_czyt('iloscCech', fa, av)
    parse_if_not_czyt('iloscPowtorzenWKlasie', fa, av)
    parse_if_not_czyt('minLos', fa, av)
    parse_if_not_czyt('maxLos', fa, av)
    parse_if_not_czyt('zaburzenie', fa, av)

    s = 'ograniczNietermin'
    if fa[s] != -1:
        av[s] = sys.argv[fa[s]]
    elif av['etap'] == 'a3' or av['etap'] == 'a4':
        error_no_arg(s)

    s = 'dyskretyzacja'
    if fa[s] == -1:
        error_no_arg(s)
    av[s] = sys.argv[fa[s]]

    parse_or_set_def('rownolegle', 'nie', fa, av)

    if av['etap'] in ['a2', 'a4', 'a6']:
        if fa['procRozmObce'] != -1:
            if fa['sciezkaObceTrain'] != -1 or fa['sciezkaObceTest'] != -1:
                error_wrong_arg('procRozmObce')
            else:
                av['procRozmObce'] = sys.argv[fa['procRozmObce']]
        else:
            if fa['sciezkaObceTrain'] == -1:
                error_no_arg('sciezkaObceTrain')
            av['sciezkaObceTrain'] = sys.argv[fa['sciezkaObceTrain']]
            if fa['sciezkaObceTest'] != -1:
                av['sciezkaObceTest'] = sys.argv[fa['sciezkaObceTest']]
    else:
        if fa['sciezkaObceTrain'] != -1 or fa['sciezkaObceTest'] != -1 or fa['procRozmObce'] != -1:
            error_wrong_arg('sciezkeObceTrain/sciezkaObceTest/procRozmObce')

    # TODO: procRozmZaburz

    parse_or_set_def('PSOiter', 1000, fa, av)
    parse_or_set_def('PSOs', 40, fa, av)

    s = 'PSOmaxit'
    if fa[s] != -1 and fa['PSOiter'] == -1:
        av['PSOiter'] = sys.argv[fa[s]]

    parse_or_set_def('PSOtrace', 0, fa, av)
    parse_or_set_def('PSOfnscale', 1.0, fa, av)
    parse_or_set_def('PSOw', 1 / (2 * math.log(2)), fa, av)
    parse_or_set_def('PSOc.p', math.log(2) + 0.5, fa, av)
    parse_or_set_def('PSOc.g', math.log(2) + 0.5, fa, av)

    # TODO: PSO

    return av


def check_args(args):
    check_if_values_in('etap', args, ['a1', 'a2', 'a3', 'a4', 'a5', 'a6'])
    check_if_values_in('wejscieTyp', args, ['czyt', 'gen'])
    check_if_values_in('rownolegle', args, ['tak', 'nie'])
    if 'rownolegle' in args.keys() and args['rownolegle'] == 'tak':
        error_invalid_arg('rownolegle', 'tak')

    check_if_file_exists('sciezkaTrain', args)
    check_if_file_exists('sciezkaTest', args)

    s = 'dyskretyzacja'
    try:
        if int(args[s]) <= 1:
            error_invalid_arg(s, args[s])
    except ValueError:
        error_invalid_arg(s, args[s])

    try:
        if 'minLos' in args and 'maxLos' in args:
            if float(args['minLos']) > float(args['maxLos']):
                error_invalid_arg('minLos/maxLos', '')
    except ValueError:
        error_invalid_arg('minLos/maxLos', '')

    s = 'procRozmTest'
    try:
        if s in args:
            if int(args[s]) < 0 or int(args[s]) > 100:
                error_invalid_arg(s, args[s])
    except ValueError:
        error_invalid_arg(s, args[s])

    s = 'iloscKlas'
    try:
        if s in args:
            if int(args[s]) <= 1:
                error_invalid_arg(s, args[s])
    except ValueError:
        error_invalid_arg(s, args[s])

    s = 'iloscCech'
    try:
        if s in args:
            if int(args[s]) <= 0:
                error_invalid_arg(s, args[s])
    except ValueError:
        error_invalid_arg(s, args[s])

    s = 'iloscPowtorzenWKlasie'
    try:
        if s in args:
            if int(args[s]) <= 0:
                error_invalid_arg(s, args[s])
    except ValueError:
        error_invalid_arg(s, args[s])

    s = 'ograniczNietermin'
    try:
        if s in args:
            if float(args[s]) < 0 or float(args[s]) > 100:
                error_invalid_arg(s, args[s])
    except ValueError:
        error_invalid_arg(s, args[s])

    check_if_file_exists('sciezkaObceTrain', args)
    check_if_file_exists('sciezkaObceTest', args)

    s = 'procRozmObce'
    try:
        if s in args:
            if float(args[s]) < 0:
                error_invalid_arg(s, args[s])
    except ValueError:
        error_invalid_arg(s, args[s])

    check_is_int(args['PSOiter'])
    check_is_int(args['PSOs'])
    check_is_int(args['PSOtrace'])
    check_is_float(args['PSOfnscale'])
    check_is_float(args['PSOw'])
    check_is_float(args['PSOc.p'])
    check_is_float(args['PSOc.g'])


def prepare_data(a_type, i_type, args):
    pso_args = (args['PSOiter'], args['PSOs'], args['PSOtrace'], args['PSOfnscale'],
                args['PSOw'], args['PSOc.p'], args['PSOc.g'])

    if i_type == 'gen':
        if a_type == 'a1':
            data_gen.generate_data_a1(args['iloscKlas'], args['iloscCech'], args['iloscPowtorzenWKlasie'],
                                      args['minLos'], args['maxLos'], args['zaburzenie'], args['procRozmTest'],
                                      args['dyskretyzacja'], pso_args)
        elif a_type == 'a2':
            data_gen.generate_data_a2(args['iloscKlas'], args['iloscCech'], args['iloscPowtorzenWKlasie'],
                                      args['minLos'], args['maxLos'], args['zaburzenie'], args['procRozmTest'],
                                      args['dyskretyzacja'], args['procRozmObce'], pso_args)
        elif a_type == 'a3':
            data_gen.generate_data_a3(args['iloscKlas'], args['iloscCech'], args['iloscPowtorzenWKlasie'],
                                      args['minLos'], args['maxLos'], args['zaburzenie'], args['procRozmTest'],
                                      args['dyskretyzacja'], args['ograniczNietermin'], pso_args)
        elif a_type == 'a4':
            data_gen.generate_data_a4(args['iloscKlas'], args['iloscCech'], args['iloscPowtorzenWKlasie'],
                                      args['minLos'], args['maxLos'], args['zaburzenie'], args['procRozmTest'],
                                      args['dyskretyzacja'], args['procRozmObce'], args['ograniczNietermin'], pso_args)
        elif a_type == 'a5':
            data_gen.generate_data_a5()
        else:
            data_gen.generate_data_a6()
    else:
        # TODO
        return

arguments = [
    'etap', 'wejscieTyp', 'sciezkaTrain', 'sciezkaTest',
    'sciezkaOutputKlas', 'sciezkaOutputErr', 'sciezkaObceTrain', 'sciezkaObceTest', 'iloscKlas', 'iloscCech',
    'iloscPowtorzenWKlasie', 'minLos', 'maxLos', 'zaburzenie',
    'procRozmTest', 'procRozmObce', 'procRozmZaburz', 'dyskretyzacja',
    'ograniczNietermin', 'rownolegle', 'PSOiter', 'PSOs', 'PSOmaxit', 'PSOtrace', 'PSOfnscale',
    'PSOw', 'PSOc.p', 'PSOc.g']

argc = len(sys.argv)

if argc % 2 != 1 or argc < 2:
    print 'Error: Invalid arguments!'
    exit(1)

foundArgs = {}

for a in arguments:
    if sys.argv.count('-' + a) > 1:
        print 'Error: To many arguments!'
        exit(1)

    foundArgs[a] = -1 if sys.argv.count('-' + a) == 0 else sys.argv.index('-' + a) + 1

argValues = parse_args(foundArgs)
check_args(argValues)

prepare_data(argValues['etap'], argValues['wejscieTyp'], argValues)
# TODO