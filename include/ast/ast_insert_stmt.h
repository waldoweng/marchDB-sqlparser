#ifndef FONDLE_FISH_MYSQL_AST_INSERT_STMT_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_INSERT_STMT_H_INCLUDED

#include <cstdint>
#include <string>
#include "ast_base.h"

class Ast_OptColNames;
class Ast_InsertValList;
class Ast_OptOnDupUpdate;
class Ast_InsertAsgnList;
class Ast_SelectStmt;

class Ast_InsertStmt : public Ast_Base {
public:
    enum _insert_opts {
        INSERT_OPTS_LOW_PRIORITY    = 1 << 1,
        INSERT_OPTS_DELAYED         = 1 << 2,
        INSERT_OPTS_HIGH_PRIORITY   = 1 << 3,
        INSERT_OPTS_IGNORE          = 1 << 4
    };
    enum insert_type {
        INSERT_TYPE_VALLIST     = 0,
        INSERT_TYPE_ASGNLIST    = 1,
        INSERT_TYPE_SELECT      = 2
    };

    class InsertStmtValList {
    public:
        explicit InsertStmtValList(enum _insert_opts insert_opts, const char *name, Ast_OptColNames *opt_col_names,
            Ast_InsertValList *insert_val_list, Ast_OptOnDupUpdate *opt_dupupdate);
        ~InsertStmtValList();
    public:
        enum _insert_opts insert_opts;
        std::string name;
        Ast_OptColNames *opt_col_names;
        Ast_InsertValList *insert_val_list;
        Ast_OptOnDupUpdate *opt_dupupdate;
    };

    class InsertStmtAsgnList {
    public:
        explicit InsertStmtAsgnList(enum _insert_opts insert_opts, const char *name, 
            Ast_InsertAsgnList *insert_asgn_list, Ast_OptOnDupUpdate *opt_ondupupdate);
        ~InsertStmtAsgnList();
    public:
        enum _insert_opts insert_opts;
        std::string name; 
        Ast_InsertAsgnList *insert_asgn_list;
        Ast_OptOnDupUpdate *opt_ondupupdate;
    };

    class InsertStmtSelect {
    public:
        explicit InsertStmtSelect(enum _insert_opts insert_opts, const char *name, Ast_OptColNames *opt_col_names,
            Ast_SelectStmt *select_stmt, Ast_OptOnDupUpdate *opt_ondupupdate);
        virtual ~InsertStmtSelect();
    public:
        enum _insert_opts insert_opts;
        std::string name;
        Ast_OptColNames *opt_col_names;
        Ast_SelectStmt *select_stmt;
        Ast_OptOnDupUpdate *opt_ondupupdate;
    };

public:
    explicit Ast_InsertStmt(enum _insert_opts insert_opts, const char *name, Ast_OptColNames *opt_col_names,
        Ast_InsertValList *insert_val_list, Ast_OptOnDupUpdate *opt_dupupdate);
    explicit Ast_InsertStmt(enum _insert_opts insert_opts, const char *name, Ast_InsertAsgnList *insert_asgn_list,
        Ast_OptOnDupUpdate *opt_ondupupdate);
    explicit Ast_InsertStmt(enum _insert_opts insert_opts, const char *name, Ast_OptColNames *opt_col_names,
        Ast_SelectStmt *select_stmt, Ast_OptOnDupUpdate *opt_ondupupdate);
    virtual ~Ast_InsertStmt();

public:
    virtual std::string format();

private:
    std::string insertOptName(enum _insert_opts insert_opts);

private:
    insert_type insert_type;
    union {
        InsertStmtValList *_val_list;
        InsertStmtAsgnList *_asgn_list;
        InsertStmtSelect *_select;
    };
};

#endif
