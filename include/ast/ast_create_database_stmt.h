#ifndef FONDLE_FISH_MYSQL_AST_CREATE_DATABASE_STMT_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_CREATE_DATABASE_STMT_H_INCLUDED

#include <cstdint>
#include <string>
#include "ast_base.h"

class Ast_CreateDatabaseStmt : public Ast_Base {
public:
    explicit Ast_CreateDatabaseStmt(bool opt_if_not_exists, const char *name);
    virtual ~Ast_CreateDatabaseStmt();

public:
    virtual std::string format();

private:
    bool opt_if_not_exists;
    std::string name;
};

#endif