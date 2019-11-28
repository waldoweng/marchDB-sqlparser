#include "ast_opts.h"
#include "ast_expr.h"
#include "ast_column_list.h"
#include "ast_insert_asgn_list.h"

Ast_GroupByList::GroupByItem::GroupByItem(Ast_Expr *expr, bool asc) 
    : expr(expr), asc(asc)
{
}

Ast_GroupByList::GroupByItem::~GroupByItem() {
    if (expr) delete expr;
}

Ast_GroupByList::Ast_GroupByList(Ast_Expr *expr, bool asc) {
    groupby_list.push_back(Ast_GroupByList::GroupByItem(expr, asc));
}

Ast_GroupByList::~Ast_GroupByList() {}

void Ast_GroupByList::addGroupBy(Ast_Expr *expr, bool asc) {
    groupby_list.push_back(Ast_GroupByList::GroupByItem(expr, asc));
}

std::string Ast_GroupByList::format() {
    std::string str;

    for (std::vector<Ast_GroupByList::GroupByItem>::iterator it = groupby_list.begin();
        it != groupby_list.end();
        ++it
    ) {
        str += this->rawf("GROUP BY %s %s",
            it->expr->format().c_str(),
            it->asc ? "ASC" : "DESC"
        );
    }

    return str;
}


Ast_OptOnDupUpdate::Ast_OptOnDupUpdate(Ast_InsertAsgnList *insert_asgn_list) 
    : insert_asgn_list(insert_asgn_list)
{
}

Ast_OptOnDupUpdate::~Ast_OptOnDupUpdate() {
    if (insert_asgn_list) delete insert_asgn_list;
}

std::string Ast_OptOnDupUpdate::format() {
    if (this->insert_asgn_list) {
        return this->rawf("ON DUPLICATE KEY UPDATE %s", this->insert_asgn_list->format().c_str());
    }
    return "";
}


Ast_OptLimit::Ast_OptLimit(Ast_Expr *limit) 
    : offset(nullptr), limit(limit)
{
}

Ast_OptLimit::Ast_OptLimit(Ast_Expr *offset, Ast_Expr *limit) 
    : offset(offset), limit(limit)
{
}

Ast_OptLimit::~Ast_OptLimit() 
{
    if (offset) delete offset;
    if (limit) delete limit;
}


std::string Ast_OptLimit::format() {
    if (offset) {
        return this->rawf("LIMIT %s, %s", 
            this->offset->format().c_str(),
            this->limit->format().c_str()
        );
    } else if (limit) {
        return this->rawf("LIMIT %s", 
            this->limit->format().c_str()
        );
    }
    return "";
}


Ast_OptIntoList::Ast_OptIntoList(Ast_ColumnList *column_list) 
    : column_list(column_list)
{
}

Ast_OptIntoList::~Ast_OptIntoList() {
    if (column_list) delete column_list;
}

std::string Ast_OptIntoList::format() {
    return this->rawf("INTO %s",
        this->column_list->format().c_str()
    );
}


Ast_OptHaving::Ast_OptHaving(Ast_Expr *expr) 
    : expr(expr)
{
}

Ast_OptHaving::~Ast_OptHaving() {
    if (expr) delete expr;
}

std::string Ast_OptHaving::format() {
    if (expr) {
        return this->rawf("HAVING %s", this->expr->format().c_str());
    }
    return "";
}


Ast_OptGroupBy::Ast_OptGroupBy(Ast_GroupByList *groupby_list, bool opt_with_rollup) 
    : groupby_list(groupby_list), opt_with_rollup(opt_with_rollup)
{
}

Ast_OptGroupBy::~Ast_OptGroupBy() {
    if (groupby_list) delete groupby_list;
}

std::string Ast_OptGroupBy::format() {
    if (this->groupby_list)
        return this->rawf("GROUP BY %s %s",
            this->groupby_list->format().c_str(), 
            this->opt_with_rollup ? "WITH ROLLUP" : ""
        );

    return "";
}


Ast_OptColNames::Ast_OptColNames(Ast_ColumnList *column_list)
    : column_list(column_list)
{
}

Ast_OptColNames::~Ast_OptColNames() {
    if (column_list) delete column_list;
}

std::string Ast_OptColNames::format() {
    if (column_list)
        return '(' + column_list->format() + ')';
    return "";
}


Ast_OptOrderBy::Ast_OptOrderBy(Ast_GroupByList *orderby_list) 
    : orderby_list(orderby_list)
{
}

Ast_OptOrderBy::~Ast_OptOrderBy() {
    if (orderby_list) delete orderby_list;
}

std::string Ast_OptOrderBy::format() {
    if (orderby_list) {
        return this->rawf("ORDER BY %s", this->orderby_list->format().c_str());
    }
    return "";
}


Ast_OptWhere::Ast_OptWhere(Ast_Expr *expr)
    : expr(expr)
{
}

Ast_OptWhere::~Ast_OptWhere() 
{
    if (expr) delete expr;
}

std::string Ast_OptWhere::format() {
    if (expr) {
        return this->rawf("WHERE %s", this->expr->format().c_str());
    }
    return "";
}
