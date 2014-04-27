#!/usr/bin/env python
# coding=utf8
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

import struct

from daemon import Daemon
import dictate


def test_frame_error():
    msg = 'hello world'

    frame = dictate.frame_error(msg)

    theoric_frame = ''
    theoric_frame += '\x00\x00'
    theoric_frame += '\x0B\x00\x00\x00\x00\x00\x00\x00'
    theoric_frame += struct.pack('{}s'.format(len(msg)), msg)

    assert len(theoric_frame) == 2 + 8 + len(msg)
    assert frame == theoric_frame

def test_deamon_info():
    daemon = Daemon()
    daemon.computers = 3
    daemon.threads = 7

    frame = dictate.deamon_info(daemon)

    theoric_frame = ''
    theoric_frame += '\x00\x10'
    theoric_frame += '\x10\x00\x00\x00\x00\x00\x00\x00'
    theoric_frame += '\x03\x00\x00\x00\x00\x00\x00\x00'
    theoric_frame += '\x07\x00\x00\x00\x00\x00\x00\x00'

    assert frame == theoric_frame
