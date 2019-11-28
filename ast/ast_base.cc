#include <cstdio>
#include <cstdarg>
#include "ast_base.h"

int Ast_Base::next_ast_id = 0;
int Ast_Base::level = 0;

Ast_Base::Ast_Base() : node_id(next_ast_id++) {}

Ast_Base::~Ast_Base() {}

int Ast_Base::getID() const {return node_id;}

int Ast_Base::getLevel() const { return level; }
void Ast_Base::incLevel() { level ++; }
void Ast_Base::decLevel() { level --; }

std::string Ast_Base::rawf(const char *fmt, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    return buffer;
}

std::string Ast_Base::indentf(const char *fmt, ...) {
    char buffer[1024];

    std::string indent;
    for (int i = 0; i < this->getLevel(); i++)
        indent += "    ";
    
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    return indent + buffer;
}
