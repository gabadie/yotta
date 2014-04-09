
import struct
from twisted.internet.protocol import Protocol
from twisted.internet.protocol import Factory

# struct.pack("qQ", hello)

LABEL_DEAMON_INFO = 0x1000


def deamon_info(deamon):
    """ Generates LABEL_DEAMON_INFO's trame with a given Deamon
    """
    # https://docs.python.org/2/library/struct.html

    return struct.pack('HQQQ', LABEL_DEAMON_INFO, 16, deamon.computers, deamon.threads)


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

        Protocol.__init__(self)

        self.state = "WAITING"
        self.factory = factory

    @property
    def deamon(self):
        assert self.factory != None
        assert self.factory.deamon != None

        return self.factory.deamon

    def connectionMade(self):
        self.transport.write("Connection Made\r\n")
        self.transport.sendInformation(deamon_info(self.deamon))

    def dataReceived(self, data):
        if self.state == "WAITING":
            datasent = deamon_infos()
            self.sendInformation(datasent)
            self.state = "READY"
        else :
            pass
            # write code to create new whispering socket with parameters.

    def connectionLost(self, reason):
        self.state =="WAITING"

    def sendInformation(self, data):
        self.transport.write(struct.pack("!L",data.data_label))


class DeamonProtocolFactory(Factory):
    """ Twisted's dictate protocol factory

    members:
        deamon = deamon.Deamon
    """

    def __init__(self, deamon):
        assert deamon != None

        #Factory.__init__(self)

        self.deamon = deamon

    def buildProtocol(self, addr):
        return DeamonProtocol(self)
