#ifndef FONDLE_FISH_MYSQL_AST_OPT_COLUMN_LIST_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_OPT_COLUMN_LIST_H_INCLUDED

#include <string>
#include <vector>
#include "ast_base.h"

class Ast_ColumnList : public Ast_Base{
public:
    explicit Ast_ColumnList(const char *name);
    virtual ~Ast_ColumnList();

public:
    virtual std::string format();

public:
    void addName(const char *name);

private:
    std::vector<std::string> names;
};

#endif