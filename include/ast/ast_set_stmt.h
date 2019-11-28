#ifndef FONDLE_FISH_MYSQL_AST_SET_STMT_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_SET_STMT_H_INCLUDED

#include <cstdint>
#include <string>
#include <vector>
#include "ast_base.h"

class Ast_Expr;

class Ast_SetExpr : public Ast_Base {
public:
    explicit Ast_SetExpr(const char *uservar, Ast_Expr *expr);
    virtual ~Ast_SetExpr();

public:
    virtual std::string format();

private:
    std::string uservar;
    Ast_Expr *expr;
};

class Ast_SetList : public Ast_Base {
public:
    explicit Ast_SetList(Ast_SetExpr *expr);
    virtual ~Ast_SetList();

public:
    virtual std::string format();

public:
    void addSetExpr(Ast_SetExpr *expr);

private:
    std::vector<Ast_SetExpr *> exprs;
};

class Ast_SetStmt : public Ast_Base {
public:
    explicit Ast_SetStmt(Ast_SetList *set_list);
    virtual ~Ast_SetStmt();

public:
    virtual std::string format();

private:
    Ast_SetList *set_list;
};

#endif