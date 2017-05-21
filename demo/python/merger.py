#!/usr/bin/python
""" merger script """

import sys

'''
input:
key \t 0 \t value1
key \t 1 \t value2

output:
key \t 1 \t value1 \space value2 if value found
key \t 0 \t value1
the key in the output is from the data source with flag 0

ex:
input
k1 \t 0
k1 \t 1 \t v1
k2 \t 0
k2 \t 1 \t v2
k3 \t 0
k4 \t 1 \t v4

output:
k1 \t 1 \t v1
k2 \t 1 \t v2
k3 \t 0

when using hadoop
    -partitioner "org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner" \
    -jobconf stream.num.map.output.key.fields=2 \
    -jobconf num.key.fields.for.partition=1 \
'''

d = {}

'''
flag_0: whether record from data source with flag 0 has value
flag_1: whether record from data source with flag 1 has value
sep: field seperator between value from data source 0 and data source 1 if both flag_0 and flag_1 are True, with space as default
'''
flag_0 = True
flag_1 = True
sep = ' '

def compose_fv(flag, v0, v1):
    """ compose flag and value when found """
    if flag_0:
        if flag_1:
            if v1 is None:
                return flag + '\t' + v0
            else:
                return flag + '\t' + v0 + sep + v1
        else:
            return flag + '\t' + v0
    else:
        if flag_1:
            if v1 is None:
                return flag
            else:
                return flag + '\t' + v1
        else:
            return flag


def process_dict(d, key):
    """ process a dict with only a key """
    if key + '_0' in d:
        vs0 = d[key + '_0']
        if key + '_1' in d:
            vs1 = d[key + '_1']
            for v0 in vs0:
                for v1 in vs1:
                    print >> sys.stdout, "%s\t%s" % (key, compose_fv('1', v0, v1))
        else:
            ## only v0
            for v0 in vs0:
                print >> sys.stdout, "%s\t%s" % (key, compose_fv('0', v0, None))
    ## clean
    if key + '_0' in d:
        del d[key + '_0']
    if key + '_1' in d:
        del d[key + '_1']


def main():
    """ main func """
    print >> sys.stderr, "flag_0: " + str(flag_0) + "; flag_1: " + str(flag_1)
    key_old = ''
    flag_old = ''
    value_old = ''
    key = ''
    flag = ''
    value = ''
    
    num = 0
    for line in sys.stdin:
        ## parser the line
        line = line.strip()
        terms = line.split('\t')
        key = terms[0]
        flag = terms[1]

        if flag == '0':
            if flag_0:
                value = terms[2]
            else:
                value = ''
        elif flag == '1':
            if flag_1:
                value = terms[2]
            else:
                value = ''
        else:
            print >> sys.stderr, "flag %s not supported" % flag
            sys.exit(1)
        
        ## a new key
        if key + '_0' not in d:
            if flag == '0':
                d[key + '_0'] = []
                d[key + '_0'].append(value)
        else:
        ## key aleady exists
            if flag == '1':
                if key + '_1' not in d:
                    d[key + '_1'] = []
                d[key + '_1'].append(value)
            else:
                d[key + '_0'].append(value)
    
        ## the first line
        if key_old == '':
            key_old = key
            flag_old = flag
            value_old = value
    
        ## other lines
        if key != key_old:
            process_dict(d, key_old)
            key_old = key
            flag_old = flag
            value_old = value

        num = num + 1 ## end of for line in sys.stdin
    
    ## the last possible key
    process_dict(d, key)

    print >> sys.stderr, "total input num: %d" % num

if __name__ == "__main__":
    if len(sys.argv) == 4:
        flag_0 = int(sys.argv[1])
        flag_1 = int(sys.argv[2])
        sep = sys.argv[3]
    elif len(sys.argv) == 3:
        flag_0 = int(sys.argv[1])
        flag_1 = int(sys.argv[2])
    else:
        pass
    main()

