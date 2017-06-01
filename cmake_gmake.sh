#!/bin/sh
set -x
mkdir -p build_gmake
cd build_gmake
cmake .. -G "Unix Makefiles"

