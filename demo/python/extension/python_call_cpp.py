#!/usr/bin/python
#encoding: utf-8

import sys
import great_module as gm

print gm.inc(1)
print gm.str_len('abcd')
print gm.offset('abcd')
print gm.sign64to32('11022301334815100767') ## 1425526623,2566329514 11022301334815100767

sys.exit(0)

