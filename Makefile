SHELL=/bin/bash

#==========================
# include lex and yacc libraries
# enable debugging and use gnu11 standard
#==========================
LOADLIBES = -ll -ly
YFLAGS = -d --verbose
CFLAGS = -g  -std=gnu11

# several projects use y.tab.h produced by yacc, to
# avoid using wrong y.tab.h they must each build their own,
# this is used to force the application of a rule
FORCE:

#==========================
# Make rule syntax
# target: dependencies
# 	rule to build target from dependencies
#
# - no rule necessary if make already knows (i.e. .o from .c)
# - sometimes no dependencies (e.g. clean target)

#==========================
# Symbol Table Project
SymTab.o: SymTab.c SymTab.h
SymTabDriver.o: SymTabDriver.c SymTab.h
SymTabDriver: SymTabDriver.o SymTab.o

symtest: SymTabDriver
		make -s symtest-0
		make -s symtest-1
		make -s symtest-2

symtest-0: SymTabDriver
		@./SymTabDriver | tee SymData-0.out
		@echo -n "Testing Sanity " >&2 ; \
		cmp -s SymData-0.out SymData-0.out.ref; echo "out: $$?" | sed 's/0/YES/g' | sed 's/1/NO/g' >&2

symtest-1: SymTabDriver
		@./SymTabDriver SymData-1.in | tee SymData-1.out
		@echo -n "Testing SymData-1 " >&2 ; \
		cmp -s SymData-1.out SymData-1.out.ref; echo "out: $$?" | sed 's/0/YES/g' | sed 's/1/NO/g' >&2

symtest-2: SymTabDriver
		@./SymTabDriver SymData-2.in | tee SymData-2.out
		@echo -n "Testing SymData-2 " >&2 ; \
		cmp -s SymData-2.out SymData-2.out.ref; echo "out: $$?" | sed 's/0/YES/g' | sed 's/1/NO/g' >&2

# Other
clean:
	rm -f *.o SymTabDriver 
