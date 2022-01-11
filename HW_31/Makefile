FLAGS = -Wall -pthread

all: fork-join.o\
	 rendezvous.o\
	 barrier.o\
	 reader-writer-nostarve.o \
	 reader-writer.o\
	 mutex-nostarve.o\

%.o: %.c common_threads.h
	$(CC) $(FLAGS) -o $@ $<

clean:
	rm -f ./*.o
