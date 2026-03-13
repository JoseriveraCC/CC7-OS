CC = gcc
CFLAGS = -Wall -Wextra -pthread
OBJ = main_linux.o log_processor.o

all: log_analyzer

log_analyzer: $(OBJ)
	$(CC) $(CFLAGS) -o log_analyzer $(OBJ)

main_linux.o: main_linux.c log_processor.h
	$(CC) $(CFLAGS) -c main_linux.c

log_processor.o: log_processor.c log_processor.h
	$(CC) $(CFLAGS) -c log_processor.c

clean:
	rm -f *.o log_analyzer
