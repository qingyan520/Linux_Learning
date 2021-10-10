.PYONY:all
all:test_pipe test_

test_pipe:test_pipe.c
	gcc -o test_pipe test_pipe.c
test_:test.c
	gcc -o test_ test.c

.PYONY:clean
clean:
	rm -rf test_pipe

