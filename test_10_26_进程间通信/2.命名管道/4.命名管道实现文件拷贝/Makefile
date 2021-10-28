.PHONY:all
all:server client
server:server.c
	gcc -o $@ $^ -std=c99
client:client.c
	gcc -o $@ $^ -std=c99

.PHONY:clean
clean:
	rm -rf server client myfifo

