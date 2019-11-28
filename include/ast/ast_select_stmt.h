#ifndef FONDLE_FISH_MYSQL_AST_SELECT_STMT_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_SELECT_STMT_H_INCLUDED

#include <cstdint>
#include <string>
#include <vector>
#include "ast_base.h"

class Ast_Expr;

class Ast_SelectExpr : public Ast_Base {
public:
    explicit Ast_SelectExpr(Ast_Expr *expr, const char *alias);
    virtual ~Ast_SelectExpr();

public:
    virtual std::string format();

private:
    Ast_Expr *expr;
    std::string alias;
};

class Ast_SelectExprList : public Ast_Base{
public:
    const static int SELECTALL = 0;

public:
    explicit Ast_SelectExprList();
    explicit Ast_SelectExprList(Ast_SelectExpr *expr);
    virtual ~Ast_SelectExprList();

public:
    virtual std::string format();

public:
    void addSelectExpr(Ast_SelectExpr *expr);

private:
    std::vector<Ast_SelectExpr *> exprs;
};
class Ast_SelectExprList;
class Ast_TableReferences;
class Ast_OptWhere;
class Ast_OptGroupBy;
class Ast_OptHaving;
class Ast_OptOrderBy;
class Ast_OptLimit;
class Ast_OptIntoList;

class Ast_SelectStmt : public Ast_Base {
public:
    enum _select_opts {
        SELECT_OPTS_ALL                 = 1 << 0,
        SELECT_OPTS_DISTINCT            = 1 << 1,
        SELECT_OPTS_DISTINCTROW         = 1 << 2,
        SELECT_OPTS_HIGH_PRIORITY       = 1 << 3,
        SELECT_OPTS_STRAIGHT_JOIN       = 1 << 4,
        SELECT_OPTS_SQL_SMALL_RESULT    = 1 << 5,
        SELECT_OPTS_SQL_BIG_RESULT      = 1 << 6,
        SELECT_OPTS_SQL_CALC_FOUND_ROWS = 1 << 7
    };
    enum select_type {
        SELECT_TYPE_TABLELESS,
        SELECT_TYPE_TABLE
    };

public:
    class TablelessSelectStmt {
    public:
        explicit TablelessSelectStmt(enum _select_opts select_opts, Ast_SelectExprList *select_expr_list);
        ~TablelessSelectStmt();
    public:
        enum _select_opts select_opts;
        Ast_SelectExprList *select_expr_list;
    };

    class TableSelectStmt {
    public:
        explicit TableSelectStmt(enum _select_opts select_opts, Ast_SelectExprList *select_expr_list,
            Ast_TableReferences *table_references, Ast_OptWhere *opt_where, Ast_OptGroupBy *opt_groupby,
            Ast_OptHaving *opt_having, Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit,
            Ast_OptIntoList *opt_into_list);
        ~TableSelectStmt();
    public:
        enum _select_opts select_opts;
        Ast_SelectExprList *select_expr_list;
        Ast_TableReferences *table_references;
        Ast_OptWhere *opt_where;
        Ast_OptGroupBy *opt_groupby;
        Ast_OptHaving *opt_having;
        Ast_OptOrderBy *opt_orderby;
        Ast_OptLimit *opt_limit;
        Ast_OptIntoList *opt_into_list;
    };

public:
    explicit Ast_SelectStmt(enum _select_opts select_opts, Ast_SelectExprList *select_expr_list);
    explicit Ast_SelectStmt(enum _select_opts select_opts, Ast_SelectExprList *select_expr_list,
        Ast_TableReferences *table_references, Ast_OptWhere *opt_where, Ast_OptGroupBy *opt_groupby,
        Ast_OptHaving *opt_having, Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit,
        Ast_OptIntoList *opt_into_list);
    virtual ~Ast_SelectStmt();

public:
    virtual std::string format();

private:
    const char * selectOptsName(enum _select_opts select_opts);

private:
    enum select_type select_type;
    union {
        TablelessSelectStmt *tableless;
        TableSelectStmt *table;
    };
};

#endif
