#
#
# MAKEFILE 
#
# Daniel Rill, Sebastian Kaeser
# 12.11.2021
# Betriebssysteme Gruppe 11
# ausführen mit make -f tar.mak


RM = rm -f

LDBIN =  Makefile \
		shell-script.sh \
		tlb.c \
		tar.mak
	
TAR = tar cvzf 
FORMAT = tar.gz
LIB = HW_19

.PHONY: all clean 

all: $(LIB).$(FORMAT)

$(LIB).$(FORMAT):  $(LDBIN)
		$(TAR)  $@ $^


clean:
		$(RM) $(LIB).$(FORMAT)
		

