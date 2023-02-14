#include <string>
#include <iostream>
#include "ast.h"

void printKeyVal(std::string key, std::string val, int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << key << ": " << val << std::endl;
}

// ------------------------------------------ ForNode ------------------------------------------

ForNode::ForNode(std::string variable, std::string tableName, Node* action) {
    this->variable = variable;
    this->tableName = tableName;
    this->action = action;
}

void ForNode::print(int depth) {
    printKeyVal("node", "for", depth);
    printKeyVal("variable", this->variable, depth);
    printKeyVal("table", this->tableName, depth);
    printKeyVal("actions", "", depth);
    if (this->action != nullptr) {
        this->action->print(depth + 1);
    }
}

ForNode::~ForNode() {
    delete this->action;
}

// ------------------------------------------ ActionNode ------------------------------------------

void ActionNode::addAction(Node* action) { 
    this->actions.push_back(action); 
}

void ActionNode::print(int depth) {
    for (auto arg : this->actions) {
        printKeyVal("action", "", depth);
        arg->print(depth + 1);
    }
}

ActionNode::~ActionNode() {
    for (auto action : this->actions) {
        delete action;
    }
}

// ------------------------------------------ Constant ------------------------------------------

Constant::Constant(int val) {
    this->type = INT;
    this->value.intVal = val;
}

Constant::Constant(float val) {
    this->type = FLOAT;
    this->value.floatVal = val;
}

Constant::Constant(const std::string& val, bool isRef = false) {
    if (isRef) {
        this->type = REF;
    } else {
        this->type = STRING;
    }
    char* temp = new char[val.length() + 1];
    val.copy(temp, val.length());
    temp[val.length()] = '\0';
    this->value.strVal = temp;
}

Constant::Constant(bool val) {
    this->type = BOOL;
    this->value.boolVal = val;
}

std::string Constant::getStrVal() {
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

std::string Constant::getStrType() {
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

void Constant::print(int depth) {
    printKeyVal("type", this->getStrType(), depth);
    printKeyVal("value", this->getStrVal(), depth);
}

Constant::~Constant() {
    if (this->type == STRING || this->type == REF) {
        delete[] this->value.strVal;
    }
}

// ------------------------------------------ Condition ------------------------------------------

Condition::Condition(Constant* lval, Constant* rval, ConstantOperation op): lval(lval), rval(rval) {
    this->op = op;
}

void Condition::print(int depth) {
    printKeyVal("Operation", operation_str[this->op], depth);
    printKeyVal("Left", "", depth);
    this->lval->print(depth + 1);
    printKeyVal("Right", "", depth);
    this->rval->print(depth + 1);
}

Condition::~Condition() {
    delete this->lval;
    delete this->rval;
}

// ------------------------------------------ ConditionUnion ------------------------------------------

std::string ConditionUnion::getStrOperator() {
    switch (this->op) {
        case AND:
            return "and";
        case OR:
            return "or";
        default:
            return "unknown";
    }
};

ConditionUnion::ConditionUnion(LogicalOp op, Predicate* lval, Predicate* rval) {
    this->op = op;
    this->lval = lval;
    this->rval = rval;
}

void ConditionUnion::print(int depth) {
    printKeyVal("Operation", getStrOperator(), depth);
    printKeyVal("Left", "", depth);
    this->lval->print(depth + 1);
    printKeyVal("Right", "", depth);
    this->rval->print(depth + 1);
}

ConditionUnion::~ConditionUnion() {
    delete this->lval;
    delete this->rval;
}

// ------------------------------------------ FilterNode ------------------------------------------

FilterNode::FilterNode(Predicate* predicate) {
    this->predicate = predicate;
}

void FilterNode::print(int depth) {
    printKeyVal("Name", "filter", depth);
    this->predicate->print(depth + 1);
}

FilterNode::~FilterNode() {
    delete this->predicate;
}

// ------------------------------------------ ReturnAction ------------------------------------------

ReturnAction::ReturnAction(Node* retVal) {
    this->retVal = retVal;
}

void ReturnAction::print(int depth) {
    printKeyVal("Name", "return", depth);
    this->retVal->print(depth);
}

ReturnAction::~ReturnAction() {
    delete this->retVal;
}

// ------------------------------------------ UpdateAction ------------------------------------------

UpdateAction::UpdateAction(std::string variable, MapNode* value, std::string table) {
    this->variable = variable;  
    this->value = value;
    this->table = table;
}

void UpdateAction::print(int depth) {
    printKeyVal("name", "update", depth);
    printKeyVal("variable", this->variable, depth + 1);
    printKeyVal("table", this->table, depth + 1);
    this->value->print(depth + 1);
}

UpdateAction::~UpdateAction() {
    delete this->value;
}

// ------------------------------------------ RemoveAction ------------------------------------------

RemoveAction::RemoveAction(std::string variable, std::string table) {
    this->variable = variable;
    this->table = table;
}

void RemoveAction::print(int depth) {
    printKeyVal("name", "update", depth);
    printKeyVal("variable", this->variable, depth + 1);
    printKeyVal("table", this->table, depth + 1);
}

// ------------------------------------------ MapEntry ------------------------------------------

MapEntry::MapEntry(std::string key, Constant* value) {
    this->key = key;
    this->value = value;
}

void MapEntry::print(int depth) {
    printKeyVal("key", this->key, depth);
    printKeyVal("value", "", depth);
    this->value->print(depth + 1);
}

MapEntry::~MapEntry() {
    delete this->value;
}

// ------------------------------------------ MapNode ------------------------------------------

void MapNode::addEntry(MapEntry* entry) {
    this->entries.push_back(entry);
}

void MapNode::print(int depth) {
    printKeyVal("map node", "", depth);
    for (auto entry : this->entries) {
        printKeyVal("entry", "", depth + 1);
        entry->print(depth + 2);
    }
}

MapNode::~MapNode() {
    for (auto entry : this->entries) {
        delete entry;
    }
}

// ------------------------------------------ InsertNode ------------------------------------------

InsertNode::InsertNode(MapNode* map, std::string table) {
    this->map = map;
    this->table = table;
}

void InsertNode::print(int depth) {
    printKeyVal("insert node", "", depth);
    printKeyVal("table", this->table, depth + 1);
    this->map->print(depth + 1);
}

InsertNode::~InsertNode() {
    delete this->map;
}