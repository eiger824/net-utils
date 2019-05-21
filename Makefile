all: test

test: ipconv.c
	gcc -Wall $< -o $@

clean:
	rm -f test
