
import struct
from twisted.internet.protocol import Protocol
from twisted.internet.protocol import Factory

# struct.pack("qQ", hello)

LABEL_DEAMON_INFO = 0x1000


def deamon_info(deamon):
    """ Generates LABEL_DEAMON_INFO's trame with a given Deamon
    """
    # https://docs.python.org/2/library/struct.html

    return struct.pack('H', LABEL_DEAMON_INFO) + struct.pack('QQQ', 16, deamon.computers, deamon.threads)


class DeamonProtocol(Protocol):
    """ Twisted's dictate protocol

    members:
        factory = DeamonProtocolFactory

    properties:
        deamon = deamon.Deamon
    """

    def __init__(self, factory):
        assert factory != None
        assert factory.deamon != None

        self.factory = factory

    @property
    def deamon(self):
        assert self.factory != None
        assert self.factory.deamon != None

        return self.factory.deamon

    @property
    def logger(self):
        return self.deamon.logger

    @property
    def peer_addr(self):
        addr = self.transport.getPeer()

        return '{}:{}'.format(addr.host, addr.port)

    def connectionMade(self):
        self.transport.write(deamon_info(self.deamon))
        pass

    def connectionLost(self, reason):
        addr = self.transport.getPeer()

        self.logger.info("connection from {} losted: {}".format(self.peer_addr, reason))

    def dataReceived(self, data):
        # TODO: receive data
        pass

    def sendInformation(self, data):
        # TODO; send data
        pass


class DeamonProtocolFactory(Factory):
    """ Twisted's dictate protocol factory

    members:
        deamon = deamon.Deamon
    """

    def __init__(self):
        #Factory.__init__(self)

        self.deamon = None

    @property
    def logger(self):
        return self.deamon.logger

    def buildProtocol(self, addr):
        assert self.deamon != None

        self.logger.info('connection from {}:{}'.format(addr.host, addr.port))

        return DeamonProtocol(self)
