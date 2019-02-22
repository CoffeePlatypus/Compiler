# CS 442 - Compiler Construction
## Scanner Assignment Respository

This repository contains files to be added to your main assignment repository to include the Scanner portion of the project. The repository contains

- ScannerDriver.c
 - This is the test driver for this portion of the project. The driver opens the source file and repeatedly requests the next token from the scanner. Depending on the token type different actions are taken. The "@INIT" token causes a symbol table to be created. Identifiers are stored in this symbol table. A running sum of integer literal tokens and float literal tokens is maintained. The "@DUMP" token displays the integer and float sums and the symbol table contents is sorted order.  
- Scanner.l
 - This is the starting point for the scanner definition. It contains the outline of the needed scanner rules. 
- Scanner.h
 - This exposes as an include file the functions and data provided by the lex/flex generated scanner. 
- ScanTokens.h
 - This is an include file containing defines for the various token types. 
- Makefile
 - The Makefile containing new rules and definitions for the scanner component. 
- ScannerSource
 - The input source.
- Scanner.out.ref
 - The reference output 

## The Assignment

 - Scanner.l contains rules for the @INIT, @DUMP and Identifier tokens. Extend these rules to incorporate nestable comments (both "/* ... */" comments, which may be nested, and "through to the end of line" comments "// ...."). Include the code necessary to only return tokens when not inside a comment. Also include rules for integers, floats and at least one symbol. You will need to add defines for these token types to ScanTokens.h. Integer and float literals should allow an immediately preceding "-" to negate values. This can either be done with separate regular expressions for the positive and negative cases or using the lex/flex question mark operator (e.g. "a?" represents an optional "a"). 
 - ScannerDriver.c contains most of the required code. The cases for @INIT and @DUMP are supplied. @INIT creates a symbol table and @DUMP displays the contents of the table. You need to supply code for the IDENT_TOK case to add the token text to the symbol table with an attributes structure keeping track of the token's span in the source and it's occurrence count. This case should set messages indicating the action taken (i.e. no action because no symboltable, found with an occurrence count and added as new identifier). You will also need to add cases for integer and float literals. These cases should update the accumulated sum of integer and float literals. You may use the ``atoi()`` and ``atof()`` functions to convert the token text to numeric values. Both functions are able to handle negative values.The @DUMP case displays the running sum of integer and float literals and the symbol table contents. 
 
## What To Do

- DO NOT FORK this repository on Gitlab, instead
- On your development machine, in a location different than your assignment repository, clone this project. Your assignment repository will continue to accumulate all files for the project. 
- Copy the files from this repository to your assignment repository.
- Discard the clone of this repository.
- Implement the required capabilities. 
- When complete, "make clean" to remove object files and executables, use git to add and commit your changes and push the project back up to your repository.
- Finaly, create an issue on your repository, with me as the reporter assignee, indicating that it is ready for grading. 


