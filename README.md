# CS 442 - Compiler Construction
## RecDescent

This repository contains files to be added to your main project repository to include the Recursive Descent capabilities. The repository contains

- RecDescent.c
 - This is the test driver for the assignment. This driver requires a source filename as a command line option.  

- RDGrammar.h and RDGrammar.c
 - These contain the implementations of the recursive descent functions. The ".h" file lists all the recursive functions even though only the initial nonterminal function needs to be exposed. 

- RDSupport.h and RDSupport.c
 - These contain the implementations of support functions in particular matching an expected token.

- RDScanner.l
 - This specifies the token types used in this project. It will reuse the Scanner.h file from the previous assignment to expose the functions and data created by Flex in compiling this specification. 

- RDTokens.h
 - This provides symbolic constants for the required token types. 

## The Assignment

- Complete RDScanner.l to specify regular expressions and actions for the required token types. The regular expressions will be similar to previous assignments. 

- Complete RDGrammar.c to provide the implementation of the required recursive descent functions. If a function corresponds to a single production rule then it will simply be a sequence of token tests. If a function corresponds to multiple productions then you will need a switch statement, based on the current token, which selects the correct right-hand-side sequence to follow. In C, cases in a switch fall through unless there is a break. Consequently, if more than one token corresponds to the same right-hand-size then simply list the cases with no breaks in between and implement the right-hand-side in the last case.

## What To Do

- Do NOT fork this repository on Gitlab, instead
- On your development machine, in a location different than your assignment repository, clone this project. Your assignment repository will continue to accumulate all files for the project. 
- Copies the files from this repository to your assignment repository.
- Discard the clone of this repository.
- Implement the required capabilities. 
- When complete, "make clean" to remove object files and executables, use git to add and commit your changes and push the project back up to your repository.
- Finaly, create an issue on your repository, with me as the reporter assignee, indicating that it is ready for grading. 

