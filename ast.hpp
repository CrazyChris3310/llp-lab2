#pragma once

#include <iostream>
#include <list>
#include <string>

class Node {
   public:
    virtual void print(int depth) = 0;
    virtual ~Node() {}
};

void printKeyVal(std::string key, std::string val, int depth);

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
        printKeyVal("node", "for", depth);
        printKeyVal("variable", this->variable, depth);
        printKeyVal("table", this->tableName, depth);
        printKeyVal("actions", "", depth);
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
        for (auto arg : this->actions) {
            printKeyVal("action", "", depth);
            arg->print(depth + 1);
        }
    }

    ~ActionNode() {
        for (auto action : this->actions) {
            delete action;
        }
    }
};

enum DataType { INT, FLOAT, STRING, BOOL, REF };

class Constant :public Node {
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

    Constant(const std::string& val, bool isRef = false) {
        if (isRef) {
            this->type = REF;
        } else {
            this->type = STRING;
        }
        char* temp = new char[val.length() + 1];
        val.copy(temp, val.length());
        temp[val.length()] = '\0';
        this->value.strVal = temp;
        // strcpy((char*)this->value.strVal, val.c_str());
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
            case REF:
                return this->value.strVal;
            default:
                return "[none]";
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
            case REF:
                return "reference";
            default:
                return "unknown";
        }
    }

    void print(int depth) override {
        printKeyVal("type", this->getStrType(), depth);
        printKeyVal("value", this->getStrVal(), depth);
    }

    ~Constant() {
        if (this->type == STRING || this->type == REF) {
            delete[] this->value.strVal;
        }
    }
};

enum LogicalOp { AND, OR };

class Predicate : public Node {
    public:
        virtual ~Predicate() {}
};

enum ConstantOperation { EQ, NEQ, GT, LT, GTE, LTE, LIKE };

class Condition : public Predicate {
    private:
        Constant* lval;
        Constant* rval;
        ConstantOperation op;

        std::string operation_str[7] = { "==", "!=", ">", "<", ">=", "<=", "like" };
    public:   
        Condition(Constant* lval, Constant* rval, ConstantOperation op): lval(lval), rval(rval) {
            this->op = op;
        }

        void print(int depth) override {
            printKeyVal("Operation", operation_str[this->op], depth);
            printKeyVal("Left", "", depth);
            this->lval->print(depth + 1);
            printKeyVal("Right", "", depth);
            this->rval->print(depth + 1);
        }

        ~Condition() {
            delete this->lval;
            delete this->rval;
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
                default:
                    return "unknown";
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

        ~ConditionUnion() {
            delete this->lval;
            delete this->rval;
        }
};

class FilterNode : public Node {
   private:
      Predicate* predicate;
   public:

    FilterNode(Predicate* predicate) {
        this->predicate = predicate;
    }

    void print(int depth) override {
        printKeyVal("Name", "filter", depth);
        this->predicate->print(depth + 1);
    }

    ~FilterNode() {
        delete this->predicate;
    }
};

class TerminalAction : public Node {

};

class ReturnAction : public TerminalAction {
    private:
        Node* retVal;
    public:
        ReturnAction(Node* retVal) {
            this->retVal = retVal;
        }

        void print(int depth) override {
            printKeyVal("Name", "return", depth);
            this->retVal->print(depth);
        }
};

class MapEntry : public Node {
    private:
        std::string key;
        Constant* value;
    public:
        MapEntry(std::string key, Constant* value) {
            this->key = key;
            this->value = value;
        }

        void print(int depth) override {
            printKeyVal("key", this->key, depth);
            printKeyVal("value", "", depth);
            this->value->print(depth + 1);
        }
};

class MapNode : public Node {
    private:
        std::list<MapEntry*> entries;
    public:
        void addEntry(MapEntry* entry) {
            this->entries.push_back(entry);
        }
    
        void print(int depth) override {
            printKeyVal("map node", "", depth);
            for (auto entry : this->entries) {
                printKeyVal("entry", "", depth + 1);
                entry->print(depth + 2);
            }
        }

        ~MapNode() {
            for (auto entry : this->entries) {
                delete entry;
            }
        }
};

class UpdateAction : public TerminalAction {
    private:
        std::string variable;
        MapNode* value;
        std::string table;
    public:
        UpdateAction(std::string variable, MapNode* value, std::string table) {
            this->variable = variable;  
            this->value = value;
            this->table = table;
        }

        void print(int depth) override {
            printKeyVal("name", "update", depth);
            printKeyVal("variable", this->variable, depth + 1);
            printKeyVal("table", this->table, depth + 1);
            this->value->print(depth + 1);
        }

        ~UpdateAction() {
            delete this->value;
        }
};

class RemoveAction : public TerminalAction {
    private:
        std::string variable;
        std::string table;
    public:
        RemoveAction(std::string variable, std::string table) {
            this->variable = variable;
            this->table = table;
        }

        void print(int depth) override {
            printKeyVal("name", "update", depth);
            printKeyVal("variable", this->variable, depth + 1);
            printKeyVal("table", this->table, depth + 1);
        }
};

class InsertNode : public Node {
    private:
        MapNode* map;
        std::string table;
    public:
        InsertNode(MapNode* map, std::string table) {
            this->map = map;
            this->table = table;
        }

        void print(int depth) override {
            printKeyVal("insert node", "", depth);
            printKeyVal("table", this->table, depth + 1);
            this->map->print(depth + 1);
        }

        ~InsertNode() {
            delete this->map;
        }
};