
import os
import struct
import subprocess
import tempfile
from twisted.internet.protocol import Protocol
from twisted.internet.protocol import Factory


# ------------------------------------------------------------------------------ dictate label

LABEL_ERROR = 0x0000
LABEL_DEAMON_INFO = 0x1000
LABEL_MASTER_BINARIES = 0x2000
LABEL_MASTER_START = 0x2010

# ------------------------------------------------------------------------------ dictate's frame generators

def frame_error(msg):
    """ Generates LABEL_ERROR's trame with a given message
    """
    assert len(msg) < 256 # the yotta library doesn't support longer

    return struct.pack('H', LABEL_ERROR) + struct.pack('Q', len(msg)) + struct.pack('{}s'.format(len(msg)), msg)

def deamon_info(daemon):
    """ Generates LABEL_DEAMON_INFO's trame with a given daemon
    """
    # https://docs.python.org/2/library/struct.html

    return struct.pack('H', LABEL_DEAMON_INFO) + struct.pack('QQQ', 16, daemon.computers, daemon.threads)


# ------------------------------------------------------------------------------ dictate instruction

class InstAbstract(object):
    """ abstract dictate instruction

    members:
        frame_size = int
        frame_cursor = int
        protocol = DeamonProtocol

    properties:
        daemon = daemon.daemon
        logger = logging.Logger
    """

    def __init__(self, protocol, frame_size):
        assert protocol != None
        assert frame_size > 0

        self.protocol = protocol
        self.frame_size = frame_size
        self.frame_cursor = 0

    @property
    def daemon(self):
        assert self.protocol != None

        return self.protocol.daemon

    @property
    def logger(self):
        return self.daemon.logger

    def receive(self, data):
        """ receive data from the protocol.
        """

        assert False # should have been implemented

    def frame_error(self, msg):
        self.protocol.transport.write(frame_error(msg))


class InstSkip(InstAbstract):
    """ skeeping instruction used when an unknow command has been received
    """

    def __init__(self, protocol, frame_size):
        InstAbstract.__init__(self, protocol, frame_size)

    def __del__(self):
        self.logger.info("stop skeeping from {}".format(self.protocol.peer_addr))

    def receive(self, data):
        pass


class InstError(InstAbstract):
    """ error receiving instruction

    members:
        msg = str
    """

    def __init__(self, protocol, frame_size):
        InstAbstract.__init__(self, protocol, frame_size)

        self.msg = ''

    def receive(self, data):
        self.msg += data

        if len(self.msg) == self.frame_size:
            self.logger.info('received dictate error frame from {}: {}'.format(self.protocol.peer_addr, struct.unpack('s', self.msg)))


class InstExecBinaries(InstAbstract):
    """ binary receiving instruction

    members:
        binary_file = open()
        binary_path = str
    """

    def __init__(self, protocol, frame_size):
        InstAbstract.__init__(self, protocol, frame_size)

        self.binary_file, self.binary_path = tempfile.mkstemp()
        self.logger.info('receiving binary file {} from {}'.format(self.binary_path, self.protocol.peer_addr))

    def __del__(self):
        if self.binary_file == None:
            return

        self.logger.warning('unexpected InstExecBinaries destructor -> removing binary file {}'.format(self.binary_path))
        os.close(self.binary_file)
        os.remove(self.binary_path)

    def receive(self, data):
        os.write(self.binary_file, data)

        self.logger.info('receiving binary file {} from {} : {}'.format(self.binary_path, self.protocol.peer_addr, float(self.frame_cursor)/float(self.frame_size)))

        if self.frame_cursor + len(data) == self.frame_size:
            self.finish()

    def finish(self):
        assert self.binary_file != None

        os.close(self.binary_file)
        os.chmod(self.binary_path, 0744)
        self.binary_file = None
        self.protocol.binary_path = self.binary_path

        self.logger.info('binary file {} from {} fully received'.format(self.binary_path, self.protocol.peer_addr))

class InstStartSlave(InstAbstract):
    """ start slave execution
    """

    def __init__(self, protocol, frame_size):
        InstAbstract.__init__(self, protocol, frame_size)
        self.data = ''

    def receive(self, data):
        self.data += data

        if len(self.data) == self.frame_size:
            self.finish(self.data)

    def finish(self, data):
        assert len(data) == 2

        whisper_ip = self.protocol.transport.getPeer().host
        whisper_port = struct.unpack('H', data)[0]

        error_prefix = 'can\'t start {} \'s process: '.format(self.protocol.peer_addr)

        if self.protocol.binary_path == '':
            self.logger.info(error_prefix + 'binary file doesn\'t exists')
            self.frame_error('can\'t start executable: binary file doesn\'t exists')

        if self.protocol.process != None:
            self.logger.info(error_prefix + 'already started')
            self.frame_error('can\'t start executable: already started')

        if whisper_port == 0:
            self.logger.info(error_prefix + 'invalid whisper port 0')
            self.frame_error('can\'t start executable: invalid whisper port 0')

        cmd = [self.protocol.binary_path]
        cmd.append('--yotta')
        cmd.extend(['--yotta-client-ip', str(whisper_ip)])
        cmd.extend(['--yotta-client-port', str(whisper_port)])

        self.logger.info('starting {}\'s binary: {}'.format(self.protocol.peer_addr, ' '.join(cmd)))

        self.protocol.process = subprocess.Popen(cmd)


