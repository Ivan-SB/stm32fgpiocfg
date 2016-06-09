#!/usr/bin/env python3
# -*- coding: utf-8-*

import argparse

import os
import re

import base64

def file2variable(variable, path):
  with open(path, 'br') as f:
    content = f.read()
    return '{} = {}\n'.format(variable, base64.b64encode(content))

def replacevariable(variable, path, content):
  varstr = file2variable(variable, path)
  content = re.sub('{}\s*=\s*""'.format(variable), varstr, content)
  return content

if __name__ == "__main__":
  parser = argparse.ArgumentParser(
  description='Add Cube libraries to an Eclipse program and import macros from a CubeMX project',
  formatter_class=argparse.ArgumentDefaultsHelpFormatter)

#   parser.add_argument('-d', '--destination',
#                       dest='project',
#                       required=True,
#                       metavar='DIR',
#                       help='project directory')
  
  args = parser.parse_args()
  
  mypath = os.path.dirname(os.path.realpath(__file__))
  ccodepath = os.path.join(mypath, 'code/gpiocfg.cpp')
  hcodepath = os.path.join(mypath, 'code/gpiocfg.h')
  pycodepath = os.path.join(mypath, 'code/gpiocfg_template.py')

  with open(pycodepath, 'r') as f:
    pyprogram = f.read()
  
  vcprogram = file2variable('cprogram', ccodepath)
  vhprogram = file2variable('hprogram', hcodepath)
  
  pyprogram = replacevariable('cprogram', ccodepath, pyprogram)
  pyprogram = replacevariable('hprogram', hcodepath, pyprogram)
  
  print(pyprogram)

                     