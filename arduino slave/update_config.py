#!/usr/bin/env python
import yaml
import os
import glob
import json

from yaml.loader import Loader

ini_template = """
[env:%s]
src_filter =
    +<../ws_server/*>
%s
    
build_flags = ${env.build_flags}
%s
"""

configs = glob.glob('src/*.yaml')
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
    for key in entries:
        value = entries[key]
        if type(value) == str:
            if key[-1] == '!':
                flags += "    -D%s=%s\n" % ((name+'_'+key[:-1]).upper(), value)
            else:
                parts = key.split('_')
                if parts[-1] == 'pin':
                    # Here for legacy reasons, ! should be used instead in the future
                    flags += "    -D%s=%s\n" % ((name+'_'+key).upper(), value)
                else:
                    flags += "    '-D%s=\"%s\"'\n" % (
                        (name+'_'+key).upper(), value)
        elif type(value) == bool:
            flags += "    -D%s=%s\n" % ((name+'_'+key).upper(),
                                        {False: 0, True: 1}[value])
        elif type(value) == list:
            data = json.dumps(value).replace("\"", "\\\\\"")
            flags += "    '-D%s=\"%s\"'\n" % ((name+'_'+key).upper(), data)
        else:
            flags += "    -D%s=%s\n" % ((name+'_'+key).upper(), value)
    flags += "    \n"

    return flags


for config in configs:
    ini = ''
    srcs = ''
    flags = ''
    board_name = os.path.basename(board).split('.')[0]
    board_names.append(board_name)

    with open(board, 'r') as f:
        try:
            config = yaml.load(f, Loader=yaml.Loader)

            if 'network' in config:
                network_config = config['network']
                network_config[network_config['type']] = 1
                flags += "    ; Network\n"
                flags += appendFlags(network_config, 'network')
                if network_config['type'] == 'ethernet':
                    flags += '    -DNETWORK_CONTROLLER=ETHERNET_CONTROLLER_W5X00'
                else:
                    flags += '    -DNETWORK_CONTROLLER=NETWORK_CONTROLLER_WIFI'
                flags += "\n\n"
            else:
                print('No network configuration in ' + board)

            if 'modules' in config and config['modules'] is not None:
                for module in config['modules']:
                    if not os.path.exists('src/%s.cpp' % module):
                        print('! Module not found: %s' % module)
                        exit(1)
                    srcs += "    +<%s.cpp>\n" % module

                    if module=="leds" and "animations" not in config['modules'] :
                        srcs += "    +<%s.cpp>\n" % "animations"
                    

                    if config['modules'][module]:
                        flags += "    ; From %s\n" % module
                        flags += appendFlags(config['modules'][module], module)
                        # print(flags)

            ini = ini_template % (board_name, srcs, flags)
            ini_filename = 'boards/platformio/%s.ini' % board_name

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
            print('Yaml file ' + board + ' contain errors: ' + "\n\n" + str(e))

inis = glob.glob('boards/platformio/*.ini')
for ini in inis:
    board_name = os.path.basename(ini).split('.')[0]
    if board_name not in board_names:
        print('Removing %s because there is no corresponding yaml file' % ini)
        os.unlink(ini)

if changed:
    print('! There were some outdated INI files, they were re-generated, please restart pio')
    exit(1)