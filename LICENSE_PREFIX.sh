#!/bin/sh
#
#       This file is part of the Yotta Distributed Computing Library Project.
#       Yotta Distributed Computing Library, named from the International
#       System of Units (SI) denoting a factor of 10^24, is a C library
#       designed for network-based distributed computing.
#       Yotta provides features to the developer in order to execute
#       simultaneously time-consuming algorithms across severals
#       clustered computers.
#
#       Co-founders:
#          Guillaume Abadie
#          Thierry Cantenot
#
#       Yotta Distributed Computing Library's official code repository is
#       available at <http://github.com/gabadie/yotta>.
#
#   GNU GPL v3 license:
#       Yotta is a free C library: you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation, either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#   github.com:
#       github.com is NEITHER property of the Yotta Distributed Computing
#       Library Project NOR any of its Co-founders'.  Please see
#       <http://github.com/site/terms>.
#

#
# This script applies LICENSE_PREFIX to all Yotta's source files
#

python candy_makefile/utils/src_prefix.py -v c/c++ LICENSE_PREFIX demos/*/*.{c,h}
python candy_makefile/utils/src_prefix.py -v c/c++ LICENSE_PREFIX src/*.{c,h}
python candy_makefile/utils/src_prefix.py -v c/c++ LICENSE_PREFIX src/*/*.{c,h}
python candy_makefile/utils/src_prefix.py -v c/c++ LICENSE_PREFIX tests/*.{c,h}
python candy_makefile/utils/src_prefix.py -v flist LICENSE_PREFIX {demos,src,src_daemon,tests}/*.flist demos/*/*.flist
python candy_makefile/utils/src_prefix.py -v python LICENSE_PREFIX src_daemon/*.py
python candy_makefile/utils/src_prefix.py -v shell LICENSE_PREFIX tests/*.sh $0
