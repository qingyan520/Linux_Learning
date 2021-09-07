Mytest:test.o main.o
		gcc -o Mytest test.o main.o  -std=c99
main.o:main.c
		gcc -c main.c 
test.o:test.c
		gcc -c test.c 
.PHONY:clean
clean:
		rm -rf *.o Mytest
