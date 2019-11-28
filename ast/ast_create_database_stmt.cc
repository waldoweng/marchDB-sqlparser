#include <cstdio>
#include "ast_create_database_stmt.h"

Ast_CreateDatabaseStmt::Ast_CreateDatabaseStmt(bool opt_if_not_exists, const char *name) 
    : opt_if_not_exists(opt_if_not_exists), name(name) {}

Ast_CreateDatabaseStmt::~Ast_CreateDatabaseStmt() {}

std::string Ast_CreateDatabaseStmt::format() {
    if (opt_if_not_exists)
        return this->indentf("CREATE IF NOT EXIST DATABASE %s", name.c_str());
    else
        return this->indentf("CREATE DATABASE %s", name.c_str());
}