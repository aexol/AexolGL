#!/bin/bash
args=("$@")

cd ./build/android_build/tests/${args[0]}
make -j 8
ant debug
adb install -r  bin/${args[0]}-debug.apk
date