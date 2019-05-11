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

symtabref:
		@for file in SymData-0 SymData-1 SymData-2 ; do \
		  cp $$file.out $$file.out.ref ;\
		done

#===========================
# IOMngr Project
IOMngr.o: IOMngr.c IOMngr.h
IOMngrDriver.o: IOMngrDriver.c IOMngr.h
IOMngrDriver: IOMngrDriver.o IOMngr.o
IOMngrTest.o: IOMngrTest.c IOMngr.h
IOMngrTest: IOMngrTest.o IOMngr.o

iotest:	IOMngrDriver IOMngrTest
		make -s iotest-0
		make -s iotest-1
		make -s iotest-2

iotest-0: IOMngrTest
		@./IOMngrTest | tee IOMngrTest.out
		@echo -n "Testing IOMngr-0 " >&2 ; \
		cmp -s IOMngrTest.out IOMngrTest.out.ref; echo "out: $$?" | sed 's/0/YES/g' | sed 's/1/NO/g' >&2

iotest-1: IOMngrDriver
		@./IOMngrDriver IOMngrSource | tee IOMngr.out
		@echo -n "Testing IOMngr-1 " >&2 ; \
		cmp -s IOMngr.out IOMngr.out.ref; echo "out: $$?" | sed 's/0/YES/g' | sed 's/1/NO/g' >&2

iotest-2: IOMngrDriver
		@./IOMngrDriver -u IOMngrSource | tee IOMngr-UNKN.out
		@echo -n "Testing IOMngr-3 " >&2 ; \
		cmp -s IOMngr-UNKN.out IOMngr-UNKN.out.ref; echo "out: $$?" | sed 's/0/YES/g' | sed 's/1/NO/g' >&2

#===========================
# Scanner Project
ScannerDriver.o: ScannerDriver.c Scanner.h SymTab.h ScanTokens.h
ScannerDriver: 	ScannerDriver.o Scanner.o SymTab.o IOMngr.o

scantest: 	ScannerDriver
		@./ScannerDriver | tee Scanner.out
		@echo -n "Testing Scanner " >&2 ; \
		cmp -s Scanner.out Scanner.out.ref; echo "out: $$?" | sed 's/0/YES/g' | sed 's/1/NO/g' >&2

#===========================
# Recursive Descent Project
RDSupport.o:	RDSupport.c RDSupport.h
RecDescent.o: 	RecDescent.c Scanner.h RDTokens.h
RecDescent: 	RecDescent.o RDGrammar.o RDScanner.o RDSupport.o IOMngr.o

rdtest: 	rdtest1 rdtest2 rdtest3 rdtest4 rdtest5

rdtest1: 	RecDescent
	./RecDescent RDSrc-1.src
rdtest2: 	RecDescent
	./RecDescent RDSrc-2.src
rdtest3: 	RecDescent
	./RecDescent RDSrc-3.src
rdtest4: 	RecDescent
	./RecDescent RDSrc-4.src
rdtest5: 	RecDescent
	./RecDescent RDSrc-5.src


#===========================
# Parser Stage 1 & 2
ParserScanner.o: ParserScanner.l IOMngr.h ParserGrammar.o y.tab.h
ParserGrammar.o: FORCE ParserGrammar.y
Parse.o: Parse.c Grammar.h Scanner.h IOMngr.h
Parse: Parse.o ParserGrammar.o ParserScanner.o IOMngr.o

partest:	parse1 parse2

parse1:	Parse
	./Parse ParSrc-1.src
parse2:	Parse
	./Parse ParSrc-2.src

#===========================
# Semantics
CGTest.o:     CGTest.c YCodeGen.h
CGTest:       CGTest.o YCodeGen.o

YCodeGen.o:	YCodeGen.c YCodeGen.h

YScanner.o: 	YScanner.l IOMngr.h YSemantics.h y.tab.h
YGrammar.o y.tab.h: YGrammar.y
YStructs.o:   YStructs.h YStructs.c
YSemantics.o: YSemantics.c YSemantics.h YStructs.h
Y.o: 		      Y.c Grammar.h YScanner.l IOMngr.h
Y:		        Y.o SymTab.o IOMngr.o YGrammar.o YScanner.o YStructs.o YSemantics.o YCodeGen.o

ytest: Y
		@for file in y1 y2 y3 y4 y5 y6 y7 ; do \
		  make -s $$file.test; \
		done

%.test: %.src
		@./Y $* | tee $*.lst
		@spim -noexception -file $*.asm < $*.in | tee $*.out
		@cmp -s $*.lst $*.lst.ref; echo -n "lst: $$? " >> $*.test
		@cmp -s $*.asm $*.asm.ref; echo -n "asm: $$? " >> $*.test
		@cmp -s $*.out $*.out.ref; echo "out: $$?" >> $*.test
		@echo "=========="
		@echo -n "Testing $< " >&2 ; cat $*.test | sed 's/0/YES/g' | sed 's/1/NO/g' >&2
		@echo "===================="
		@rm $*.test

sem2test:    feature1 feature2 feature3 feature4

feature1: Y
		echo "Increment and decrement"
		rm -f f1.asm
		./Y f1
		cat f1.asm
		spim -noexception -file f1.asm

feature2: Y
		echo "For loop"
		rm -f f2.asm
		./Y f2
		cat f2.asm
		spim -noexception -file f2.asm

feature3: Y
		echo "ternary test"
		rm -f f3.asm
		./Y f3
		cat f3.asm
		spim -noexception -file f3.asm < f3.in

feature4: Y
		echo "LOOP"
		rm -f f4.asm
		./Y f4
		cat f4.asm
		spim -noexception -file f4.asm

# Other
clean:
	rm -f *.o SymTabDriver IOMngrTest IOMngrDriver ScannerDriver RecDescent Parse ParserScanner.c YGrammar.c Y-1 Y-2 Y-3 Y-4 y.tab.h
