#include <cstdio>
#include "ast_column_list.h"

Ast_ColumnList::Ast_ColumnList(const char *name) {
    free_unique_ptr f1(name);
    names.push_back(std::move(f1));
}

Ast_ColumnList::~Ast_ColumnList() {}

void Ast_ColumnList::addName(const char *name) {
    free_unique_ptr f1(name);
    names.push_back(std::move(f1));
}

std::string Ast_ColumnList::format() {
    std::string str;

    if (!names.empty())
        str = this->indentf("%s", names[0].get());

    for (std::vector<free_unique_ptr>::iterator it = names.begin() + 1;
        it != names.end();
        it ++) {
        str += this->rawf(", %s", (*it).get());
    }

    return str;
}
