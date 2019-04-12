Letter	[A-Za-z]
Digit   [0-9]

%{
  #include "IOMngr.h"
  #include "YSemantics.h"
  #include "y.tab.h"

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
    // also computes and sets span of token
    #define RETURN(val,offset) \
      { if (shouldLog) fprintf(stderr,"Token#=%d, Length=%zu, Text=\"%s\"\n",val,(unsigned long)yyleng,yytext); \
        tokenStartPosition = GetLastPosition() - yyleng + offset; \
        yylloc.first_column = tokenStartPosition; \
        yylloc.last_column = yylloc.first_column + yyleng - 1; \
        return(val); \
      }

      bool lineComment = false;
      int nestedCommentCount = 0;
    #define IN_COMMENT (lineComment || nestedCommentCount > 0)

%}

%%
\/\/  					  { lineComment = true;}
\/\*						  { nestedCommentCount++;}
\*\/						  { nestedCommentCount--;}
\,                                 { if (!IN_COMMENT) RETURN(',',1); }
int                             { if (!IN_COMMENT) RETURN(INT_TOK,1); }
chr                             { if (!IN_COMMENT) RETURN(CHR_TOK,1); }
bool                            { if (!IN_COMMENT) RETURN(BOOL_TOK,1); }
void                            { if (!IN_COMMENT) RETURN(VOID_TOK,1); }
true                            { if (!IN_COMMENT) RETURN(BOOLLIT_TOK,1); }
false                           { if (!IN_COMMENT) RETURN(BOOLLIT_TOK,1); }
-?{Digit}{Digit}*                 { if (!IN_COMMENT) RETURN(INTLIT_TOK,0);
{Letter}                        { if (!IN_COMMENT) RETURN(CHRLIT_TOK,0);
{Letter}({Letter}|{Digit})*     { if (!IN_COMMENT) RETURN(IDENT_TOK,0); }
\:\:                            { if (!IN_COMMENT) RETURN(DBLCOLON_TOK,1); }
\:                              { if (!IN_COMMENT) RETURN(':',1); }
[ \t]                           { }
[\n]                            { lineComment = false; }
.                               { }
%%
