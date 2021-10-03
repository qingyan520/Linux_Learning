myproc:myproc.o
	gcc -o myproc myproc.o -std=c99
myproc.o:myproc.c
	gcc -c myproc.c  -std=c99
.PYONY:clean
clean:
	rm -rf myproc *.o
