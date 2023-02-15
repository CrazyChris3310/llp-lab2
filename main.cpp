#include <iostream>
#include <string>
#include "ast.h"
#include "parser.h"
#include "lexer.h"

int parseInput(std::string& query, NodeWrapper& nodeWrapper) {
    yy_scan_string(query.c_str());
    int code = yyparse(nodeWrapper);
    yylex_destroy();
    return code;
}

int main() {

    std::string buf;
    std::string line;
    std::cout << "> ";
    while(getline(std::cin, line)) {
        buf.append(line);
        buf.append("\n");
        if (line.find(';') != std::string::npos) {
            NodeWrapper nodeWrapper;
            int code = parseInput(buf, nodeWrapper);
            if (code) {
                std::cout << "ret_code: " << code << std::endl;
            } else {
                nodeWrapper.node->print(0);
                delete nodeWrapper.node;
            }
            buf.clear();
            std::cout << "> ";
        }
    }

    return 0;
}