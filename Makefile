
# gcc compiler
CC = gcc

# Compile flags
CFLAGS = -ggdb3 -Wall

# Libs
LIBS = huffman.c

# Build Target
TARGET = main

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIBS)

clean:
	$(RM) $(TARGET)
