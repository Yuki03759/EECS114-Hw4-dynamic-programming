all: EditDistance

EditDistance: EditDistance.o 
	gcc -o EditDistance EditDistance.o

EditDistance.o: EditDistance.c
	gcc -c EditDistance.c

Calculate: Calculate.o 
	gcc -o Calculate Calculate.o

Calculate.o: Calculate.c
	gcc -c Calculate.c

clean:
	rm -rf *.o Calculate EditDistance