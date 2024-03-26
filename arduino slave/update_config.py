#!/usr/bin/env python
import os
import glob
import json
import config as conf
id=''


ini_template = """
[env:%s]
build_src_filter =
    +<../main/*>
%s
    
build_flags = ${env.build_flags}
%s
"""

configs = glob.glob('pioconfig/config.json')
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

def appendFlags(entries, name,val=-1):
    if val==-1 :
        flatten(entries)

    flags = ''
    flags += "    -D%s=%s\n" % ((name).upper(),"1")

    for key in entries:
        value = entries[key]
        if val==-1 :
            if type(value) == str:
                if key[-1] == '!':
                    flags += "    -D%s=%s\n" % ((name+'_'+key[:-1]).upper(), value)
                else :
                    flags += "    -D%s=%s\n" % ((name+'_'+key).upper(), int(value))
            else:
                flags += "    -D%s=%s\n" % ((name+'_'+key).upper(), value)
        else : 
            flags += "    -D%s=%s\n" % ((name+'_'+key).upper(), val)
    flags += "    \n"

    return flags


for config in configs:
    ini = ''
    srcs = ''
    flags = ''
    id=''
    config_name = os.path.basename(config).split('.')[0]
    config_names.append(config_name)

    with open(config, 'r') as f:
        try:
            config = json.load(f)
            if 'device' in config and config['device'] is not None:
                for device in config['device']:
                    if device =="info" :
                        for info_key in config['device']["info"] : 
                            if info_key=="subscription" : 
                                subscrip = ""
                                print(len(config['device']["info"]))
                                for sub_key in config['device']["info"]["subscription"] :
                                    sub=config['device']["info"]["subscription"][sub_key]                                    
                                    subscrip+=bin(int(conf.subject_dictionnary[sub]))[2:].zfill(8)
                                flags += "    ; From %s\n" % device
                                flags += appendFlags(config['device'][device], device, int(subscrip,2))


                                # print(flags).
                    else : 
                        
                        if not os.path.exists('src/%s.cpp' % device):
                            print('! Module not found: %s' % device)
                            exit(1)
                        srcs += "    +<%s.cpp>\n" % device

                        print(device)
                        
                        id=id+conf.config_nodes[device]["type"] + conf.config_nodes[device]["fam"]+ "000"
                        if (config['device'][device]["id"]) : 
                            id='0b'+id+to_four_bits(config['device'][device]["id"])
                            config['device'][device]["id"]=int(id,2)

                        flags += "    ; From %s\n" % device
                        flags += appendFlags(config['device'][device], device)
                        flags += "    -D%s=%s\n" % (("info"+'_'+"production").upper(), int(conf.config_nodes[device]["produced_subject"]))
                        flags += "    \n"
                        # print(flags)

                        
                        
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