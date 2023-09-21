# compile the main.c 
CC = gcc

main: main.c
	$(CC) -o main main.c

compiletest: etape1.c etape2.c etape3.c etape4.c etape5.c
	$(CC) -o etape1 etape1.c
	$(CC) -o etape2 etape2.c
	$(CC) -o etape3 etape3.c
	$(CC) -o etape4 etape4.c
	$(CC) -o etape5 etape5.c
# test
test: 
	./tests_auto.sh
exec:
	./main fichiers_test/exemple1.o fichiers_test/exemple2.o

clean:
	rm -f main
cleantest:
	rm -f etape1 etape2 etape3 etape4 etape5

#clean all .txt files
cleantxt:
	rm -f *.txt