# ------------------------------------------------------------------------------ dictate instructions' map

instruction_map = dict()
instruction_map[LABEL_ERROR] = InstError
instruction_map[LABEL_MASTER_BINARIES] = InstExecBinaries
instruction_map[LABEL_MASTER_START] = InstStartSlave


# ------------------------------------------------------------------------------ dictate protocol instance

class DeamonProtocol(Protocol):
    """ Twisted's dictate protocol

    members:
        factory = DeamonProtocolFactory
        instruction = InstAbstract # the current instruction
        meta_buffer = str
        binary_path = str

    properties:
        daemon = daemon.daemon
        logger = logging.Logger
    """

    def __init__(self, factory):
        assert factory != None
        assert factory.daemon != None

        self.factory = factory
        self.instruction = None
        self.meta_buffer = ''
        self.binary_path = ''
        self.process = None

    def __del__(self):
        self.logger.info("deleting connection from {} ...".format(self.peer_addr))

        if self.instruction != None:
            del self.instruction

        if self.binary_path != '':
            os.remove(self.binary_path)
            self.logger.info("binary file {} from {} removed".format(self.binary_path, self.peer_addr))

    @property
    def daemon(self):
        assert self.factory != None
        assert self.factory.daemon != None

        return self.factory.daemon

    @property
    def logger(self):
        return self.daemon.logger

    @property
    def peer_addr(self):
        addr = self.transport.getPeer()

        return '{}:{}'.format(addr.host, addr.port)

    def connectionMade(self):
        self.logger.info('connection with {} made -> sending daemon informations'.format(self.peer_addr))

        self.transport.write(deamon_info(self.daemon))
        pass

    def connectionLost(self, reason):
        addr = self.transport.getPeer()

        self.logger.info("connection from {} losted: {}".format(self.peer_addr, reason))

        # we remove the protocol from the factory's protocol list to break up the circular dependency
        self.factory.remove(self)

        if self.instruction != None:
            # we cleans up self.instruction to break up the circular dependency
            del self.instruction

            self.instruction = None

        del self

    def dataReceived(self, data):
        while True:
            if self.instruction != None:
                # we already have got an instruction before,
                # then we continue to process it

                assert self.instruction.frame_size > self.instruction.frame_cursor

                frame_remaining_size = self.instruction.frame_size - self.instruction.frame_cursor

                instruction_data = data

                if len(instruction_data) > frame_remaining_size:
                    instruction_data = data[0:frame_remaining_size]
                    data = data[frame_remaining_size:]

                else:
                    data = ''

                self.instruction.receive(instruction_data)

                self.instruction.frame_cursor += len(instruction_data)

                assert self.instruction.frame_cursor <= self.instruction.frame_size

                if self.instruction.frame_cursor < self.instruction.frame_size:
                    break

                del self.instruction
                self.instruction = None

                self.meta_buffer = ''


            assert self.instruction == None

            if len(data) == 0:
                break

            meta_buffer_size = 8 + 2
            meta_buffer_remaining = max(meta_buffer_size - len(self.meta_buffer), 0)

            self.meta_buffer += data[0:meta_buffer_remaining]
            data = data[meta_buffer_remaining:]

            assert len(self.meta_buffer) <= meta_buffer_size

            if len(self.meta_buffer) != meta_buffer_size:
                # we didn't receive the entire meta buffer
                break

            frame_label = struct.unpack('H', self.meta_buffer[0:2])[0]
            frame_size = struct.unpack('Q', self.meta_buffer[2:])[0]

            if frame_label not in instruction_map:
                frame_label = '0x%04x' % frame_label
                error_msg = 'unknown frame label {}'.format(frame_label)
                error_frame = frame_error(error_msg)

                self.transport.write(error_frame)
                self.logger.error('unknown dictate protocol label {} received from {} -> ignoring frame'.format(frame_label, self.peer_addr))
                self.instruction = InstSkip(self, frame_size)

                continue

            self.instruction = instruction_map[frame_label](self, frame_size)

        return


# ------------------------------------------------------------------------------ dictate protocol factory

class DeamonProtocolFactory(Factory):
    """ Twisted's dictate protocol factory

    members:
        daemon = daemon.daemon
        protocols = list(DeamonProtocol)
    """

    def __init__(self):
        #Factory.__init__(self)

        self.daemon = None
        self.protocols = []

    @property
    def logger(self):
        return self.daemon.logger

    def buildProtocol(self, addr):
        assert self.daemon != None

        self.logger.info('new connection from {}:{}'.format(addr.host, addr.port))

        protocol = DeamonProtocol(self)

        self.protocols.append(protocol)

        return protocol

    def remove(self, protocol):
        assert protocol in self.protocols

        self.protocols.remove(protocol)
