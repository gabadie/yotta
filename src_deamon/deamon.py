
import sys
from twisted.internet.endpoints import TCP4ServerEndpoint
from twisted.internet import reactor

import dictate


class Deamon(object):
    """ Deamon general instance

    members:
        computers = int
        threads = int
    """

    def __init__(self):
        self.computers = 1
        self.threads = 1

    def main(self):
        endpoint = TCP4ServerEndpoint(reactor,8037)
        endpoint.listen(dictate.DeamonProtocolFactory())

        reactor.run()

        return 0


if __name__ == "__main__":
    deamon = Deamon()

    r = deamon.main()

    assert isinstance(r, int)

    sys.exit(r)
