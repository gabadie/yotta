
import sys
from twisted.internet.endpoints import TCP4ServerEndpoint
from twisted.internet.endpoints import TCP6ServerEndpoint
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

    def __init__(self, ipv4_port=None, ipv6_port=None):
        self.computers = 1
        self.threads = 1
        self.tcp4_endpoint = None
        self.tcp6_endpoint = None

        if ipv4_port != None:
            self.tcp4_endpoint = TCP4ServerEndpoint(reactor, ipv4_port)

        if ipv6_port != None:
            self.tcp6_endpoint = TCP6ServerEndpoint(reactor, ipv6_port)

    @property
    def tcp_endpoints(self):
        endpoints = []

        if self.tcp4_endpoint:
            endpoints.append(self.tcp4_endpoint)

        if self.tcp6_endpoint:
            endpoints.append(self.tcp6_endpoint)

        return endpoints

    def listen(self, protocol_factory):
        assert len(self.tcp_endpoints) > 0
        assert protocol_factory.deamon == None

        for endpoint in self.tcp_endpoints:
            endpoint.listen(protocol_factory)

        protocol_factory.deamon = self

        return True


    def main(self):
        reactor.run()

        return 0

def man():
    print 'syntax:'
    print '    {} [-ipv4 <IPV4_PORT>] [-ipv6 <IPV6_PORT>]'.format(sys.argv[0])
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
    ipv4_port = None
    ipv6_port = None

    i = 0

    while i < len(args):
        if args[i] == '-ipv4':
            args_assert_exist(args, i + 1)

            ipv4_port = int(args[i + 1])
            i += 2

        elif args[i] == '-ipv6':
            args_assert_exist(args, i + 1)

            ipv6_port = int(args[i + 1])
            i += 2

        else:
            args_error(args, i, 'unknown parameter {}'.format(args[i]))

    args_assert(ipv4_port != None or ipv6_port != None)

    dictate_factory = dictate.DeamonProtocolFactory()

    deamon = Deamon(ipv4_port=ipv4_port, ipv6_port=ipv6_port)
    deamon.listen(dictate_factory)

    r = deamon.main()

    print ''

    return r


if __name__ == "__main__":
    r = main(sys.argv[1:])

    assert isinstance(r, int)

    sys.exit(r)
