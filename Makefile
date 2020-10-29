
# gcc compiler
CC = gcc

# Compile flags
CFLAGS = -ggdb3 -Wall -pg


# Build Target
TARGET = main
RT = RTree

all: $(TARGET)

$(TARGET): $(TARGET).o $(RT).o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $< $(LIBS)

$(RT).o: $(RT).c
	$(CC) $(CFLAGS) -c $< $(LIBS)

clean:
	rm *.o
	rm $(TARGET)
