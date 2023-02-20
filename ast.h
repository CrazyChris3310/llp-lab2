#ifndef AST_H
#define AST_H

#include <iostream>
#include <list>

enum NodeType { FOR_NODE, ACTION_NODE, FILTER_NODE, RETURN_NODE, UPDATE_NODE, REMOVE_NODE, INSERT_NODE,
                MAP_NODE, MAP_ENTRY_NODE, CONDITION_NODE, CONDITION_UNION_NODE, CONSTANT_NODE,
                CREATE_TABLE_NODE, DROP_TABLE_NODE };

class Node {
    protected:
        NodeType nodeType;
    public:
        virtual void print(int depth) = 0;
        virtual ~Node() {};
        NodeType getNodeType() {
            return this->nodeType;
        }
};


struct NodeWrapper {
    Node* node;
};

void printKeyVal(const char* key, const char* val, int depth);

class ForNode : public Node {
   private:
    const char* variable;
    const char* tableName;
    Node* action;

   public:
    ForNode(const char* variable, const char* tableName, Node* action);
    void print(int depth) override;
    ~ForNode();
};

class ActionNode : public Node {
   private:
    std::list<Node*> actions;

   public:
    ActionNode() { this->nodeType = ACTION_NODE; }
    void addAction(Node* action);
    void print(int depth) override;
    ~ActionNode();
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
    Constant(int val);
    Constant(float val);
    Constant(const char* val, bool isRef);
    Constant(bool val);
    std::string getStrVal();
    std::string getStrType();
    void print(int depth) override ;
    ~Constant();
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

        const char* operation_str[7] = { "==", "!=", ">", "<", ">=", "<=", "like" };
    public:   
        Condition(Constant* lval, Constant* rval, ConstantOperation op);
        void print(int depth) override;
        ~Condition();
};

class ConditionUnion : public Predicate {
    private:
        LogicalOp op;
        Predicate* lval;
        Predicate* rval;

        const char* getStrOperator();
    public:
        ConditionUnion(LogicalOp op, Predicate* lval, Predicate* rval);
        void print(int depth) override;
        ~ConditionUnion();
};

class FilterNode : public Node {
   private:
      Predicate* predicate;
   public:

    FilterNode(Predicate* predicate);
    void print(int depth) override;
    ~FilterNode();
};

class TerminalAction : public Node {

};

class ReturnAction : public TerminalAction {
    private:
        Node* retVal;
    public:
        ReturnAction(Node* retVal);
        void print(int depth) override;
        ~ReturnAction();
};

class MapEntry : public Node {
    private:
        const char* key;
        Constant* value;
    public:
        MapEntry(const char* key, Constant* value);
        void print(int depth) override;
        ~MapEntry();
};

class MapNode : public Node {
    private:
        std::list<MapEntry*> entries;
    public:
        MapNode() { this->nodeType = MAP_NODE; }
        void addEntry(MapEntry* entry);
        void print(int depth) override;
        ~MapNode();
};

class UpdateAction : public TerminalAction {
    private:
        const char* variable;
        MapNode* value;
        const char* table;
    public:
        UpdateAction(const char* variable, MapNode* value, const char* table);
        void print(int depth) override;
        ~UpdateAction();
};

class RemoveAction : public TerminalAction {
    private:
        const char* variable;
        const char* table;
    public:
        RemoveAction(const char* variable, const char* table);
        void print(int depth) override;
        ~RemoveAction();
};

class InsertNode : public Node {
    private:
        MapNode* map;
        const char* table;
    public:
        InsertNode(MapNode* map, const char* table);
        void print(int depth) override;
        ~InsertNode();
};

class CreateTableNode : public Node {
    private:
        const char* table;
        MapNode* fields;
    public:
        CreateTableNode(const char* table, MapNode* fields);
        void print(int depth) override;
        ~CreateTableNode();
};

class DropTableNode : public Node {
    private:
        const char* table;
    public:
        DropTableNode(const char* table);
        void print(int depth) override;
        ~DropTableNode();
};

#endif