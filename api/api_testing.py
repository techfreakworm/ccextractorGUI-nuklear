import ccextractor as cc
import os
import time
import sys
s =  cc.api_init_options()
cc.check_configuration_file(s)
for i in sys.argv[1:]:
    cc.api_add_param(s,str(i))
#cc.setstdout(s)
compile_ret = cc.compile_params(s,len(sys.argv[1:]));
start_ret = cc.api_start(s);
#print cc.cc_to_python_get_subs_number_of_lines()
#printing python_subs.subs
#for item in xrange(cc.cc_to_python_get_subs_number_of_lines()):
#    print cc.cc_to_python_get_sub(item)

#os.system('clear')
