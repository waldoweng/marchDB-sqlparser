#include "ast_update_stmt.h"
#include "ast_expr.h"
#include "ast_table_reference.h"
#include "ast_opts.h"

Ast_UpdateAsgnList::UpdateAsgn::UpdateAsgn(const char *tablename, const char *name, Ast_Expr *expr)
    : tablename(tablename), name(name), expr(expr)
{
}

Ast_UpdateAsgnList::UpdateAsgn::~UpdateAsgn() {
    if (this->expr) delete this->expr;
}

Ast_UpdateAsgnList::Ast_UpdateAsgnList(const char *name, Ast_Expr *expr)
{
    this->update_asgn_list.push_back(
        new Ast_UpdateAsgnList::UpdateAsgn("", name, expr)
    );
}

Ast_UpdateAsgnList::Ast_UpdateAsgnList(const char *tablename, const char *name, Ast_Expr *expr) 
{
    this->update_asgn_list.push_back(
        new Ast_UpdateAsgnList::UpdateAsgn(tablename, name, expr)
    );
}

Ast_UpdateAsgnList::~Ast_UpdateAsgnList() {
    for (std::vector<Ast_UpdateAsgnList::UpdateAsgn *>::iterator it = this->update_asgn_list.begin();
        it != this->update_asgn_list.end();
        it ++)
    {
        if (*it) delete (*it);
    }
}

void Ast_UpdateAsgnList::addUpdateAsgn(const char *name, Ast_Expr *expr)
{
    this->update_asgn_list.push_back(
        new Ast_UpdateAsgnList::UpdateAsgn("", name, expr)
    );
}

void Ast_UpdateAsgnList::addUpdateAsgn(const char *tablename, const char *name, Ast_Expr *expr) 
{
    this->update_asgn_list.push_back(
        new Ast_UpdateAsgnList::UpdateAsgn(tablename, name, expr)
    );
}

std::string Ast_UpdateAsgnList::format() {
    std::string str;

    if (!this->update_asgn_list.empty()) {
        str = this->rawf("%s = %s", 
            this->update_asgn_list[0]->tablename.empty() 
                ? this->update_asgn_list[0]->name.c_str() 
                : (this->update_asgn_list[0]->name + this->update_asgn_list[0]->tablename).c_str(),
            this->update_asgn_list[0]->expr->format().c_str()
        );

        for (std::vector<Ast_UpdateAsgnList::UpdateAsgn *>::iterator it = this->update_asgn_list.begin();
            it != this->update_asgn_list.end();
            it ++)
        {
            str += ", ";
            str += this->rawf("%s = %s", 
                (*it)->tablename.empty() ? (*it)->name.c_str() : ((*it)->name + (*it)->tablename).c_str(),
                (*it)->expr->format().c_str()
            );
        }
    }

    return str;
}

Ast_UpdateStmt::Ast_UpdateStmt(enum Ast_UpdateStmt::update_opts update_opts, 
    Ast_TableReferences *table_references, Ast_UpdateAsgnList *update_asgn_list, Ast_OptWhere *opt_where,
    Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit)
    : update_opts(update_opts), references(table_references), update_asgn_list(update_asgn_list),
        opt_where(opt_where), opt_orderby(opt_orderby), opt_limit(opt_limit)
{
}

Ast_UpdateStmt::~Ast_UpdateStmt() {
    if (this->references) delete this->references;
    if (this->update_asgn_list) delete this->update_asgn_list;
    if (this->opt_where) delete this->opt_where;
    if (this->opt_orderby) delete this->opt_orderby;
    if (this->opt_limit) delete this->opt_limit;
}

const char * Ast_UpdateStmt::updateOptsName(enum Ast_UpdateStmt::update_opts update_opts) {
    static const char names[3][32] = {
        "",
        "LOW_PRIORITY",
        "IGNORE"
    };
    return names[update_opts];
}

std::string Ast_UpdateStmt::format() {
    return this->rawf("UPDATE %s %s SET %s %s %s %s", 
        this->updateOptsName(this->update_opts),
        this->references ? this->references->format().c_str() : "",
        this->update_asgn_list ? this->update_asgn_list->format().c_str() : "",
        this->opt_where ? this->opt_where->format().c_str() : "",
        this->opt_orderby ? this->opt_orderby->format().c_str() : "",
        this->opt_limit ? this->opt_limit->format().c_str() : ""
    );
}
