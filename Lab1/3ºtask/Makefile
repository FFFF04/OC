CC = gcc
CFLAGS=-Wall -Wextra
TARGET=SimpleProgram
FILE1 = SimpleProgram.txt
FILE2 = results_L2_2W.txt

all:
	$(CC) $(CFLAGS) SimpleProgram.c L3Cache.c -o $(TARGET)

clean:
	rm $(TARGET) $(FILE1)

run:
	./$(TARGET) > $(FILE1)

resposta:
	@diff -u $(FILE1) $(FILE2) && echo "No Differences found."|| echo "Differences found. Please check the output."