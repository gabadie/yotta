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

import logging
import multiprocessing
import sys
from twisted.internet import reactor
# TODO: import argparse

import dictate


def default_logger():
    log_format = '[%(asctime)s] %(levelname)s - %(message)s'
    log_formatter = logging.Formatter(log_format)
    logging.basicConfig(level=logging.DEBUG, format=log_format)
    logger = logging.getLogger(sys.argv[0])

    logger.handlers = []
    logger.setLevel(logging.INFO)

    return logger

class Daemon(object):
    """ Deamon general instance

    members:
        computers = int
        threads = int
        logger = dictate.DeamonProtocolFactory
    """

    def __init__(self, logger=default_logger()):
        self.computers = 1
        self.threads = multiprocessing.cpu_count()
        self.logger = logger

    def listen(self, port, protocol_factory):
        assert port >= 0
        assert port <= 65535
        assert protocol_factory != None
        assert protocol_factory.daemon == None

        protocol_factory.daemon = self

        return reactor.listenTCP(port, protocol_factory)

    def main(self):
        reactor.run()

        return 0

def man():
    print 'syntax:'
    print '    {} (--port <PORT>, --out-port)*'.format(sys.argv[0])
    print ''

def args_assert(condition=False):
    if condition:
        return

    man()

    sys.exit(1)

def args_error(args, i, message):
    print 'argument syntax error: {}'.format(message)
    print '    {} {}'.format(sys.argv[0], ' '.join(args))

    pos = len(sys.argv[0]) + 1 + i + sum([len(args[x]) for x in xrange(i)])

    print '    {}^'.format(' ' * pos)
    print ''

    args_assert()

def args_assert_exist(args, i):
    if i < len(args):
        return

    args_error(args, i, 'missing argument')

def main(args):
    port = None
    print_port_on_first_line = False

    i = 0

    while i < len(args):
        if args[i] == '--port':
            args_assert_exist(args, i + 1)

            port = int(args[i + 1])
            i += 2

        elif args[i] == '--out-port':
            print_port_on_first_line = True
            i += 1

        else:
            args_error(args, i, 'unknown parameter {}'.format(args[i]))

    args_assert(port != None)

    dictate_factory = dictate.DeamonProtocolFactory()

    daemon = Daemon(logger=default_logger())
    port = daemon.listen(port, dictate_factory)

    if print_port_on_first_line:
        print str(port.getHost().port)

    daemon.logger.info('listening on port: {}'.format(port.getHost().port))

    r = daemon.main()

    print ''

    return r


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)

    r = main(sys.argv[1:])

    assert isinstance(r, int)

    sys.exit(r)
