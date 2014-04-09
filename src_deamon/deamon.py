
import logging
import sys
from twisted.internet import reactor
# TODO: import argparse

import dictate


class Deamon(object):
    """ Deamon general instance

    members:
        computers = int
        threads = int
        listen_port = int
        tcp4_endpoint = twisted.internet.endpoints.TCP4ServerEndpoint
        tcp6_endpoint = twisted.internet.endpoints.TCP6ServerEndpoint
        dictate_factory = dictate.DeamonProtocolFactory
    """

    def __init__(self, logger=None):
        self.computers = 1
        self.threads = 1
        self.logger = logger

        if self.logger == None:
            self.logger = logging.getLogger(__name__)

    def listen(self, port, protocol_factory):
        assert port >= 0
        assert port <= 65535
        assert protocol_factory != None
        assert protocol_factory.deamon == None

        protocol_factory.deamon = self

        return reactor.listenTCP(port, protocol_factory)

    def main(self):
        reactor.run()

        return 0

def man():
    print 'syntax:'
    print '    {} [--port <PORT>]'.format(sys.argv[0])
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

    i = 0

    while i < len(args):
        if args[i] == '--port':
            args_assert_exist(args, i + 1)

            port = int(args[i + 1])
            i += 2

        else:
            args_error(args, i, 'unknown parameter {}'.format(args[i]))

    args_assert(port != None)


    logger = logging.getLogger(sys.argv[0])
    logger.setLevel(logging.INFO)

    formatter = logging.Formatter("%(asctime)s - %(levelname)s: %(message)s")

    stream = logging.StreamHandler()
    stream.setFormatter(formatter)
    stream.setLevel(logging.INFO)
    logger.addHandler(stream)


    dictate_factory = dictate.DeamonProtocolFactory()

    deamon = Deamon()
    port = deamon.listen(port, dictate_factory)

    deamon.logger.info('listening on port: {}'.format(port.getHost().port))

    r = deamon.main()

    print ''

    return r


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)

    r = main(sys.argv[1:])

    assert isinstance(r, int)

    sys.exit(r)
