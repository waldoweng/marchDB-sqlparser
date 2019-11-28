#ifndef FONDLE_FISH_MYSQL_AST_DELETE_STMT_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_DELETE_STMT_H_INCLUDED

#include <cstdint>
#include <string>
#include <vector>
#include "ast_base.h"

class Ast_OptWhere;
class Ast_OptOrderBy;
class Ast_OptLimit;
class Ast_TableReferences;

class Ast_DeleteList : public Ast_Base {
public:
    explicit Ast_DeleteList(const char *name);
    virtual ~Ast_DeleteList();

public:
    virtual std::string format();

public:
    void addName(const char *name);

private:
    std::vector<std::string> names;
};


class Ast_DeleteStmt : public Ast_Base {
public:
    enum delete_type {
        SINGLE_TABLE = 1,
        MULTIPLE_TABLE = 2
    };
    enum _delete_opts {
        DELETE_OPTS_LOW_PRIORITY = 1,
        DELETE_OPTS_QUICK = 2,
        DELETE_OPTS_IGNORE = 3
    };
public:
    explicit Ast_DeleteStmt(enum _delete_opts delete_opts, const char *name, Ast_OptWhere *opt_where,
        Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit);
    explicit Ast_DeleteStmt(enum _delete_opts delete_opts, Ast_DeleteList *delete_list, 
        Ast_TableReferences *table_references, Ast_OptWhere *opt_where);
    virtual ~Ast_DeleteStmt();

public:
    class SingleTableDeleteStmt {
    public:
        explicit SingleTableDeleteStmt(enum _delete_opts delete_opts, const char *name, Ast_OptWhere *opt_where,
            Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit);
        ~SingleTableDeleteStmt();
    public:
        enum _delete_opts delete_opts;
        std::string name;
        Ast_OptWhere *opt_where;
        Ast_OptOrderBy *opt_orderby;
        Ast_OptLimit *opt_limit;
    };

    class MultipleTableDeleteStmt {
    public:
        explicit MultipleTableDeleteStmt(enum _delete_opts delete_opts, Ast_DeleteList *delete_list, 
            Ast_TableReferences *table_references, Ast_OptWhere *opt_where);
        ~MultipleTableDeleteStmt();
    public:
        enum _delete_opts delete_opts;
        Ast_DeleteList *delete_list;
        Ast_TableReferences *table_references;
        Ast_OptWhere *opt_where;
    };

public:
    virtual std::string format();

private:
    const char *deleteOptName(enum _delete_opts delete_opts);

private:
    delete_type delete_type;
    union {
        SingleTableDeleteStmt *single_stmt;
        MultipleTableDeleteStmt *multi_stmt;
    };
};

#endif
