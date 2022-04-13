%{
#include "parse.h"
#include <string.h>
#define saveText(text) yylval.sval = strdup(text)
#define saveInt(text)  yylval.ival = atoi(text) 
#define saveDouble(text)    yylval.dval = atof(text)
%}

WS          [\x20\x09\x0A\x0D]
MINUS       \x2D
PLUS        \x2B

SIGN        {MINUS}|{PLUS}

DIGIT19     [\x31-\x39]
DIGIT       [\x30-\x39]
ZERO        \x30

POINT       \x2E

E           \x65|\x45

INT         {ZERO}|{DIGIT19}{DIGIT}*   
EXP         {E}{SIGN}{0,1}{DIGIT}+
FRAC        {POINT}{DIGIT}+

INTEGER     {MINUS}{0,1}{INT}
NUMBER      {MINUS}{0,1}{INT}{FRAC}{0,1}{EXP}{0,1}

QUOTATION   \x22
ESCAPE      \x5C
UNESCAPED   [\x20-\x21\x23-\x5B\x5D-\xFF]

CHAR        {UNESCAPED}|{ESCAPE}(\x22|\x5C|\x2F|\x62|\x66|\x6E|\x72|\x74|\x75)

STRING      {QUOTATION}{CHAR}*{QUOTATION}

%%

{WS}*       {}

"{"         { return '{'; }
"}"         { return '}'; }
"["         { return '['; }
"]"         { return ']'; }
","         { return ','; }
\x3A        { return ':'; }

"true"      { return TRUE; }
"false"     { return FALSE; }
{INTEGER}   { saveInt(yytext); return INTEGER; }
{NUMBER}    { saveDouble(yytext); return NUMBER; }
{STRING}    { saveText(yytext); return STRING; }   

.           { printf("%s\n", yytext); }

%%

int yywrap(void)
{
    return 1;
}