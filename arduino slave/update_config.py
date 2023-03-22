#!/usr/bin/env python
import yaml
import os
import glob
import json

from yaml.loader import Loader

ini_template = """
[env:%s]
build_src_filter =
    +<../main/*>
%s
    
build_flags = ${env.build_flags}
%s
"""

configs = glob.glob('pioconfig/*.json')
board_names = []
changed = False


def flatten(obj):
    flattened = []
    for key in obj:
        if type(obj[key]) == dict:
            flatten(obj[key])
            flattened.append(key)
    for key in flattened:
        values = obj[key]
        del obj[key]
        for entry in values:
            obj[key + '_' + entry] = values[entry]

    return obj


def appendFlags(entries, name):
    flatten(entries)

    flags = ''
    flags += "    -D%s=%s\n" % ((name).upper(),"1")

    for key in entries:
        value = entries[key]
        if type(value) == str:
            if key[-1] == '!':
                flags += "    -D%s=%s\n" % ((name+'_'+key[:-1]).upper(), value)
        else:
            flags += "    -D%s=%s\n" % ((name+'_'+key).upper(), value)
    flags += "    \n"

    return flags


for config in configs:
    ini = ''
    srcs = ''
    flags = ''
    board_name = os.path.basename(config).split('.')[0]
    board_names.append(board_name)

    with open(config, 'r') as f:
        try:
            config = yaml.load(f, Loader=yaml.Loader)

            if 'device' in config and config['device'] is not None:
                for device in config['device']:
                    if not os.path.exists('src/%s.cpp' % device):
                        print('! Module not found: %s' % device)
                        exit(1)
                    srcs += "    +<%s.cpp>\n" % device
                    

                    for name in config['device'][device]:
                        if not os.path.exists('src/%s.cpp' % name):
                            print('! Module not found: %s' % name)
                            exit(1)
                        srcs += "    +<%s.cpp>\n" % name

                        if config['device'][device][name] : 
                            flags += "    ; From %s\n" % name
                            flags += appendFlags(config['device'][device][name], name)
                            # print(flags)

            ini = ini_template % (board_name, srcs, flags)
            ini_filename = 'pioconfig/platformio_ini/%s.ini' % board_name

            before = None
            if os.path.isfile(ini_filename):
                f = open(ini_filename, 'r')
                before = f.read()
                f.close()

            if before != ini:
                f = open(ini_filename, 'w')
                f.write(ini)
                f.close()
                print('- Wrote %s.ini' % board_name)

                if before != ini:
                    changed = True

        except yaml.scanner.ScannerError as e:
            print('Yaml file ' + config + ' contain errors: ' + "\n\n" + str(e))

inis = glob.glob('pioconfig/platformio_ini/*.ini')
for ini in inis:
    board_name = os.path.basename(ini).split('.')[0]
    if board_name not in board_names:
        print('Removing %s because there is no corresponding yaml file' % ini)
        os.unlink(ini)

if changed:
    print('! There were some outdated INI files, they were re-generated, please restart pio')
    exit(1)