
CC = gcc
STD = c11

CFLAGS = -g -std=$(STD) -pthread

TARGET = measure_concurrent
OBJECTS = concurrent_counter.o \
		measure_time.o\
		sloppy_counter.o\
		linked_list.o\
		hand_over_list.o\
		

all: $(TARGET)

$(TARGET): $(TARGET).o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm
	
linked_list.o: linked_list.c linked_list.h
hand_over_list.o: hand_over_list.c hand_over_list.h		
concurrent_counter.o: concurrent_counter.c concurrent_counter.h
sloppy_counter.o: sloppy_counter.c sloppy_counter.h
measure_time.o: measure_time.c measure_time.h
measure_concurrent.o: measure_concurrent.c measure_time.h concurrent_counter.h sloppy_counter.h linked_list.h hand_over_list.h

clean:
	rm -f $(TARGET) $(TARGET).o $(OBJECTS)
