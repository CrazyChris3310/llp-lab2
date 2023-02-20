%{
#include <iostream>
#include <string>
#include <cstdlib>
#include "ast.h"
#include "parser.h"
%}

%option yylineno noyywrap nounput 

%%

"CREATE"          { return CREATE; }
"DROP"            { return DROP; }
"TABLE"           { return TABLE; }
"FOR"             { return FOR; }
"IN"              { return IN; }
"FILTER"          { return FILTER; }
"RETURN"          { return RETURN; }
"INSERT"          { return INSERT; }
"INTO"            { return INTO; }
"UPDATE"          { return UPDATE; }
"REMOVE"          { return REMOVE; }
"WITH"            { return WITH; }
"("               { return LPAREN; }
")"               { return RPAREN; }
"{"               { return LBRACE; }
"}"               { return RBRACE; }
":"               { return COLON; }
","               { return COMMA; }
">="              { yylval.compOp = ConstantOperation::GTE; return COMP_OP; }
"<="              { yylval.compOp = ConstantOperation::LTE; return COMP_OP; }
"=="              { yylval.compOp = ConstantOperation::EQ; return COMP_OP; }
"!="              { yylval.compOp = ConstantOperation::NEQ; return COMP_OP; }
">"               { yylval.compOp = ConstantOperation::GT; return COMP_OP; }
"<"               { yylval.compOp = ConstantOperation::LT; return COMP_OP; }
"LIKE"           { yylval.compOp = ConstantOperation::LIKE; return COMP_OP; }
"&&"              { yylval.logicOp = LogicalOp::AND;  return LOGIC_OP; }
"||"              { yylval.logicOp = LogicalOp::OR;  return LOGIC_OP; }
"true"            { yylval.boolVal = true; return BOOL_TOKEN; }
"false"           { yylval.boolVal = false; return BOOL_TOKEN; }
\"[^\"]*\"        { yylval.str = strdup(yytext); return STRING_TOKEN; }
[a-zA-Z][a-zA-Z0-9.]* { yylval.str = strdup(yytext); return ID; }
-?[0-9]+            { yylval.intVal = atoi(yytext); return INT_TOKEN; }
-?[0-9]+\.[0-9]+     { yylval.floatVal = atof(yytext); return FLOAT_TOKEN; }
[ \t\n]+          { /* ignore white spaces */ }
.                 { /* ignore everything else */ }

%%