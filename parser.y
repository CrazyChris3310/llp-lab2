%{
#include <iostream>
#include <string>
#include <list>
#include "ast.hpp"

extern int yylex();

void yyerror(const char *s){
  extern int yylineno;
  std::cerr << yylineno << ": error: " << s << std::endl;
}

%}

%define parse.error verbose

%union {
  const char* str;
  Node* node;
}

%token FOR
%token IN
%token FILTER
%token RETURN
%token INSERT
%token INTO
%token ID
%token VALUE
%token LPAREN
%token RPAREN
%token COLON
%token LBRACE
%token RBRACE
%token LOGIC_OP
%token COMP_OP

%type<str> ID VALUE LOGIC_OP COMP_OP
%type<node> id value for_stmt select_stmt action conditions condition return_val return_stmt map map_items map_item insert_stmt

%%

query: select_stmt  { $1->print(0); delete $1; }
      | insert_stmt { $1->print(0); delete $1; }
select_stmt: for_stmt action { $$ = new Node("SELECT", "");
    std::cout << "select" << std::endl;
                                $$->appendChild($1);
                                $$->appendChild($2); }
for_stmt: FOR id IN id {
                          $$ = new Node("FOR", "");
                          $$->appendChild($2);
                          $$->appendChild($4);
                        }

action: FILTER conditions return_stmt {
                                        $$ = new Node("ACTION", "");
                                        Node* filter = new Node("FILTER", "");
                                        filter->appendChild($2);
                                        $$->appendChild(filter);
                                        $$->appendChild($3);
                                    }


conditions: condition                      { $$ = $1; }
          | condition LOGIC_OP conditions {
                                          $$ = new Node($2, "");
                                          $$->appendChild($1);
                                          $$->appendChild($3);
                                        }
condition: id COMP_OP value {
                                        $$ = new Node($2, "");
                                        $$->appendChild($1);
                                        $$->appendChild($3);
                                        }


return_stmt: RETURN return_val { $$ = new Node("RETURN", "");
                            $$->appendChild($2);}
return_val: id
          | value
          | map { $$ = $1; }

map: LBRACE map_items RBRACE { $$ = new Node("MAP", "");
                            $$->appendChild($2); }
    | LBRACE RBRACE { $$ = new Node("MAP", ""); }

map_items: map_item          { $$ = $1; }
          | map_item map_items { $2->appendChild($1); $$ = $2; }

map_item: id COLON value {
                            $$ = new Node("map_item", "");
                            $$->appendChild($1);
                            $$->appendChild($3);
}

id: ID { $$ = new Node("ID", $1); }
value: VALUE { $$ = new Node("VALUE", $1); }

insert_stmt: INSERT map INTO id { $$ = new Node("INSERT", "");
                                $$->appendChild($2);
                                $$->appendChild($4); }

%%

int main() {
  yyparse();
  return 0;
}