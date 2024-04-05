all: test
	./test

test: test.c ../src/http_parser.c ../src/utils.c
	gcc test.c ../src/http_parser.c ../src/utils.c -o test /usr/local/lib/libtap.a

clean:
	rm test
