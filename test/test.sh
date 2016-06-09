#!/usr/bin/env sh

../gpiocfg_install.py > /tmp/gpiocfg.py
chmod +x /tmp/gpiocfg.py
/tmp/gpiocfg.py -d ~/Documents/workspace/c2eT/STCube/ -c ./
