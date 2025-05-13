#!/bin/bash
import os

basedir = os.path.dirname(__file__)

def run_cmd(cmd):
    full_cmd = ''
    if basedir!='':
        full_cmd = 'cd '+basedir+'; '
    full_cmd += 'cd ..; '+cmd
    print(full_cmd)
    os.system(full_cmd)


run_cmd('rm -rf build/')
run_cmd('rm -rf build2/')

run_cmd('rm -rf index.*')

run_cmd('make clean')
