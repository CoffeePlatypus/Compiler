Letter	[A-Za-z]
Digit   [0-9]

%{
#include "RDTokens.h"
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


%}

%%
int                { RETURN(INT_TOK,1); }
chr                { RETURN(CHR_TOK,1); }
.					         { }
%%
