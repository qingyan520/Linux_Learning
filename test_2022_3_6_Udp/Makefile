.PHONY:all
all:UdpServer UdpClient

UdpServer:UdpServer.cc
	g++ -o $@ $^ -std=c++11
UdpClient:UdpClient.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f UdpServer UdpClient
