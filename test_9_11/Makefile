myproc:test.o
		gcc -o myproc test.o
test.o:test.c
		gcc -c test.c -o test.o
.PHONY:clean
clean:
		rm -rf *.o myproc
