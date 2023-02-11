#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

class Node {
   public:
    virtual void print(int depth) = 0;
};

void printKeyVal(std::string key, std::string val, int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << key << ": " << val << std::endl;
}

class ForNode : public Node {
   private:
    std::string variable;
    std::string tableName;
    Node* action;

   public:
    ForNode(std::string variable, std::string tableName, Node* action) {
        this->variable = variable;
        this->tableName = tableName;
        this->action = action;
    }

    void print(int depth) override {
        printKeyVal("variable", this->variable, depth);
        printKeyVal("table", this->tableName, depth);
        if (this->action != nullptr) {
            this->action->print(depth + 1);
        }
    }
};

class ActionNode : public Node {
   private:
    std::list<Node*> actions;

   public:
    ActionNode() {}

    void addAction(Node* action) { this->actions.push_back(action); }

    void print(int depth) override {
        printKeyVal("actions: ", "", depth);
        for (auto arg : this->actions) {
            arg->print(depth + 1);
        }
    }

    ~ActionNode() {
        for (auto action : this->actions) {
            delete action;
        }
    }
};

enum DataType { INT, FLOAT, STRING, BOOL };

class Constant {
private:
    DataType type;
    union {
        int intVal;
        float floatVal;
        const char* strVal;
        bool boolVal;
    } value;

public:
    Constant(int val) {
        this->type = INT;
        this->value.intVal = val;
    }

    Constant(float val) {
        this->type = FLOAT;
        this->value.floatVal = val;
    }

    Constant(std::string val) {
        this->type = STRING;
        this->value.strVal = val.c_str();
    }

    Constant(bool val) {
        this->type = BOOL;
        this->value.boolVal = val;
    }

    std::string getStrVal() {
        switch (this->type) {
            case INT:
                return std::to_string(this->value.intVal);
            case FLOAT:
                return std::to_string(this->value.floatVal);
            case STRING:
                return this->value.strVal;
            case BOOL:
                return this->value.boolVal ? "true" : "false";
        }
    }

    std::string getStrType() {
        switch (this->type) {
            case INT:
                return "int";
            case FLOAT:
                return "float";
            case STRING:
                return "string";
            case BOOL:
                return "bool";
        }
    }
};

enum LogicalOp { AND, OR };

class Predicate : public Node {
    public:

};

enum ConstantOperations { EQ, NEQ, GT, LT, GTE, LTE, LIKE };

class Condition : public Predicate {
    private:
        Constant lval;
        Constant rval;
        std::string op;
    public:   
        Condition(Constant lval, Constant rval, std::string op): lval(lval), rval(rval) {
            this->op = op;
        }

        void print(int depth) override {
            printKeyVal("type", this->lval.getStrType(), depth);
            printKeyVal("value", this->lval.getStrVal(), depth);
        }
};

class ConditionUnion : public Predicate {
    private:
        LogicalOp op;
        Predicate* lval;
        Predicate* rval;

        std::string getStrOperator() {
            switch (this->op) {
                case AND:
                    return "and";
                case OR:
                    return "or";
            }
        };
    public:
        ConditionUnion(LogicalOp op, Predicate* lval, Predicate* rval) {
            this->op = op;
            this->lval = lval;
            this->rval = rval;
        }

        void print(int depth) override {
            printKeyVal("Operation", getStrOperator(), depth);
            printKeyVal("Left", "", depth);
            this->lval->print(depth + 1);
            printKeyVal("Right", "", depth);
            this->rval->print(depth + 1);
        }
};

class FilterNode : public Node {
   private:
      Predicate* predicate;
   public:

    void print(int depth) override {
        this->predicate->print(depth + 1);
    }
};

class TerminalAction : Node {

};

class ReturnAction : TerminalAction {
    private:
        std::string variable;
    public:
        ReturnAction(std::string variable) {
            this->variable = variable;
        }

        void print(int depth) override {
            printKeyVal("variable", this->variable, depth);
        }
};

class MapNode : public Node {
    private:
        std::unordered_map<std::string, std::string> actions;
    public:
        void addEntry(std::string key, std::string value) {
            this->actions[key] = value;
        }
    
        void print(int depth) override {
            printKeyVal("map node", "", depth);
            for (auto entry : this->actions) {
                printKeyVal("entry", "", depth + 1);
                printKeyVal("key", entry.first, depth + 2);
                printKeyVal("value", entry.second, depth + 2);
            }
        }
};

class UpdateAction : TerminalAction {
    private:
        std::string variable;
        MapNode value;
        std::string table;
    public:
        UpdateAction(std::string variable, MapNode value) {
            this->variable = variable;  
        }

        void print(int depth) override {
            printKeyVal("update action", "", depth);
            printKeyVal("variable", this->variable, depth + 1);
            printKeyVal("table", this->table, depth + 1);
            this->value.print(depth + 1);
        }
};

class RemoveAction : TerminalAction {
    private:
        std::string variable;
        std::string table;
    public:
        RemoveAction(std::string variable) {
            this->variable = variable;
        }

        void print(int depth) override {
            printKeyVal("remove action", "", depth);
            printKeyVal("variable", this->variable, depth + 1);
            printKeyVal("table", this->table, depth + 1);
        }
};

class InsertNode : Node {
    private:
        MapNode map;
        std::string table;
    public:
        InsertNode(MapNode map, std::string table) {
            this->map = map;
            this->table = table;
        }

        void print(int depth) override {
            printKeyVal("insert node", "", depth);
            printKeyVal("table", this->table, depth + 1);
            this->map.print(depth + 1);
        }
};