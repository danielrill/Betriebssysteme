
CC = gcc
STD = c11

CFLAGS = -g -std=$(STD) -pthread

TARGET = measure_concurrent
OBJECTS = concurrent_counter.o \
		measure_time.o\

all: $(TARGET)

$(TARGET): $(TARGET).o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

concurrent_counter.o: concurrent_counter.c concurrent_counter.h
measure_time.o: measure_time.c measure_time.h
measure_concurrent.o: measure_concurrent.c measure_time.h concurrent_counter.h

clean:
	rm -f $(TARGET) $(TARGET).o $(OBJECTS)
