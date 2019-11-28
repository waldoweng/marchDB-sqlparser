#ifndef FONDLE_FISH_MYSQL_AST_UPDATE_STMT_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_UPDATE_STMT_H_INCLUDED

#include <cstdint>
#include <string>
#include <vector>
#include "ast_base.h"

class Ast_Expr;
class Ast_TableReferences;
class Ast_OptWhere;
class Ast_OptOrderBy;
class Ast_OptLimit;

class Ast_UpdateAsgnList : public Ast_Base {
public:
    struct UpdateAsgn {
    public:
        explicit UpdateAsgn(const char *tablename, const char *name, Ast_Expr *expr);
        ~UpdateAsgn();
    public:
        std::string tablename;
        std::string name;
        Ast_Expr *expr;
    };
public:
    explicit Ast_UpdateAsgnList(const char *name, Ast_Expr *expr);
    explicit Ast_UpdateAsgnList(const char *tablename, const char *name, Ast_Expr *expr);
    virtual ~Ast_UpdateAsgnList();

public:
    virtual std::string format();

public:
    void addUpdateAsgn(const char *name, Ast_Expr *expr);
    void addUpdateAsgn(const char *tablename, const char *name, Ast_Expr *expr);

private:
    std::vector<UpdateAsgn *> update_asgn_list;
};

class Ast_UpdateStmt : public Ast_Base {
public:
    enum update_opts {
        UPDATE_OPTS_LOW_PRIORITY    = 1,
        UPDATE_OPTS_IGNORE          = 2
    };
public:
    explicit Ast_UpdateStmt(update_opts update_opts, Ast_TableReferences *table_reference,
        Ast_UpdateAsgnList *update_asgn_list, Ast_OptWhere *opt_where,
        Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit);
    virtual ~Ast_UpdateStmt();

public:
    virtual std::string format();

private:
    const char * updateOptsName(update_opts update_opts);

private:
    update_opts update_opts;
    Ast_TableReferences *references;
    Ast_UpdateAsgnList *update_asgn_list;
    Ast_OptWhere *opt_where;
    Ast_OptOrderBy *opt_orderby;
    Ast_OptLimit *opt_limit;
};

#endif