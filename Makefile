all: dynamic

dynamic: dynamic.o 
	gcc -o dynamic dynamic.o

dynamic.o: dynamic.c
	gcc -c dynamic.c


clean:
	rm -rf *.o dynamic
