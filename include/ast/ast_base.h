#ifndef FONDLE_FISH_MYSQL_AST_BASE_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_BASE_H_INCLUDED

#include <string>

class Ast_Base {
public:
    explicit Ast_Base();
    virtual ~Ast_Base();

public:
    int getID() const;

    int getLevel() const;
    void incLevel();
    void decLevel();

    virtual std::string format() = 0;

protected:
    std::string indentf(const char *fmt, ...);
    std::string rawf(const char *fmt, ...);

private:
    int node_id;
private:
    static int next_ast_id;
    static int level;
};

#endif
