
import struct

from daemon import Daemon
import dictate


def test_frame_error():
    msg = 'hello world'

    frame = dictate.frame_error(msg)

    theoric_frame = ''
    theoric_frame += '\x00\x00'
    theoric_frame += '\x0B\x00\x00\x00\x00\x00\x00\x00'
    theoric_frame += struct.pack('{}s'.format(len(msg)), msg)

    assert len(theoric_frame) == 2 + 8 + len(msg)
    assert frame == theoric_frame

def test_deamon_info():
    daemon = Daemon()
    daemon.computers = 3
    daemon.threads = 7

    frame = dictate.deamon_info(daemon)

    theoric_frame = ''
    theoric_frame += '\x00\x10'
    theoric_frame += '\x10\x00\x00\x00\x00\x00\x00\x00'
    theoric_frame += '\x03\x00\x00\x00\x00\x00\x00\x00'
    theoric_frame += '\x07\x00\x00\x00\x00\x00\x00\x00'

    assert frame == theoric_frame
