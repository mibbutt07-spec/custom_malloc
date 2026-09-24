CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = test
OBJS = custom_malloc.o test.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

malloc.o: malloc.c
	$(CC) $(CFLAGS) -c custom_malloc.c

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

clean:
	rm -f $(OBJS) $(TARGET)