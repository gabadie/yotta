
import sys
from twisted.internet.endpoints import TCP4ServerEndpoint
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

    def __init__(self, listen_port=5000):
        self.computers = 1
        self.threads = 1
        self.listen_port = listen_port
        self.tcp4_endpoint = TCP4ServerEndpoint(reactor, self.listen_port)
        self.tcp6_endpoint = None
        self.dictate_factory = dictate.DeamonProtocolFactory()

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
    deamon = Deamon()

    r = deamon.main()

    assert isinstance(r, int)

    sys.exit(r)
