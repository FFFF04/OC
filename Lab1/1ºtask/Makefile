CC = gcc
CFLAGS=-Wall -Wextra
TARGET=SimpleProgram
FILE1 = SimpleProgram.txt
FILE2 = /home/francisco/OC/Lab1/tests/results_L1.txt

all:
	$(CC) $(CFLAGS) SimpleProgram.c L1Cache.c -o $(TARGET)

clean:
	rm $(TARGET) $(FILE1)

run:
	./$(TARGET) > $(FILE1)

resposta:
	cmp -s $(FILE1) $(FILE2); \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
	        echo "SAME"; \
	else \
	        echo "NOT SAME"; \
	fi