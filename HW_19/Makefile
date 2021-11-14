#
#
# MAKEFILE 
#
# Daniel Rill, Sebastian Kaeser
# 12.11.2021
# Betriebssysteme Gruppe 11
#



CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror -std=c11 -o0
CPPCHECK = cppcheck --enable=warning

TARGET = tlb

.PHONY: all clean cppcheck

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $@ $^
	rm -f $(TARGET).o 


cppcheck: $(TARGET).c
	$(CPPCHECK) $^

clean: 
	rm -f $(TARGET) $(TARGET).o

