#include <string>
#include <iostream>

void printKeyVal(std::string key, std::string val, int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << key << ": " << val << std::endl;
}