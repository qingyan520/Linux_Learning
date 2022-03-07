.PHONY:all
all:TcpServer TcpClient
TcpServer:TcpServer.cc
	g++ -o $@ $^ -std=c++11
TcpClient:TcpClient.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f TcpServer TcpClient
