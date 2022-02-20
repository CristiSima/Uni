#!/bin/bash
# Copyright 2020 Darius Neatu (neatudarius@gmail.com)

pacman -Syu

echo aa
pacman -S                        \
    gcc                                   \
    python python-pip                     \
    perl                                    \
    make gdb valgrind

# all py packages json os pathlib recordclass subprocess sys
pip install wheel
pip install                           \
    argparse                                \
    pathlib2                                 \
    recordclass                     \
     python-sat
