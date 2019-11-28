#ifndef FONDLE_FISH_MYSQL_AST_TABLE_REFERENCE_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_TABLE_REFERENCE_H_INCLUDED

#include <string>
#include <vector>
#include "ast_base.h"

class Ast_Expr;
class Ast_TableSubquery;
class Ast_TableReference;
class Ast_TableReferences;
class Ast_SelectStmt;
class Ast_ColumnList;

class Ast_IndexList : public Ast_Base {
public:
    explicit Ast_IndexList(const char *name);
    virtual ~Ast_IndexList();

public:
    virtual std::string format();

public:
    void addName(const char *name);

private:
    std::vector<std::string> names;
};

class Ast_IndexHint : public Ast_Base {
public:
    explicit Ast_IndexHint(bool use, bool for_join, Ast_IndexList *index_list);
    virtual ~Ast_IndexHint();

public:
    virtual std::string format();

private:
    bool use;
    bool for_join;
    Ast_IndexList *index_list;
};

class Ast_JoinCondition : public Ast_Base {
public:

public:
    explicit Ast_JoinCondition(Ast_Expr *expr);
    explicit Ast_JoinCondition(Ast_ColumnList *column_list);
    virtual ~Ast_JoinCondition();

public:
    virtual std::string format();

private:
    Ast_Expr *expr;
    Ast_ColumnList *column_list;
};


class Ast_TableSubquery : public Ast_Base {
public:
    explicit Ast_TableSubquery(Ast_SelectStmt *select_stmt);
    virtual ~Ast_TableSubquery();

public:
    virtual std::string format();

private:
    Ast_SelectStmt *select_stmt;
};


class Ast_TableFactor : public Ast_Base {
public:
    enum factor_type {
        FACTOR_TYPE_NORMAL      = 0,
        FACTOR_TYPE_SUBQUERY    = 1,
        FACTOR_TYPE_REFERENCES  = 2
    };
public:
    explicit Ast_TableFactor(const char *name, const char *alias, Ast_IndexHint *index_hint);
    explicit Ast_TableFactor(const char *database_name, const char *name, const char *alias,
        Ast_IndexHint *index_hint);
    explicit Ast_TableFactor(Ast_TableSubquery *subquery, const char *name);
    explicit Ast_TableFactor(Ast_TableReferences *references);
    virtual ~Ast_TableFactor();

public:
    class TableFactorNormal {
    public:
        explicit TableFactorNormal(const char *database_name, const char *name, 
            const char *alias, Ast_IndexHint *index_hint);
        ~TableFactorNormal();
    public:
        std::string database_name;
        std::string name;
        std::string alias;
        Ast_IndexHint *index_hint;
    };

    class TableFactorSubquery {
    public:
        explicit TableFactorSubquery(Ast_TableSubquery *subquery, const char *name);
        ~TableFactorSubquery();
    public:
        Ast_TableSubquery *subquery;
        std::string name;
    };

    class TableFactorReferences {
    public:
        explicit TableFactorReferences(Ast_TableReferences *references);
        ~TableFactorReferences();
    public:
        Ast_TableReferences *references;
    };

public:
    virtual std::string format();

private:
    enum factor_type factor_type;
    union {
        TableFactorNormal *normal;
        TableFactorSubquery *subquery;
        TableFactorReferences *references;
    };
};

class Ast_JoinTable : public Ast_Base {
public:
    enum join_type {
        JOIN_TABLE_INNER            = 0,
        JOIN_TABLE_CROSS            = 1,
        JOIN_TABLE_LEFT_OUTER       = 2,
        JOIN_TABLE_RIGHT_OUTER      = 3,
        JOIN_TABLE_STRAIGHT_JOIN    = 4
    };

public:
    explicit Ast_JoinTable(Ast_TableReference *reference, join_type join_type, 
        Ast_TableFactor *table_factor, Ast_JoinCondition *join_condition);
    explicit Ast_JoinTable(Ast_TableReference *reference, Ast_TableFactor *table_factor);
    explicit Ast_JoinTable(Ast_TableReference *reference, join_type join_type, 
        Ast_TableFactor *table_factor, Ast_Expr *expr);
    explicit Ast_JoinTable(Ast_TableReference *reference, join_type join_type, 
        Ast_TableFactor *table_factor);
    virtual ~Ast_JoinTable();

public:
    virtual std::string format();

private:
    const char * joinTypeName(enum Ast_JoinTable::join_type join_type);

private:
    Ast_TableReference *reference;
    join_type join_type;
    Ast_TableFactor *table_factor;
    Ast_JoinCondition *join_condition;
};

class Ast_TableReference : public Ast_Base {
public:
    explicit Ast_TableReference(Ast_TableFactor *table_factor);
    explicit Ast_TableReference(Ast_JoinTable *join_table);
    virtual ~Ast_TableReference();

public:
    virtual std::string format();

private:
    Ast_TableFactor *table_factor;
    Ast_JoinTable *join_table;
};

class Ast_TableReferences : public Ast_Base{
public:
    explicit Ast_TableReferences(Ast_TableReference *reference);
    virtual ~Ast_TableReferences();

public:
    virtual std::string format();

public:
    void addTableReference(Ast_TableReference *reference);

private:
    std::vector<Ast_TableReference *> references;
};

#endif
