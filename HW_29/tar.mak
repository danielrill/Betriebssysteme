#
#
# tar.mak 
#
#

LDBIN = Makefile\
        tar.mak\
		concurrent_counter.h concurrent_counter.c \
		sloppy_counter.h sloppy_counter.c \
		linked_list.h linked_list.c \
		hand_over_list.h hand_over_list.c \
		measure_time.h measure_time.c \
		measure_concurrent.c \

TAR = tar cvzf
FORMAT = tar.gz
LIB = HW_29

.PHONY: all clean

all: $(LIB).$(FORMAT)

$(LIB).$(FORMAT): $(LDBIN)
	$(TAR) $@ $^

clean:
	rm -f $(LIB).$(FORMAT)
