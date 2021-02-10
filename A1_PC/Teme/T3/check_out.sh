#!/bin/bash
cd Release
make
cd ..
cp Release/image_editor Checker\ Tema3/
cd Checker\ Tema3
./check.sh
