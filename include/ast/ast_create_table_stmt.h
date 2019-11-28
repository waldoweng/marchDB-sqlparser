#ifndef FONDLE_FISH_MYSQL_AST_CREATE_TABLE_STMT_H_INCLUDED
#define FONDLE_FISH_MYSQL_AST_CREATE_TABLE_STMT_H_INCLUDED

#include <map>
#include <string>
#include <vector>
#include "ast_base.h"

class Ast_ColumnList;
class Ast_SelectStmt;

class Ast_OptCSC {
public:
    enum csc_type {
        CSC_TYPE_CHARSET = 0,
        CSC_TYPE_COLLATE = 1
    };
public:
    explicit Ast_OptCSC(csc_type csc_type, const char *value);
    virtual ~Ast_OptCSC();

public:
    csc_type csc_type;
    std::string value;
};

class Ast_EnumList : public Ast_Base {
public:
    explicit Ast_EnumList(const char *name);
    virtual ~Ast_EnumList();

public:
    virtual std::string format();

public:
    void addEnum(const char *name);

private:
    std::vector<std::string> enum_list;
};

class Ast_DataType : public Ast_Base {
public:
    enum data_type {
        DATA_TYPE_BIT         = 1,
        DATA_TYPE_TINYINT     = 2,
        DATA_TYPE_SMALLINT    = 3,
        DATA_TYPE_MEDIUMINT   = 4,
        DATA_TYPE_INTEGER     = 5,
        DATA_TYPE_BIGINT      = 6,
        DATA_TYPE_REAL        = 7,
        DATA_TYPE_DOUBLE      = 8,
        DATA_TYPE_FLOAT       = 9,
        DATA_TYPE_DECIMAL     = 10,
        DATA_TYPE_DATE        = 11,
        DATA_TYPE_TIME        = 12,
        DATA_TYPE_TIMESTAMP   = 13,
        DATA_TYPE_DATETIME    = 14,
        DATA_TYPE_YEAR        = 15,
        DATA_TYPE_CHAR        = 16,
        DATA_TYPE_VARCHAR     = 17,
        DATA_TYPE_BINARY      = 18,
        DATA_TYPE_VARBINARY   = 19,
        DATA_TYPE_TINYBLOB    = 20,
        DATA_TYPE_BLOB        = 21,
        DATA_TYPE_MEDIUMBLOB  = 22,
        DATA_TYPE_LONGBLOB    = 23,
        DATA_TYPE_TINYTEXT    = 24,
        DATA_TYPE_TEXT        = 25,
        DATA_TYPE_MEDIUMTEXT  = 26,
        DATA_TYPE_LONGTEXT    = 27,
        DATA_TYPE_ENUM        = 28,
        DATA_TYPE_SET         = 29
    };

    struct NumericType {
        uint32_t length;
        bool unsigned_flag;
        bool zerofill_flag;
    };

    struct StringType {
        uint32_t length;
        bool binary_flag;
        std::string charset;
        std::string collate;
    };

    struct CompondType {
        Ast_EnumList *enum_list;
        std::string charset;
        std::string collate;
    };

public:
    explicit Ast_DataType(data_type data_type);
    explicit Ast_DataType(data_type data_type, uint32_t length, bool unsigned_flag, bool zerofill_flag);
    explicit Ast_DataType(data_type data_type, uint32_t length, bool binary_flag, Ast_OptCSC *opt_csc);
    explicit Ast_DataType(data_type data_type, Ast_EnumList *enum_list, Ast_OptCSC *opt_csc);
    virtual ~Ast_DataType();

public:
    virtual std::string format();

private:
    const char * typeName(data_type data_type);
    std::string format(data_type data_type);
    std::string format(data_type data_type, NumericType numeric);
    std::string format(data_type data_type, StringType str);
    std::string format(data_type data_type, CompondType compond);

private:
    data_type data_type;
    union {
        NumericType *numeric;
        StringType *str;
        CompondType *compond;
    };
};

class Ast_ColumnAttrs : public Ast_Base {
public:
    enum mask{
        ATTR_NOT_NULL            = 0x0001,
        ATTR_DEFAULT_STRING      = 0x0002,
        ATTR_DEFAULT_INTNUM      = 0x0004,
        ATTR_DEFAULT_APPROXNUM   = 0x0008,
        ATTR_DEFAULT_BOOL        = 0x0010,
        ATTR_AUTO_INCREMENT      = 0x0020,
        ATTR_UNIQ                = 0x0040,
        ATTR_UNIQ_KEY            = 0x0080,
        ATTR_PRIMARY_KEY         = 0x0100,
        ATTR_INDEX_KEY           = 0x0200,
        ATTR_COMMENT             = 0x0400
    };

