#!/bin/bash

import os
import multiprocessing

basedir = os.path.dirname(__file__)

full_path = os.path.dirname(os.path.abspath(__file__))
executable_name = full_path.split('/')[-2]


def run_cmd(cmd):
    full_cmd = ''
    if basedir!='':
        full_cmd = 'cd '+basedir+'; '
    full_cmd += 'cd ..; '+cmd
    print(full_cmd)
    os.system(full_cmd)

run_cmd('python3 scripts/linux_clean.py')

run_cmd('mkdir build/')
run_cmd('cd build/; cmake ..')
run_cmd('cd build/; make -j'+str(multiprocessing.cpu_count()))

run_cmd('cd build/; ./'+executable_name)