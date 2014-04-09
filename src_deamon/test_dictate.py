
import struct

from deamon import Deamon
import dictate

def test_deamon_info():
    deamon = Deamon()
    deamon.computers = 3
    deamon.threads = 7

    deamon_info_tram = dictate.deamon_info(deamon)

    theoric_tram = ''
    theoric_tram += '\x00\x10'
    theoric_tram += '\x10\x00\x00\x00\x00\x00\x00\x00'
    theoric_tram += '\x03\x00\x00\x00\x00\x00\x00\x00'
    theoric_tram += '\x07\x00\x00\x00\x00\x00\x00\x00'

    assert deamon_info_tram == theoric_tram
