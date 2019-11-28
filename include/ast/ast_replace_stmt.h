#ifndef FONDLE_FISH_MYSQL_AST_REPLACE_STMT_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_REPLACE_STMT_H_INCLUDED

#include <cstdint>
#include <string>
#include "ast_base.h"

class Ast_OptColNames;
class Ast_InsertValList;
class Ast_OptOnDupUpdate;
class Ast_InsertAsgnList;
class Ast_SelectStmt;

class Ast_ReplaceStmt : public Ast_Base {
public:
    enum _replace_opts {
        REPLACE_OPTS_LOW_PRIORITY    = 1 << 0,
        REPLACE_OPTS_DELAYED         = 1 << 2,
        REPLACE_OPTS_HIGH_PRIORITY   = 1 << 3,
        REPLACE_OPTS_IGNORE          = 1 << 4
    };
    enum replace_type {
        REPLACE_TYPE_VALLIST     = 0,
        REPLACE_TYPE_ASGNLIST    = 1,
        REPLACE_TYPE_SELECT      = 2
    };

public:
    class ReplaceStmtValList {
    public:
        explicit ReplaceStmtValList(enum _replace_opts insert_opts, const char *name, Ast_OptColNames *opt_col_names,
            Ast_InsertValList *insert_val_list, Ast_OptOnDupUpdate *opt_dupupdate);
        ~ReplaceStmtValList();
    public:
        enum _replace_opts replace_opts;
        std::string name;
        Ast_OptColNames *opt_col_names;
        Ast_InsertValList *insert_val_list;
        Ast_OptOnDupUpdate *opt_dupupdate;
    };

    class ReplaceStmtAsgnList {
    public:
        explicit ReplaceStmtAsgnList(enum _replace_opts insert_opts, const char *name, 
            Ast_InsertAsgnList *insert_asgn_list, Ast_OptOnDupUpdate *opt_ondupupdate);
        ~ReplaceStmtAsgnList();
    public:
        enum _replace_opts replace_opts;
        std::string name; 
        Ast_InsertAsgnList *insert_asgn_list;
        Ast_OptOnDupUpdate *opt_ondupupdate;
    };

    class ReplaceStmtSelect {
    public:
        explicit ReplaceStmtSelect(enum _replace_opts insert_opts, const char *name, Ast_OptColNames *opt_col_names,
            Ast_SelectStmt *select_stmt, Ast_OptOnDupUpdate *opt_ondupupdate);
        virtual ~ReplaceStmtSelect();
    public:
        enum _replace_opts replace_opts;
        std::string name;
        Ast_OptColNames *opt_col_names;
        Ast_SelectStmt *select_stmt;
        Ast_OptOnDupUpdate *opt_ondupupdate;
    };

public:
    explicit Ast_ReplaceStmt(enum _replace_opts insert_opts, const char *name, Ast_OptColNames *opt_col_names,
        Ast_InsertValList *insert_vals_list, Ast_OptOnDupUpdate *opt_ondupupdate);
    explicit Ast_ReplaceStmt(enum _replace_opts insert_opts, const char *name, Ast_InsertAsgnList *insert_asgn_list,
        Ast_OptOnDupUpdate *opt_ondupupdate);
    explicit Ast_ReplaceStmt(enum _replace_opts insert_opts, const char *name, Ast_OptColNames *opt_col_names,
        Ast_SelectStmt *select_stmt, Ast_OptOnDupUpdate *opt_ondupupdate);
    virtual ~Ast_ReplaceStmt();

public:
    virtual std::string format();

private:
    std::string replaceOptName(enum _replace_opts insert_opts);

private:
    enum replace_type replace_type;
    union {
        ReplaceStmtValList *_val_list;
        ReplaceStmtAsgnList *_asgn_list;
        ReplaceStmtSelect *_select;
    };

};

#endif
