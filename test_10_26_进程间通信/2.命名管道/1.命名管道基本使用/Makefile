.PHONY:all
all:sever client

sever:sever.c
	gcc -o $@ $^ -std=c99


client:client.c
	gcc -o $@ $^ -std=c99

.PHONY:clean

clean:
	rm -rf sever client myfifo
