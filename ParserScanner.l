Letter	[A-Za-z]
Digit   [0-9]

%{
#include "y.tab.h"
#include "IOMngr.h"
#define YY_INPUT(buf,result,max_size) \
  { int c = GetSourceChar(); \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
  }

  // Set to true to log each token returned to stderr
  bool shouldLog = false;

  // The column of the returned token's first char
  unsigned int tokenStartPosition;

  // Return token number
  // offset = 1 for tokens that include the last char read
  // offset = 0 for tokens that do not include the last char read
  #define RETURN(tokenNumber,offset) \
    { if (shouldLog) fprintf(stderr,"Token#=%d, Length=%zu, Text=\"%s\"\n", \
                            tokenNumber,(unsigned long)yyleng,yytext); \
      tokenStartPosition = GetLastPosition() - yyleng + offset; \
      return(tokenNumber); \
    }

    int nestedCommentCount = 0;
    bool lineComment = false;

    #define IN_COMMENT (lineComment || nestedCommentCount > 0)

%}

%%

  /* Combine comment nesting from Scanner.l with RDScanner.l regular
     expressions except for real literal. Use RETURN(token-symbolic-name,offset)
     where token-symbolic-name appears in yacc file or is a single char.
   */

\/\/  					  { lineComment = true;}
\/\*						  { nestedCommentCount++;}
\*\/						  { nestedCommentCount--;}
int                             { if (!IN_COMMENT) RETURN(INT_TOK,1); }
chr               			  { if (!IN_COMMENT) RETURN(CHR_TOK,1); }
\:\=						  { if (!IN_COMMENT) RETURN(ASSIGN_TOK,1); }
\;                              { if (!IN_COMMENT) RETURN(';',1); }
\(						  { if (!IN_COMMENT) RETURN('(',1); }
\)						  { if (!IN_COMMENT) RETURN(')',1); }
\-						  { if (!IN_COMMENT) RETURN('-',1); }
\+						  { if (!IN_COMMENT) RETURN('+',1); }
\*						  { if (!IN_COMMENT) RETURN('*',1); }
\/						  { if (!IN_COMMENT) RETURN('/',1); }
\{						  { if (!IN_COMMENT) RETURN('{',1); }
\}						  { if (!IN_COMMENT) RETURN('}',1); }
\,						  { if (!IN_COMMENT) RETURN(',',1); }
\-?{Digit}+				  { if (!IN_COMMENT) RETURN(INTLIT_TOK,0); }
{Letter}({Letter}|{Digit})*	  { if (!IN_COMMENT) RETURN(IDENT_TOK,0); }
[\n]                            { lineComment = false; }
[ \t]                           { }
.                               { }
%%
