#ifndef FONDLE_FISH_MYSQL_AST_OPTS_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_OPTS_H_INCLUDED

#include <vector>
#include "ast_base.h"

class Ast_Expr;
class Ast_ColumnList;
class Ast_InsertAsgnList;

class Ast_GroupByList : public Ast_Base {
public:
    explicit Ast_GroupByList(Ast_Expr *expr, bool asc);
    virtual ~Ast_GroupByList();

public:
    class GroupByItem {
    public:
        GroupByItem(Ast_Expr *expr, bool asc);
        ~GroupByItem();
    public:
        Ast_Expr *expr;
        bool asc;
    };

public:
    void addGroupBy(Ast_Expr *expr, bool asc);

public:
    virtual std::string format();

private:
    std::vector<GroupByItem> groupby_list;
};


class Ast_OptOnDupUpdate : public Ast_Base {
public:
    explicit Ast_OptOnDupUpdate(Ast_InsertAsgnList *insert_asgn_list);
    virtual ~Ast_OptOnDupUpdate();

public:
    virtual std::string format();

private:
    Ast_InsertAsgnList *insert_asgn_list;
};


class Ast_OptLimit : public Ast_Base{
public:
    explicit Ast_OptLimit(Ast_Expr *limit);
    explicit Ast_OptLimit(Ast_Expr *offset, Ast_Expr *limit);
    virtual ~Ast_OptLimit();

public:
    virtual std::string format();

private:
    Ast_Expr *offset;
    Ast_Expr *limit;
};


class Ast_OptIntoList : public Ast_Base {
public:
    explicit Ast_OptIntoList(Ast_ColumnList *column_list);
    virtual ~Ast_OptIntoList();

public:
    virtual std::string format();

private:
    Ast_ColumnList *column_list;
};


class Ast_OptHaving : public Ast_Base {
public:
    explicit Ast_OptHaving(Ast_Expr *expr);
    virtual ~Ast_OptHaving();

public:
    virtual std::string format();

private:
    Ast_Expr *expr;
};


class Ast_OptGroupBy : public Ast_Base {
public:
    explicit Ast_OptGroupBy(Ast_GroupByList *groupby_list, bool opt_with_rollup);
    virtual ~Ast_OptGroupBy();

public:
    virtual std::string format();

private:
    Ast_GroupByList *groupby_list;
    bool opt_with_rollup;
};


class Ast_OptColNames : public Ast_Base {
public:
    explicit Ast_OptColNames(Ast_ColumnList *column_list);
    virtual ~Ast_OptColNames();

public:
    virtual std::string format();

private:
    Ast_ColumnList *column_list;
};


class Ast_OptOrderBy : public Ast_Base {
public:
    explicit Ast_OptOrderBy(Ast_GroupByList *orderby_list);
    ~Ast_OptOrderBy();
    
public:
    virtual std::string format();

private:
    Ast_GroupByList *orderby_list;
};


class Ast_OptWhere : public Ast_Base {
public:
    explicit Ast_OptWhere(Ast_Expr *expr);
    virtual ~Ast_OptWhere();

public:
    virtual std::string format();

private:
    Ast_Expr *expr;
};

#endif
