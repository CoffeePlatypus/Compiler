/* Scanner.h
   This is really a convenience wrapper around the function and variables
   supplied by lex.
*/

#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>

extern int yylex();                   // The next token function.
extern char *yytext;                  // The matched token text.

#ifdef __APPLE__
extern size_t yyleng;                 // The token text length.
#endif

#ifdef __linux__
extern int yyleng;                    // The token text length.
#endif

extern unsigned int tokenStartPosition; // column of tokens first char

#endif
