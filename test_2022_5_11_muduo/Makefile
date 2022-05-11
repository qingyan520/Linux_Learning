.PHONY:all
all:ChatServer

ChatServer::ChatServer.cpp
	g++ -o $@ $^ -std=c++11 -lmuduo_net -lmuduo_base -lpthread

.PHONY:clean

clean:
	rm -rf ChatServer
