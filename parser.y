%{
#include <iostream>
#include <list>
#include "ast.h"

extern int yylex();

void yyerror(NodeWrapper& node, const char *s){
  extern int yylineno;
  std::cerr << yylineno << ": error: " << s << std::endl;
}


int temp = 0;

%}

%define parse.error verbose

%parse-param { NodeWrapper& root }

%union {
  const char* str;
  float floatVal;
  int intVal;
  bool boolVal;

  LogicalOp logicOp;
  ConstantOperation compOp;

  Node* node;
  Predicate* predicate;
  TerminalAction* terminal;
  ActionNode* action;
  Constant* constant;
}

%token<str> ID
%token<str> STRING_TOKEN
%token<boolVal> BOOL_TOKEN
%token<intVal> INT_TOKEN
%token<floatVal> FLOAT_TOKEN
%token FOR
%token IN
%token FILTER
%token RETURN
%token INSERT
%token INTO
%token LPAREN
%token RPAREN
%token COLON
%token LBRACE
%token RBRACE
%token<logicOp> LOGIC_OP
%token<compOp> COMP_OP
%token COMMA
%token UPDATE
%token WITH
%token REMOVE

%type<node> for_stmt action return_val map map_items map_item insert_stmt filter_stmt
%type<terminal> terminal_stmt return_stmt update_stmt remove_stmt
%type<predicate> conditions condition
%type<action> actions
%type<constant> constant id value

%%

query: for_stmt  { root.node = $1;  }
      | insert_stmt { root.node = $1; }

for_stmt: FOR ID IN ID actions { $$ = new ForNode($2, $4, $5); }

actions: actions action { $$ = $1; $1->addAction($2); } 
        | action { $$ = new ActionNode(); $$->addAction($1); }

action: for_stmt { $$ = $1; } 
      | filter_stmt { $$ = $1; }
      | terminal_stmt  { $$ = $1; }

terminal_stmt: return_stmt { $$ = $1; }
              | update_stmt { $$ = $1; }
              | remove_stmt { $$ = $1; }

filter_stmt: FILTER conditions { $$ = new FilterNode($2); }


conditions: condition                      { $$ = $1; }
          | conditions LOGIC_OP conditions { $$ = new ConditionUnion($2, $1, $3); }

condition: constant COMP_OP constant {
                                        $$ = new Condition($1, $3, $2);
                                        }

constant: id | value  { $$ = $1; }


return_stmt: RETURN return_val { $$ = new ReturnAction($2); }

return_val: constant  { $$ = $1; }
          | map { $$ = $1; }

update_stmt: UPDATE ID WITH map IN ID { $$ = new UpdateAction($2, (MapNode*)$4, $6); }

remove_stmt: REMOVE ID IN ID { $$ = new RemoveAction($2, $4); }

map: LBRACE map_items RBRACE { $$ = $2; }
    | LBRACE RBRACE { $$ = new MapNode(); }

map_items: map_item          { MapNode* node = new MapNode(); node->addEntry((MapEntry*)$1); $$ = node; }
          | map_item COMMA map_items { ((MapNode*)$3)->addEntry((MapEntry*)$1); $$ = $3; }

map_item: STRING_TOKEN COLON constant { $$ = new MapEntry($1, $3); }

id: ID { $$ = new Constant($1, true); }

value: INT_TOKEN { $$ = new Constant($1); }
      | FLOAT_TOKEN { $$ = new Constant($1); }
      | STRING_TOKEN { $$ = new Constant($1); }
      | BOOL_TOKEN { $$ = new Constant($1); }

insert_stmt: INSERT map INTO ID { $$ = new InsertNode((MapNode*)$2, $4); }

%%
