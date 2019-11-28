#include "ast_set_stmt.h"
#include "ast_expr.h"

Ast_SetExpr::Ast_SetExpr(const char *uservar, Ast_Expr *expr) 
    : uservar(uservar), expr(expr)
{
}

Ast_SetExpr::~Ast_SetExpr() {
    if (expr) delete expr;
}

std::string Ast_SetExpr::format() {
    return this->rawf("%s = %s", this->uservar.c_str(), this->expr->format().c_str());
}

Ast_SetList::Ast_SetList(Ast_SetExpr *expr) {
    exprs.push_back(expr);
}

Ast_SetList::~Ast_SetList() {
    for (std::vector<Ast_SetExpr *>::iterator it = exprs.begin();
        it != exprs.end();
        it ++)
    {
        if (*it)
            delete (*it);
    }
}

void Ast_SetList::addSetExpr(Ast_SetExpr *expr) {
    exprs.push_back(expr);
}

std::string Ast_SetList::format() {
    std::string str;

    if (!this->exprs.empty()) {
        str = exprs[0]->format();
        
        for (std::vector<Ast_SetExpr *>::iterator it = exprs.begin() + 1;
            it != exprs.end();
            it ++)
        {
            if (*it)
                str += (",\n " + (*it)->format());
        }
    }

    return str;
}

Ast_SetStmt::Ast_SetStmt(Ast_SetList *set_list) 
    : set_list(set_list)
{
}

Ast_SetStmt::~Ast_SetStmt() {
    if (set_list)
        delete set_list;
}

std::string Ast_SetStmt::format() {
    return this->rawf("SET %s", this->set_list->format().c_str());
}
