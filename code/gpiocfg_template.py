#!/usr/bin/env python3
# -*- coding: utf-8-*

import argparse

import os
import tempfile
import subprocess
import base64

import time

hprogram = ""
cprogram = ""

if __name__ == "__main__":
  
  parser = argparse.ArgumentParser(
  description='Create C functions to configure STM32F1(?) gpio in fewer passes',
  formatter_class=argparse.ArgumentDefaultsHelpFormatter)

  parser.add_argument('-d', '--destination',
                      dest='project',
                      required=True,
                      metavar='DIR',
                      help='project directory')
  parser.add_argument('-c', '--configuration',
                      dest='configuration',
                      required=True,
                      metavar='DIR',
                      help='configuration directory')
  
  args = parser.parse_args()

  project = args.project
  configuration = args.configuration
  
  includes = [
              "Drivers/STM32F1xx_HAL_Driver/Inc/"
              , "Drivers/CMSIS/Device/ST/STM32F1xx/Include/"
              , "Drivers/CMSIS/Include/"
              , "Inc/"
              ]
  inc = list(map(lambda include: '-I' + os.path.abspath(os.path.join(project, include)), includes))
  inc.append('-I' + os.path.abspath(configuration))
  print(os.path.abspath(configuration))
  with tempfile.TemporaryDirectory() as d:
    ad = os.path.abspath(d)
    inc.append('-I' + ad)
  
    hfile = os.path.join(ad, 'gpiocfg.h')
    with open(hfile, 'bw') as hf:
      hf.write(base64.b64decode(hprogram))
    
    cfile = os.path.join(ad, 'gpiocfg.cpp')
    with open(cfile, 'bw') as hf:
      hf.write(base64.b64decode(cprogram))  
    
    obj = os.path.join(ad, 'gpiocfg')
    cmd = ['g++', '--std=c++11', '-DSTM32F103xB', '-o{}'.format(obj), '-xc++', '-']
    cmd = cmd + inc
    p = subprocess.run(cmd, input=base64.b64decode(cprogram))

    cmd = [obj, os.path.abspath(os.path.join(project, "Inc")), os.path.abspath(os.path.join(project, "Src"))]
    p = subprocess.run(cmd)
