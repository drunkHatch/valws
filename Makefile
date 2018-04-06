all:
	gcc valws379.c -o valws379.bin -Wall
	ln -s valws379.bin valws379
reset:
	rm valws379 data.csv heap merge quick
clean:
	rm heap merge quick data.csv
plot:
	python3 lab121.py
heap:
	gcc heapsort.c -o heap -Wall
	valgrind --tool=lackey --trace-mem=yes ./heap 2>&1 | ./valws379 16 5
merge:
	gcc mergesort.c -o merge -Wall
	valgrind --tool=lackey --trace-mem=yes ./merge 2>&1 | ./valws379 16 5
quick:
	gcc quicksort.c -o quick -Wall
	valgrind --tool=lackey --trace-mem=yes ./quick 2>&1 | ./valws379 16 5
