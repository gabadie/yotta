
import sys
from twisted.internet.endpoints import TCP4ServerEndpoint
from twisted.internet.endpoints import TCP6ServerEndpoint
from twisted.internet import reactor

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
        self.dictate_factory = dictate.DeamonProtocolFactory(self)

        if ipv4_port != None:
            self.tcp4_endpoint = TCP4ServerEndpoint(reactor, ipv4_port)

        if ipv6_port != None:
            self.tcp6_endpoint = TCP6ServerEndpoint(reactor, ipv6_port)

        for endpoint in self.tcp_endpoints:
            endpoint.listen(self.dictate_factory)

    @property
    def tcp_endpoints(self):
        endpoints = []

        if self.tcp4_endpoint:
            endpoints.append(self.tcp4_endpoint)

        if self.tcp6_endpoint:
            endpoints.append(self.tcp6_endpoint)

        return endpoints

    def main(self):
        reactor.run()

        return 0


if __name__ == "__main__":
    deamon = Deamon(ipv4_port=5000)

    r = deamon.main()

    assert isinstance(r, int)

    sys.exit(r)
