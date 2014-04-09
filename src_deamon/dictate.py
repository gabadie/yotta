
import struct
from twisted.internet.protocol import Protocol
from twisted.internet.protocol import Factory

# struct.pack("qQ", hello)

LABEL_ERROR = 0x0000
LABEL_DEAMON_INFO = 0x1000

# ------------------------------------------------------------------------------ generate fram to send

def error(msg):
    """ Generates LABEL_ERROR's trame with a given message
    """
    assert len(msg) < 256 # the yotta library doesn't support longer

    return struct.pack('H', LABEL_ERROR) + struct.pack('Q', len(msg)) + msg

def deamon_info(deamon):
    """ Generates LABEL_DEAMON_INFO's trame with a given Deamon
    """
    # https://docs.python.org/2/library/struct.html

    return struct.pack('H', LABEL_DEAMON_INFO) + struct.pack('QQQ', 16, deamon.computers, deamon.threads)


# ------------------------------------------------------------------------------ dictate instruction

class InstAbstact(object):
    """ abstract dictate instruction

    members:
        frame_size = int
        protocol = DeamonProtocol

    properties:
        deamon = deamon.Deamon
        logger = logging.Logger
    """

    def __init__(self, protocol, frame_size):
        assert protocol != None
        assert frame_size > 0

        self.protocol = protocol
        self.frame_size = frame_size
        self.frame_cursor = 0

    @property
    def deamon(self):
        assert self.protocol != None

        return self.protocol.deamon

    @property
    def logger(self):
        return self.deamon.logger

    def receive(self, data):
        """ receive data from the protocol.
        """

        assert False # should have been implemented


class InstExecBinaries(InstAbstact):
    """ binary receiving instruction
    """

    def __init__(self, protocol, frame_size):
        InstAbstact.__init__(self, protocol, frame_size)

    def receive(self, data):
        assert False # todo


# ------------------------------------------------------------------------------ dictate instructions' map

instruction = dict()
instruction[0x2000] = InstExecBinaries


# ------------------------------------------------------------------------------ dictate protocol instance

class DeamonProtocol(Protocol):
    """ Twisted's dictate protocol

    members:
        factory = DeamonProtocolFactory
        instruction = InstAbstact # the current instruction

    properties:
        deamon = deamon.Deamon
        logger = logging.Logger
    """

    def __init__(self, factory):
        assert factory != None
        assert factory.deamon != None

        self.factory = factory
        self.instruction = None

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
        #self.transport.write(data)

        while True:
            if self.instruction != None:
                frame_size = self.instruction.frame_size
                frame_cursor = self.instruction.frame_cursor
                frame_remaining_size = frame_size - frame_cursor

                instruction_data = data

                if len(instruction_data) > frame_remaining_size:
                    instruction_data = data[0:frame_remaining_size]
                    data = data[frame_remaining_size:]

                else:
                    data = ''

                self.instruction.receive(instruction_data)

                self.instruction.frame_cursor += len(instruction_data)

                if self.instruction.frame_cursor < self.instruction.frame_size:
                    break

                del self.instruction
                self.instruction = None

            if len(data) == 0:
                break

            # TODO: fetch next instruction and process it

        return



# ------------------------------------------------------------------------------ dictate protocol factory

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
