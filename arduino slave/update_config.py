#!/usr/bin/env python
import os
import glob
import json

id=''

code_dictionnary = {"actuator" : '1',
                    "sensor" : '0',
                     "led" : '000',
                     "motor" : '001',
                     "potentiometer": '000',
                     "accelerometer" : '001',}

ini_template = """
[env:%s]
build_src_filter =
    +<../main/*>
%s
    
build_flags = ${env.build_flags}
%s
"""

configs = glob.glob('pioconfig/*.json')
config_names = []
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

def to_four_bits(id) :
    x=bin(id)
    x=x[2:]
    n=len(x)
    if(n<4) :
        for k in range(4-n) :
            x='0'+x
    return x

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
    config_name = os.path.basename(config).split('.')[0]
    config_names.append(config_name)

    with open(config, 'r') as f:
        try:
            config = json.load(f)
            if 'device' in config and config['device'] is not None:
                for device in config['device']:
                    if not os.path.exists('src/%s.cpp' % device):
                        print('! Module not found: %s' % device)
                        exit(1)
                    srcs += "    +<%s.cpp>\n" % device

                    id=id+code_dictionnary[device]
                    for name in config['device'][device]:
                        if not os.path.exists('src/%s.cpp' % name):
                            print('! Module not found: %s' % name)
                            exit(1)
                        srcs += "    +<%s.cpp>\n" % name
                        id=id+code_dictionnary[name]
                        if not ("id" in config['device'][device][name]): 
                            flags += "    ; From %s\n" % name
                            flags += appendFlags(config['device'][device][name], name)
                            # print(flags)
                        else : 
                            id='0b'+id+to_four_bits(config['device'][device][name]["id"])
                            print(id)
                            config['device'][device][name]["id"]=int(id,2)
                            flags += "    ; From %s\n" % name
                            flags += appendFlags(config['device'][device][name], name)
            ini = ini_template % (config_name, srcs, flags)
            ini_filename = 'pioconfig/platformio_ini/%s.ini' % config_name

            before = None
            if os.path.isfile(ini_filename):
                f = open(ini_filename, 'r')
                before = f.read()
                f.close()

            if before != ini:
                f = open(ini_filename, 'w')
                f.write(ini)
                f.close()
                print('- Wrote %s.ini' % config_name)

                if before != ini:
                    changed = True

        except ValueError as e:
            print('Json file ' + config + 'may be wrong ' + "\n\n" + str(e))

inis = glob.glob('pioconfig/platformio_ini/*.ini')
for ini in inis:
    config_name = os.path.basename(ini).split('.')[0]
    if config_name not in config_names:
        print('Removing %s because there is no corresponding json file' % ini)
        os.unlink(ini)

if changed:
    print('! There were some outdated INI files, they were re-generated, please restart pio')
    exit(1)