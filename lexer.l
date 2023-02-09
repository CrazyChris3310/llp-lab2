%{
#include <iostream>
#include <string>
#include "ast.hpp"
#include "parser.h"
%}

%option yylineno
%option noyywrap

%%

"FOR"             { return FOR; }
"IN"              { return IN; }
"FILTER"          { return FILTER; }
"RETURN"          { return RETURN; }
"INSERT"          { return INSERT; }
"INTO"            { return INTO; }
"("               { return LPAREN; }
")"               { return RPAREN; }
"{"               { return LBRACE; }
"}"               { return RBRACE; }
":"               { return COLON; }
>=|<=|[<>=]|!=    { return COMP_OP; }
&&|\|\|           { return LOGIC_OP; }
[a-zA-Z][a-zA-Z0-9]* { yylval.str = yytext; return ID; }
[0-9]+            { yylval.str = yytext; return VALUE; }
[ \t\n]+          { /* ignore white spaces */ }
.                 { /* ignore everything else */ }

%%