CC=gcc
CFLAGS=-Wall -g

all: temel bellek benchmark deney backend

temel:
	$(CC) $(CFLAGS) test/test_temel.c src/*.c -o test/test_temel

bellek:
	$(CC) $(CFLAGS) test/test_bellek.c src/*.c -o test/test_bellek

benchmark:
	$(CC) $(CFLAGS) test/benchmark.c src/*.c -o test/benchmark

deney:
	$(CC) $(CFLAGS) test/deney.c src/*.c -o test/deney

backend:
	$(CC) -shared -fPIC -o ministream.so src/*.c

clean:
	rm -f test/test_temel test/test_bellek test/benchmark test/deney ministream.so
