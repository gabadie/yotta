
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


if __name__ == "__main__":
    dictate_factory = dictate.DeamonProtocolFactory()

    deamon = Deamon(ipv4_port=5000)
    deamon.listen(dictate_factory)

    r = deamon.main()

    assert isinstance(r, int)

    sys.exit(r)
