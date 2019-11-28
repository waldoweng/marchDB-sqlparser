#ifndef FONDLE_FISH_MYSQL_AST_INSERT_ASSIGN_LIST_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_INSERT_ASSIGN_LIST_H_INCLUDED

#include <vector>
#include <string>
#include "ast_base.h"

class Ast_Expr;

class Ast_InsertAsgnList : public Ast_Base {
public:
    explicit Ast_InsertAsgnList(const char *name, Ast_Expr *expr);
    virtual ~Ast_InsertAsgnList();

public:
    class InsertAsgn {
    public:
        InsertAsgn(const char *name, Ast_Expr *expr);
        ~InsertAsgn();
    public:
        std::string name;
        Ast_Expr *expr;
    };

public:
    virtual std::string format();

public:
    void addInsertAsgn(const char *name, Ast_Expr *expr);

private:
    std::vector<InsertAsgn *> asgn_list;
};

#endif