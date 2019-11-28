#include <map>
#include "ast_expr.h"
#include "ast_select_stmt.h"
#include "ast_table_reference.h"
#include "ast_opts.h"

Ast_SelectExpr::Ast_SelectExpr(Ast_Expr *expr, const char *alias) 
    : expr(expr), alias(alias ? alias : "")
{
}

Ast_SelectExpr::~Ast_SelectExpr() {
    if (expr) delete expr;
}

std::string Ast_SelectExpr::format() {
    if(!alias.empty())
        return this->rawf("%s AS %s", this->expr->format().c_str(), alias.c_str());
    else
        return this->expr->format();
}

Ast_SelectExprList::Ast_SelectExprList()
{
    exprs.push_back(nullptr);
}

Ast_SelectExprList::Ast_SelectExprList(Ast_SelectExpr *expr) {
    exprs.push_back(expr);
}

Ast_SelectExprList::~Ast_SelectExprList() {
    for (std::vector<Ast_SelectExpr *>::iterator it = exprs.begin();
        it != exprs.end();
        it ++)
    {
        if (*it) delete *it;
    }
}

void Ast_SelectExprList::addSelectExpr(Ast_SelectExpr *expr) {
    exprs.push_back(expr);
}

std::string Ast_SelectExprList::format() {
    std::string str;

    if (!exprs.empty()) {
        str = (exprs[0] ? exprs[0]->format() : "*");

        for (std::vector<Ast_SelectExpr *>::iterator it = exprs.begin() + 1;
            it != exprs.end();
            it ++)
        {
            if (*it)
                str += (", " + (*it)->format());
            else
                str += ", *";
        }
    }

    return str;
}
Ast_SelectStmt::TablelessSelectStmt::TablelessSelectStmt(
    enum Ast_SelectStmt::_select_opts select_opts, Ast_SelectExprList *select_expr_list)
    : select_opts(select_opts), select_expr_list(select_expr_list)
{
}

Ast_SelectStmt::TablelessSelectStmt::~TablelessSelectStmt() {
    if (select_expr_list)
        delete select_expr_list;
}

Ast_SelectStmt::TableSelectStmt::TableSelectStmt(enum Ast_SelectStmt::_select_opts select_opts, 
    Ast_SelectExprList *select_expr_list, Ast_TableReferences *table_references, Ast_OptWhere *opt_where,
    Ast_OptGroupBy *opt_groupby, Ast_OptHaving *opt_having, Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit,
    Ast_OptIntoList *opt_into_list)
    : select_opts(select_opts), select_expr_list(select_expr_list), table_references(table_references),
        opt_where(opt_where), opt_groupby(opt_groupby), opt_having(opt_having), opt_orderby(opt_orderby),
        opt_limit(opt_limit), opt_into_list(opt_into_list)
{
}

Ast_SelectStmt::TableSelectStmt::~TableSelectStmt() {
    if (select_expr_list) delete select_expr_list;
    if (table_references) delete table_references;
    if (opt_where) delete opt_where;
    if (opt_groupby) delete opt_groupby;
    if (opt_having) delete opt_having;
    if (opt_orderby) delete opt_orderby;
    if (opt_limit) delete opt_limit;
    if (opt_into_list) delete opt_into_list;
}


Ast_SelectStmt::Ast_SelectStmt(enum Ast_SelectStmt::_select_opts select_opts, Ast_SelectExprList *select_expr_list)
    : select_type(Ast_SelectStmt::SELECT_TYPE_TABLELESS),
        tableless(new Ast_SelectStmt::TablelessSelectStmt(select_opts, select_expr_list))
{
}

Ast_SelectStmt::Ast_SelectStmt(enum Ast_SelectStmt::_select_opts select_opts, 
    Ast_SelectExprList *select_expr_list, Ast_TableReferences *table_references, Ast_OptWhere *opt_where,
    Ast_OptGroupBy *opt_groupby, Ast_OptHaving *opt_having, Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit,
    Ast_OptIntoList *opt_into_list) 
    : select_type(Ast_SelectStmt::SELECT_TYPE_TABLE),
        table(new Ast_SelectStmt::TableSelectStmt(
            select_opts, select_expr_list, table_references, opt_where, opt_groupby, opt_having,
            opt_orderby, opt_limit, opt_into_list))
{
}

Ast_SelectStmt::~Ast_SelectStmt() {
    switch (this->select_type)
    {
    case Ast_SelectStmt::SELECT_TYPE_TABLELESS:
        delete this->tableless;
        break;
    case Ast_SelectStmt::SELECT_TYPE_TABLE:
        delete this->table;
        break;
    default:
        break;
    }
}

const char * Ast_SelectStmt::selectOptsName(Ast_SelectStmt::_select_opts select_opts) {
    static const std::map<int, std::string> names = {
        {Ast_SelectStmt::SELECT_OPTS_ALL, "ALL"},
        {Ast_SelectStmt::SELECT_OPTS_DISTINCT, "DISTINCT"},
        {Ast_SelectStmt::SELECT_OPTS_DISTINCTROW, "DISTINCT_ROW"},
        {Ast_SelectStmt::SELECT_OPTS_HIGH_PRIORITY, "HIGH_PRIORITY"},
        {Ast_SelectStmt::SELECT_OPTS_STRAIGHT_JOIN, "STRAIGHT_JOIN"},
        {Ast_SelectStmt::SELECT_OPTS_SQL_SMALL_RESULT, "SQL_SMALL_RESULT"},
        {Ast_SelectStmt::SELECT_OPTS_SQL_BIG_RESULT, "SQL_BIG_RESULT"},
        {Ast_SelectStmt::SELECT_OPTS_SQL_CALC_FOUND_ROWS, "SQL_CALC_FOUND_ROWS"}
    };
    
    static std::string s;
    s.clear();

    for (std::map<int, std::string>::const_iterator it = names.begin();
        it != names.end();
        ++ it)
    {
        if (select_opts & it->first) {
            if (!s.empty())
                s += ' ';
            s += (it->second);
        }
    }
    
    return s.c_str();
}

std::string Ast_SelectStmt::format() {
    switch (this->select_type)
    {
    case Ast_SelectStmt::SELECT_TYPE_TABLELESS:
        return this->rawf("SELECT %s %s", 
            this->selectOptsName(this->tableless->select_opts),
            this->tableless->select_expr_list->format().c_str()
        );
    case Ast_SelectStmt::SELECT_TYPE_TABLE:
        return this->rawf("SELECT %s %s FROM %s %s %s %s %s %s %s", 
            this->selectOptsName(this->table->select_opts),
            this->table->select_expr_list->format().c_str(),
            this->table->table_references->format().c_str(),
            this->table->opt_where ? this->table->opt_where->format().c_str() : "",
            this->table->opt_groupby ? this->table->opt_groupby->format().c_str() : "",
            this->table->opt_having ? this->table->opt_having->format().c_str() : "",
            this->table->opt_orderby ? this->table->opt_orderby->format().c_str() : "",
            this->table->opt_limit ? this->table->opt_limit->format().c_str() : "",
            this->table->opt_into_list ? this->table->opt_into_list->format().c_str() : ""
        );
    default:
        return "";
    }
}
