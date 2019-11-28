#ifndef FONDLE_FISH_MYSQL_AST_INSERT_VALLIST_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_INSERT_VALLIST_H_INCLUDED

#include <vector>
#include "ast_base.h"

class Ast_Expr;

class Ast_InsertVals : public Ast_Base {
public:
    explicit Ast_InsertVals(Ast_Expr *expr);
    virtual ~Ast_InsertVals();

public:
    virtual std::string format();

public:
    void addInsertVal(Ast_Expr *expr);

private:
    std::vector<Ast_Expr *> insert_vals;
};

class Ast_InsertValList : public Ast_Base {
public:
    explicit Ast_InsertValList(Ast_InsertVals *insert_vals);
    virtual ~Ast_InsertValList();

public:
    virtual std::string format();

public:
    void addInsertVals(Ast_InsertVals *insert_vals);

private:
    std::vector<Ast_InsertVals *> insert_vals_list;
};

#endif