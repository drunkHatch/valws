all:
	gcc heapsort.c -o test -Wall
	gcc valws379.c -o valws379 -Wall
	valgrind --tool=lackey --trace-mem=yes ./test 2>&1 | ./valws379 16 5
clean:
	rm valws379 test
