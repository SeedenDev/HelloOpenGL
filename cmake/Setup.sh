#!/bin/bash

pushd ../
cmake --preset "linux-x64-gcc-ninja" # can be gcc/clang
popd