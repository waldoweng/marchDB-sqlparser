#include "ast_insert_val_list.h"
#include "ast_expr.h"

Ast_InsertVals::Ast_InsertVals(Ast_Expr *expr) {
    insert_vals.push_back(expr);
}

Ast_InsertVals::~Ast_InsertVals() {
    for (std::vector<Ast_Expr *>::iterator it = insert_vals.begin();
        it != insert_vals.end();
        it ++)
    {
        delete *it;
    }
}

void Ast_InsertVals::addInsertVal(Ast_Expr *expr) {
    insert_vals.push_back(expr);
}

std::string Ast_InsertVals::format() {
    std::string str;

    if (!this->insert_vals.empty()) {
        str = this->insert_vals[0]->format();

        for (std::vector<Ast_Expr *>::iterator it = insert_vals.begin() + 1;
            it != insert_vals.end();
            it ++) 
        {
            if (*it)
                str += ", " + (*it)->format();
            else
                str += ", DEFAULT";
        }
    }

    return str;
}


Ast_InsertValList::Ast_InsertValList(Ast_InsertVals *insert_vals) {
    insert_vals_list.push_back(insert_vals);
}

Ast_InsertValList::~Ast_InsertValList() {
    for (std::vector<Ast_InsertVals *>::iterator it = insert_vals_list.begin();
        it != insert_vals_list.end();
        it ++)
    {
        delete *it;
    }
}

void Ast_InsertValList::addInsertVals(Ast_InsertVals *insert_vals) {
    insert_vals_list.push_back(insert_vals);
}

std::string Ast_InsertValList::format() {
    std::string str;

    if (!this->insert_vals_list.empty()) {
        str = '(' + insert_vals_list[0]->format() + ')';

        for (std::vector<Ast_InsertVals *>::iterator it = insert_vals_list.begin() + 1;
            it != insert_vals_list.end();
            it ++)
        {
            str += ", (" + (*it)->format() + ')';
        }
    }

    return str;
}