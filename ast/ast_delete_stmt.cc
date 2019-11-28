#include "ast_delete_stmt.h"
#include "ast_opts.h"
#include "ast_table_reference.h"

Ast_DeleteList::Ast_DeleteList(const char *name) {
    names.push_back(name);
}

Ast_DeleteList::~Ast_DeleteList() {}

std::string Ast_DeleteList::format() {
    std::string tmp;

    if (!names.empty()) {

        tmp = names[0];

        for (std::vector<std::string>::iterator it = names.begin() + 1;
            it != names.end();
            it ++)
            tmp += (' ' + *it);
    }

    return tmp;
}

void Ast_DeleteList::addName(const char *name) {
    names.push_back(name);
}


Ast_DeleteStmt::SingleTableDeleteStmt::SingleTableDeleteStmt(enum Ast_DeleteStmt::_delete_opts delete_opts, 
    const char *name, Ast_OptWhere *opt_where, Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit)
    : delete_opts(delete_opts), name(name), opt_where(opt_where), opt_orderby(opt_orderby),
        opt_limit(opt_limit)
{
}

Ast_DeleteStmt::SingleTableDeleteStmt::~SingleTableDeleteStmt()
{
    if (opt_where) delete opt_where;
    if (opt_orderby) delete opt_orderby;
    if (opt_limit) delete opt_limit;
}

Ast_DeleteStmt::MultipleTableDeleteStmt::MultipleTableDeleteStmt(enum Ast_DeleteStmt::_delete_opts delete_opts, 
    Ast_DeleteList *delete_list, Ast_TableReferences *table_references, Ast_OptWhere *opt_where)
    : delete_opts(delete_opts), delete_list(delete_list), table_references(table_references),
        opt_where(opt_where)
{
}

Ast_DeleteStmt::MultipleTableDeleteStmt::~MultipleTableDeleteStmt()
{
    if (delete_list) delete delete_list;
    if (table_references) delete table_references;
    if (opt_where) delete opt_where;
}

Ast_DeleteStmt::Ast_DeleteStmt(enum Ast_DeleteStmt::_delete_opts delete_opts, const char *name, 
    Ast_OptWhere *opt_where, Ast_OptOrderBy *opt_orderby, Ast_OptLimit *opt_limit)
    : delete_type(Ast_DeleteStmt::SINGLE_TABLE),
        single_stmt(new SingleTableDeleteStmt(delete_opts, name, opt_where, opt_orderby, opt_limit))
{
}

Ast_DeleteStmt::Ast_DeleteStmt(enum Ast_DeleteStmt::_delete_opts delete_opts, Ast_DeleteList *delete_list, 
    Ast_TableReferences *table_references, Ast_OptWhere *opt_where)
    : delete_type(Ast_DeleteStmt::MULTIPLE_TABLE),
        multi_stmt(new MultipleTableDeleteStmt(delete_opts, delete_list, table_references, opt_where))
{
}

Ast_DeleteStmt::~Ast_DeleteStmt() {
    switch (delete_type)
    {
    case Ast_DeleteStmt::SINGLE_TABLE:
        delete this->single_stmt;
        break;
    case Ast_DeleteStmt::MULTIPLE_TABLE:
        delete this->multi_stmt;
        break;
    default:
        break;
    }
}

const char * Ast_DeleteStmt::deleteOptName(enum Ast_DeleteStmt::_delete_opts delete_opts) {
    static const char names[3][32] = {
        "LOW_PRIORITY",
        "QUICK",
        "IGNORE"
    };
    return names[delete_opts-1];
}

std::string Ast_DeleteStmt::format() {
    std::string str;

    switch (delete_type)
    {
    case Ast_DeleteStmt::SINGLE_TABLE:
        str = this->rawf("DELETE %s FROM %s\n%s\n%s\n%s",
            this->deleteOptName(this->single_stmt->delete_opts),
            this->single_stmt->name.c_str(),
            this->single_stmt->opt_where->format().c_str(),
            this->single_stmt->opt_orderby->format().c_str(),
            this->single_stmt->opt_limit->format().c_str()
        );
        break;
    case Ast_DeleteStmt::MULTIPLE_TABLE:
        str = this->rawf("DELETE %s %s FROM %s\n%s", 
            this->deleteOptName(this->multi_stmt->delete_opts),
            this->multi_stmt->delete_list->format().c_str(),
            this->multi_stmt->table_references->format().c_str(),
            this->multi_stmt->opt_where->format().c_str()
        );
        break;
    default:
        break;
    }

    return str;
}
