.PHONY:all

all:test

test:test.cpp ThreadPool.cpp
	g++ -o $@ $^ -lpthread -std=c++17 -g

.PHONY:clean

clean:
	rm -rf test
