CC = gcc
CFLAGS=-Wall -Wextra
TARGET=SimpleProgram
FILE1 = SimpleProgram.txt
FILE2 = /home/francisco/OC/Lab1/tests/results_L2_1W.txt

all:
	$(CC) $(CFLAGS) SimpleProgram.c cacheL2.c -o $(TARGET)

clean:
	rm $(TARGET) $(FILE1)

run:
	./$(TARGET) > $(FILE1)

resposta:
	@diff -u $(FILE1) $(FILE2) || echo "Differences found. Please check the output."