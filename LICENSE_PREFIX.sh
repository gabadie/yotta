#!/bin/sh
#
# This script apply LICENSE_PREFIX to all Yotta's source files
#

python candy_makefile/utils/src_prefix.py -v c/c++ LICENSE_PREFIX demos/*/*.{c,h}
python candy_makefile/utils/src_prefix.py -v c/c++ LICENSE_PREFIX src/*.{c,h}
python candy_makefile/utils/src_prefix.py -v c/c++ LICENSE_PREFIX src/*/*.{c,h}
python candy_makefile/utils/src_prefix.py -v c/c++ LICENSE_PREFIX tests/*.{c,h}
python candy_makefile/utils/src_prefix.py -v python LICENSE_PREFIX src_daemon/*.py
