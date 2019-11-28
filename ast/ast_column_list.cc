#include <cstdio>
#include "ast_column_list.h"

Ast_ColumnList::Ast_ColumnList(const char *name) {
    names.push_back(name);
}

Ast_ColumnList::~Ast_ColumnList() {}

void Ast_ColumnList::addName(const char *name) {
    names.push_back(name);
}

std::string Ast_ColumnList::format() {
    std::string str;

    if (!names.empty())
        str = this->indentf("%s", names[0].c_str());

    for (std::vector<std::string>::iterator it = names.begin() + 1;
        it != names.end();
        it ++) {
        str += this->rawf(", %s", it->c_str());
    }

    return str;
}