    union _attr {
        char *str_attr;
        uint32_t int_attr;
        double float_attr;
        Ast_ColumnList *column_list;
    };

public:
    explicit Ast_ColumnAttrs();
    virtual ~Ast_ColumnAttrs();

public:
    virtual std::string format();

public:
    void addAttr(mask mask);
    void addUInt32Attr(mask mask, uint32_t attr);
    void addDoubleAttr(mask mask, double attr);
    void addAttr(mask mask, char *attr);
    void addBoolAttr(mask mask, bool attr);
    void addAttr(mask mask, Ast_ColumnList *column_list);

private:
    uint32_t attr_mask; 
    std::map<mask, Ast_ColumnAttrs::_attr> attrs;
};

class Ast_CreateDefinition : public Ast_Base {
public:
    enum def_type {
        DATA_DEFINITION = 0,
        INDEX_DEFINITION = 1
    };
    enum _key_type {
        KEY_TYPE_KEY = 0,
        KEY_TYPE_PRIMARY_KEY = 1,
        KEY_TYPE_FULLTEXT_KEY = 2
    };

public:
    class DataDefinition {
    public:
        explicit DataDefinition(const char *name, Ast_DataType *data_type, Ast_ColumnAttrs *attrs);
        ~DataDefinition();
    public:
        std::string name;
        Ast_DataType *data_type;
        Ast_ColumnAttrs *column_attrs;
    };

    class IndexDefinition {
    public:
        explicit IndexDefinition(enum _key_type type, Ast_ColumnList *column_list);
        ~IndexDefinition();
    public:
        enum _key_type key_type;
        Ast_ColumnList *column_list;
    };

public:
    explicit Ast_CreateDefinition(enum _key_type type, Ast_ColumnList *column_list);
    explicit Ast_CreateDefinition(const char *name, Ast_DataType *data_type, Ast_ColumnAttrs *column_attr);
    virtual ~Ast_CreateDefinition();

public:
    virtual std::string format();

private:
    const char * keyTypeName(enum _key_type key_type);

private:
    def_type def_type;
    union {
        IndexDefinition *index_def;
        DataDefinition *data_def;
    };
};

class Ast_CreateColList : public Ast_Base {
public:
    explicit Ast_CreateColList(Ast_CreateDefinition *create_definition);
    virtual ~Ast_CreateColList();

public:
    virtual std::string format();

public:
    void addCreateDefinition(Ast_CreateDefinition *create_definition);

private:
    std::vector<Ast_CreateDefinition *> definitions;
};

class Ast_CreateSelectStmt : public Ast_Base {
public:
    enum {
        OPT_DEFAULT = 0,
        OPT_IGNORE  = 1,
        OPT_REPLACE = 2
    };
public:
    explicit Ast_CreateSelectStmt(uint32_t opt_ignore_replace, Ast_SelectStmt *select_stmt);
    virtual ~Ast_CreateSelectStmt();

public:
    virtual std::string format();

private:
    const char * OptName(uint32_t opt_ignore_replace);

private:
    uint32_t opt_ignore_replace;
    Ast_SelectStmt *select_stmt;
};

class Ast_CreateTableStmt : public Ast_Base {
public:
    explicit Ast_CreateTableStmt(bool opt_temporary, bool opt_if_not_exists, const char * name, 
        Ast_CreateColList *create_col_list);
    explicit Ast_CreateTableStmt(bool opt_temporary, bool opt_if_not_exists, 
        const char * database_name, const char * name, Ast_CreateColList *create_col_list);
    explicit Ast_CreateTableStmt(bool opt_temporary, bool opt_if_not_exists,
        const char * name, Ast_CreateColList *create_col_list, Ast_CreateSelectStmt *create_select_stmt);
    explicit Ast_CreateTableStmt(bool opt_temporary, bool opt_if_not_exists, const char * name, 
        Ast_CreateSelectStmt *create_select_stmt);
    explicit Ast_CreateTableStmt(bool opt_temporary, bool opt_if_not_exists, const char * database_name,
        const char * name, Ast_CreateColList *create_col_list, Ast_CreateSelectStmt *create_select_stmt);
    explicit Ast_CreateTableStmt(bool opt_temporary, bool opt_if_not_exists, const char * database_name,
        const char * name, Ast_CreateSelectStmt *create_select_stmt);
    virtual ~Ast_CreateTableStmt();

public:
    virtual std::string format();

private:
    bool opt_temporary;
    bool opt_if_not_exists;
    std::string database_name;
    std::string name;
    Ast_CreateColList *create_col_list;
    Ast_CreateSelectStmt *create_select_stmt;
};

#endif
