
from twisted.internet.protocol import Protocol
from twisted.internet.protocol import Factory
from twisted.internet.endpoints import TCP4ServerEndpoint
from twisted.internet import reactor
from ctypes import *


class deamon_infos():
    def __init__(self): 
        self.data_label = c_uint64(0x1000)
        self.data_size = c_ulonglong(16)
        # number of available computers
        self.available_computers = c_ulonglong(1)
        # number of available threads
        self.available_threads = c_ulonglong(1)

class DeamonProtocol(Protocol):

    def __init__(self):
        self.state = "WAITING"

    def connectionMade(self):
        self.transport.write("Connection Made\r\n")
        
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
    def buildProtocol(self,addr):
        return DeamonProtocol()


def main():
    endpoint = TCP4ServerEndpoint(reactor,8037)
    endpoint.listen(DeamonProtocolFactory())
    reactor.run()

if __name__ == "__main__":
    main()