#include <map>
#include "ast_insert_stmt.h"
#include "ast_opts.h"
#include "ast_insert_val_list.h"
#include "ast_insert_asgn_list.h"
#include "ast_select_stmt.h"

Ast_InsertStmt::InsertStmtValList::InsertStmtValList(enum Ast_InsertStmt::_insert_opts insert_opts, 
    const char *name, Ast_OptColNames *opt_col_names, Ast_InsertValList *insert_val_list, 
    Ast_OptOnDupUpdate *opt_dupupdate)
    : insert_opts(insert_opts), name(name), opt_col_names(opt_col_names),  
        insert_val_list(insert_val_list), opt_dupupdate(opt_dupupdate)
{
}

Ast_InsertStmt::InsertStmtValList::~InsertStmtValList() {
    if (opt_col_names) delete opt_col_names;
    if (insert_val_list) delete insert_val_list;
    if (opt_dupupdate) delete opt_dupupdate;
}

Ast_InsertStmt::InsertStmtAsgnList::InsertStmtAsgnList(enum Ast_InsertStmt::_insert_opts insert_opts, 
    const char *name, Ast_InsertAsgnList *insert_asgn_list, Ast_OptOnDupUpdate *opt_ondupupdate)
    : insert_opts(insert_opts), name(name), insert_asgn_list(insert_asgn_list),
        opt_ondupupdate(opt_ondupupdate)
{
}

Ast_InsertStmt::InsertStmtAsgnList::~InsertStmtAsgnList() {
    if (insert_asgn_list) delete insert_asgn_list;
    if (opt_ondupupdate) delete opt_ondupupdate;
}

Ast_InsertStmt::InsertStmtSelect::InsertStmtSelect(enum Ast_InsertStmt::_insert_opts insert_opts, 
    const char *name, Ast_OptColNames *opt_col_names, Ast_SelectStmt *select_stmt, 
    Ast_OptOnDupUpdate *opt_ondupupdate)
    : insert_opts(insert_opts), name(name), opt_col_names(opt_col_names),
        select_stmt(select_stmt), opt_ondupupdate(opt_ondupupdate)
{
}

Ast_InsertStmt::InsertStmtSelect::~InsertStmtSelect() {
    if (opt_col_names) delete opt_col_names;
    if (select_stmt) delete select_stmt;
    if (opt_ondupupdate) delete opt_ondupupdate;
}

Ast_InsertStmt::Ast_InsertStmt(enum Ast_InsertStmt::_insert_opts insert_opts, const char *name, 
    Ast_OptColNames *opt_col_names, Ast_InsertValList *insert_val_list, Ast_OptOnDupUpdate *opt_dupupdate)
    : insert_type(Ast_InsertStmt::INSERT_TYPE_VALLIST),
        _val_list(new Ast_InsertStmt::InsertStmtValList(insert_opts, name, opt_col_names, insert_val_list, opt_dupupdate))
{
}

Ast_InsertStmt::Ast_InsertStmt(enum Ast_InsertStmt::_insert_opts insert_opts, const char *name,
    Ast_InsertAsgnList *insert_asgn_list, Ast_OptOnDupUpdate *opt_ondupupdate)
    : insert_type(Ast_InsertStmt::INSERT_TYPE_ASGNLIST),
        _asgn_list(new Ast_InsertStmt::InsertStmtAsgnList(insert_opts, name, insert_asgn_list, opt_ondupupdate))
{
}

Ast_InsertStmt::Ast_InsertStmt(enum Ast_InsertStmt::_insert_opts insert_opts, const char *name, 
    Ast_OptColNames *opt_col_names, Ast_SelectStmt *select_stmt, Ast_OptOnDupUpdate *opt_ondupupdate)
    : insert_type(Ast_InsertStmt::INSERT_TYPE_SELECT),
        _select(new Ast_InsertStmt::InsertStmtSelect(insert_opts, name, opt_col_names, select_stmt, opt_ondupupdate))
{
}

Ast_InsertStmt::~Ast_InsertStmt() {
    switch (insert_type)
    {
    case Ast_InsertStmt::INSERT_TYPE_VALLIST:
        delete this->_val_list;
        break;
    case Ast_InsertStmt::INSERT_TYPE_ASGNLIST:
        delete this->_asgn_list;
        break;
    case Ast_InsertStmt::INSERT_TYPE_SELECT:
        delete this->_select;
        break;
    default:
        break;
    }
}

std::string Ast_InsertStmt::insertOptName(enum Ast_InsertStmt::_insert_opts insert_opts) {
    const static std::map<int, std::string> names = {
        {1 << 0, "LOW PRIORITY"},
        {1 << 1, "DELAYED"},
        {1 << 2, "HIGH PRIORITY"},
        {1 << 3, "IGNORE"}
    };

    std::string str;
    for (std::map<int, std::string>::const_iterator it = names.begin();
        it != names.end();
        ++it) {
            if (insert_opts & it->first) {
                str += it->second;
            }
        }

    return str;
}

std::string Ast_InsertStmt::format() {
    switch (this->insert_type)
    {
    case Ast_InsertStmt::INSERT_TYPE_VALLIST:
        return this->rawf("INSERT %s INTO %s %s VALUES %s %s", 
            this->insertOptName(this->_val_list->insert_opts).c_str(),
            this->_val_list->name.c_str(),
            this->_val_list->opt_col_names->format().c_str(),
            this->_val_list->opt_col_names->format().c_str(),
            this->_val_list->opt_dupupdate->format().c_str()
        );
    case Ast_InsertStmt::INSERT_TYPE_ASGNLIST:
        return this->rawf("INSERT %s INTO %s SET %s %s", 
            this->insertOptName(this->_asgn_list->insert_opts).c_str(),
            this->_asgn_list->name.c_str(),
            this->_asgn_list->insert_asgn_list->format().c_str(),
            this->_asgn_list->opt_ondupupdate->format().c_str()
        );
    case Ast_InsertStmt::INSERT_TYPE_SELECT:
        return this->rawf("INSERT %s INTO %s %s %s %s", 
            this->insertOptName(this->_select->insert_opts).c_str(),
            this->_select->name.c_str(),
            this->_select->opt_col_names->format().c_str(),
            this->_select->select_stmt->format().c_str(),
            this->_select->opt_ondupupdate->format().c_str()
        );
    default:
        break;
    }

    return "";
}
