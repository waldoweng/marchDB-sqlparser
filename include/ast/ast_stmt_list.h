#ifndef FONDLE_FISH_MYSQL_AST_STMT_LIST_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_STMT_LIST_H_INCLUDED

#include <vector>
#include "ast_base.h"

class Ast_SelectStmt;
class Ast_DeleteStmt;
class Ast_InsertStmt;
class Ast_ReplaceStmt;
class Ast_UpdateStmt;
class Ast_CreateDatabaseStmt;
class Ast_CreateTableStmt;
class Ast_SetStmt;

class Ast_Stmt : public Ast_Base {
public:
    const static unsigned char SELECT_STMT = 0;
    const static unsigned char UPDATE_STMT = 1;
    const static unsigned char DELETE_STMT = 2;
    const static unsigned char INSERT_STMT = 3;
    const static unsigned char REPLACE_STMT = 4;
    const static unsigned char CREATE_DATABASE_STMT = 5;
    const static unsigned char CREATE_TABLE_STMT = 6;
    const static unsigned char SET_STMT = 7;

public:
    explicit Ast_Stmt(Ast_SelectStmt *stmt);
    explicit Ast_Stmt(Ast_UpdateStmt *stmt);
    explicit Ast_Stmt(Ast_DeleteStmt *stmt);
    explicit Ast_Stmt(Ast_InsertStmt *stmt);
    explicit Ast_Stmt(Ast_ReplaceStmt *stmt);
    explicit Ast_Stmt(Ast_CreateDatabaseStmt *stmt);
    explicit Ast_Stmt(Ast_CreateTableStmt *stmt);
    explicit Ast_Stmt(Ast_SetStmt *stmt);
    virtual ~Ast_Stmt();

public:
    virtual std::string format();

private:
    int m_stmt_type;
    union
    {
        Ast_SelectStmt *select_stmt;
        Ast_UpdateStmt *update_stmt;
        Ast_DeleteStmt *delete_stmt;
        Ast_InsertStmt *insert_stmt;
        Ast_ReplaceStmt *replace_stmt;
        Ast_CreateDatabaseStmt *create_database_stmt;
        Ast_CreateTableStmt *create_table_stmt;
        Ast_SetStmt *set_stmt;
    };
};

class Ast_StmtList : public Ast_Base {
public:
    explicit Ast_StmtList(Ast_Stmt *stmt);
    virtual ~Ast_StmtList();

public:
    virtual std::string format();

public:
    void addStmt(Ast_Stmt *stmt);

private:
    std::vector<Ast_Stmt *> stmt_list;
};


#endif
