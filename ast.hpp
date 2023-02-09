#include <string>
#include <list>
#include <iostream>

class Node {
  public:
    std::string type;
    std::string value;
    std::list<Node*> children;

    Node(std::string type, std::string value) {
      this->type = type;
      this->value = value;
    }

    Node() {
      this->type = "";
      this->value = "";
    }

    void appendChild(Node* child) {
      this->children.push_back(child);
    }

    void printKeyVal(std::string key, std::string val, int depth) {
        for (int i = 0; i < depth; i++) {
            std::cout << "  ";
        }
        std::cout << key << ": " << val << std::endl;
    }

    void print(int depth) {
      printKeyVal("node type", this->type, depth);
      printKeyVal("node value", this->value, depth);
      std::string children = this->children.size() > 0 ? "" : "[]";
      printKeyVal("children", children, depth);
      for (Node* child : this->children) {
        child->print(depth + 1);
      }
    }

    ~Node() {
      for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
            delete (*it);
    }
};