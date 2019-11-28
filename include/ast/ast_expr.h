#ifndef FONDLE_FISH_AST_EXPR_H_INCLUDE
#define FONDLE_FISH_AST_EXPR_H_INCLUDE

#include <string>
#include <vector>
#include "ast_base.h"

class Ast_SelectStmt;
class Ast_ValList;
class Ast_Expr;

class Ast_CaseList : public Ast_Base{
public:
    struct case_item {
        Ast_Expr *when_expr;
        Ast_Expr *then_expr;
    };

public:
    explicit Ast_CaseList(Ast_Expr *when, Ast_Expr *then);
    virtual ~Ast_CaseList();

public:
    void addCase(Ast_Expr *when, Ast_Expr *then);

public:
    virtual std::string format();

private:
    std::vector<case_item> cases;
};



class Ast_IntervalExp : public Ast_Base {
public:
    enum interval_type {
        INTERVAL_DAY_HOUR = 1,
        INTERVAL_DAY_MICROSECOND = 2,
        INTERVAL_DAY_MINUTE = 3,
        INTERVAL_DAY_SECOND = 4,
        INTERVAL_YEAR_MONTH = 5,
        INTERVAL_YEAR = 6,
        INTERVAL_HOUR_MICROSECOND = 7,
        INTERVAL_HOUR_MINUTE = 8,
        INTERVAL_HOUR_SECOND = 9,
    };

public:
    explicit Ast_IntervalExp(interval_type interval_type, Ast_Expr *expr);
    virtual ~Ast_IntervalExp();

public:
    virtual std::string format();

private:
    const char * intervalTypeName(interval_type interval_type);

private:
    interval_type interval_type;
    Ast_Expr *expr;
};


class Ast_ValList : public Ast_Base {
public:
    explicit Ast_ValList(Ast_Expr *expr);
    virtual ~Ast_ValList();

public:
    virtual std::string format();

public:
    void addExpr(Ast_Expr *expr);

private:
    std::vector<Ast_Expr *> exprs;
};

class Ast_Expr : public Ast_Base {
public:
    explicit Ast_Expr();
    virtual ~Ast_Expr();
    
public:
    virtual Ast_Expr eval() const;
    virtual std::string format();
};


class Ast_LiteralExpr : public Ast_Expr {
public:
    enum literal_type {
        LiteralTypeName         = 0,
        LiteralTypeDetailName   = 1,
        LiteralTypeUserVar      = 2,
        LiteralTypeString       = 3,
        LiteralTypeIntNum       = 4,
        LiteralTypeApproxNum    = 5,
        LiteralTypeBool         = 6,
        LiteralTypeCurTs        = 7,
        LiteralTypeCurDate      = 8,
        LiteralTypeCurTime      = 9
    };

public:
    explicit Ast_LiteralExpr(literal_type literal_type, const char *first, const char *second);
    explicit Ast_LiteralExpr(int int_var);
    explicit Ast_LiteralExpr(double float_var);
    explicit Ast_LiteralExpr(bool bool_var);
    explicit Ast_LiteralExpr(literal_type literal_type);
    virtual ~Ast_LiteralExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    literal_type literal_type;
    /* TODO switch to union temporary */
    union {
        struct {
            std::string first;
            std::string second;
        } name;
        int int_var;
        double float_var;
        bool bool_var;
    };
};

class Ast_ArithmeticExpr : public Ast_Expr {
public:
    enum arithmetic_type {
        CompoundTypeAdd         = 0,
        CompoundTypeSub         = 1,
        CompoundTypeMul         = 2,
        CompoundTypeDiv         = 3,
        CompoundTypeMod         = 4,
        CompoundTypeNeg         = 5,
        CompoundTypeAnd         = 6,
        CompoundTypeOr          = 7,
        CompoundTypeXor         = 8,
        CompoundTypeBitOr       = 9,
        CompoundTypeBitAnd      = 10,
        CompoundTypeBitXor      = 11,
        CompoundTypeLeftShift   = 12,
        CompoundTypeRightShift  = 13,
        CompoundTypeNot         = 14,
        CompoundTypeCompareEQ   = 15,
        CompoundTypeCompareNSEQ = 16, // NULL SAFE EQUAL
        CompoundTypeCompareGE   = 17,
        CompoundTypeCompareGT   = 18,
        CompoundTypeCompareLE   = 19,
        ComponudTypeCompareLT   = 20,
        CompoundTypeCompareNOEQ = 21,
    };

    enum inner_type {
        InnerTypeUnaryOp       = 0,
        InnerTypeBinaryOp       = 1
    };
public:
    explicit Ast_ArithmeticExpr(arithmetic_type arithmetic_type, Ast_Expr *expr);
    explicit Ast_ArithmeticExpr(arithmetic_type arithmetic_type, Ast_Expr *lhs, Ast_Expr *rhs);
    virtual ~Ast_ArithmeticExpr();
    
public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    const char * arithmeticTypeName(arithmetic_type arithmetic_type);
    const char * arithmeticTypeStr(arithmetic_type arithmetic_type);

private:
    arithmetic_type arithmetic_type;
    inner_type inner_type;
    union {
        Ast_Expr *unary_op;
        struct { Ast_Expr *lhs; Ast_Expr *rhs; };
    };
};

class Ast_CompareExpr : public Ast_Expr {
public:
    enum compare_type {
        CompoundTypeCompareEQ   = 0,
        CompoundTypeCompareNSEQ = 1, // NULL SAFT EQUAL
        CompoundTypeCompareGE   = 2,
        CompoundTypeCompareGT   = 3,
        CompoundTypeCompareLE   = 4,
        ComponudTypeCompareLT   = 5,
        CompoundTypeCompareNOEQ = 6,
    };

    enum compare_subtype {
        CompareSubTypeNone  = 0,
        CompareSubTypeAny   = 1,
        CompareSubTypeSome  = 2,
        CompareSubTypeAll   = 3
    };

public:
    explicit Ast_CompareExpr(compare_type compare_type, Ast_Expr *lhs, Ast_SelectStmt *rhs);
    explicit Ast_CompareExpr(compare_type compare_type, compare_subtype compare_subtype, 
        Ast_Expr *lhs, Ast_SelectStmt *rhs);
    virtual ~Ast_CompareExpr();
    
public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    const char * compareTypeName(compare_type compare_type);
    const char * compareTypeStr(compare_type compare_type);
    const char * compareSubTypeName(compare_subtype compare_subtype);

private:
    compare_type compare_type;
    compare_subtype compare_subtype;
    Ast_Expr *lhs;
    Ast_SelectStmt *rhs;
};

class Ast_IsExpr : public Ast_Expr {
public:
    enum is_type {
        CompoundTypeIsNull      = 22,
        CompoundTypeIsNotNull   = 23,
        CompoundTypeIsBool      = 24,
        CompoundTypeIsNotBool   = 25,
    };

public:
    explicit Ast_IsExpr(is_type is_type, Ast_Expr *lhs);
    explicit Ast_IsExpr(is_type is_type, Ast_Expr *lhs, bool bool_var);
    virtual ~Ast_IsExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    is_type is_type;
    union {
        Ast_Expr *is_null;
        struct {
            Ast_Expr *lhs;
            bool bool_var;
        };
    };
};

class Ast_AsgnExpr : public Ast_Expr {
public:
    explicit Ast_AsgnExpr(const char *name, Ast_Expr *expr);
    virtual ~Ast_AsgnExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    std::string name;
    Ast_Expr *expr;
};

class Ast_BetweenExpr : public Ast_Expr {
public:
    explicit Ast_BetweenExpr(Ast_Expr *expr, Ast_Expr *low, Ast_Expr *high);
    ~Ast_BetweenExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    Ast_Expr *expr;
    Ast_Expr *low;
    Ast_Expr *high;
};

class Ast_InExpr : public Ast_Expr {
public:
    enum in_type {
        CompoundTypeIn          = 28,
        CompoundTypeNotIn       = 29
    };

    enum inner_type {
        InnerTypeOptWithSelect  = 0,
        InnerTypeOptWithValList = 1
    };

public:
    explicit Ast_InExpr(in_type in_type, Ast_Expr *expr, Ast_ValList *val_list);
    explicit Ast_InExpr(in_type in_type, Ast_Expr *expr, Ast_SelectStmt *select);
    virtual ~Ast_InExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    in_type in_type;
    inner_type inner_type;
    Ast_Expr *expr;
    union {
        Ast_ValList *val_list;
        Ast_SelectStmt *select;
    };
};

class Ast_ExistExpr : public Ast_Expr {
public:
    enum exist_type {
        CompoundTypeExists      = 0,
        CompoundTypeNotExists   = 1,
    };

public:
    explicit Ast_ExistExpr(exist_type exist_type, Ast_SelectStmt *stmt);
    virtual ~Ast_ExistExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    exist_type exist_type;
    Ast_SelectStmt *stmt;
};


class Ast_RegularFunctionExpr : public Ast_Expr {
public:
    explicit Ast_RegularFunctionExpr(const char *func_name, Ast_ValList *params);
    virtual ~Ast_RegularFunctionExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    std::string func_name;
    Ast_ValList *params;
};

class Ast_CountFuncExpr : public Ast_Expr {
public:
    explicit Ast_CountFuncExpr(Ast_Expr *expr);
    virtual ~Ast_CountFuncExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    Ast_Expr *expr;
};

class Ast_SubtringFuncExpr : public Ast_Expr {
public:
    enum inner_type {
        InnerTypeOptWithExpr  = 0,
        InnerTypeOptWithValList = 1
    };
public:
    explicit Ast_SubtringFuncExpr(Ast_ValList *val_list);
    explicit Ast_SubtringFuncExpr(Ast_Expr *str, Ast_Expr *begin, Ast_Expr *length);
    virtual ~Ast_SubtringFuncExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    inner_type inner_type;
    union {
        Ast_ValList *val_list;
        struct {
            Ast_Expr *str;
            Ast_Expr *begin;
            Ast_Expr *length;
        };
    };
};

class Ast_TrimFuncExpr : public Ast_Expr {
public:
    enum trim_ltb {
        TRIM_NULL       = 0,
        TRIM_LEADING    = 1,
        TRIM_TRAILING   = 2,
        TRIM_BOTH       = 3,
    };

public:
    explicit Ast_TrimFuncExpr(Ast_ValList *val_list);
    explicit Ast_TrimFuncExpr(trim_ltb trim_ltb, Ast_Expr *pattern, Ast_ValList *val_list);
    virtual ~Ast_TrimFuncExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    const char * trimLtbName(trim_ltb trim_ltb);

private:
    Ast_ValList *val_list;
    struct {
        trim_ltb _trim_ltb;
        Ast_Expr *pattern;
    };
};

class Ast_DateFuncExpr : public Ast_Expr {
public:
    enum date_func_type {
        CompoundTypeFuncDateAdd = 36,
        CompoundTypeFuncDateSub = 37,
    };

public:
    explicit Ast_DateFuncExpr(date_func_type date_func_type, Ast_Expr *expr, Ast_IntervalExp *interval_exp);
    virtual ~Ast_DateFuncExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    date_func_type date_func_type;
    Ast_Expr *expr;
    Ast_IntervalExp *interval_exp;
};

class Ast_CaseExpr : public Ast_Expr {
public:
    explicit Ast_CaseExpr(Ast_Expr *expr, Ast_CaseList *case_list, Ast_Expr *_else);
    virtual ~Ast_CaseExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    Ast_Expr *expr;
    Ast_CaseList *case_list;
    Ast_Expr *_else;
};

class Ast_LikeExpr : public Ast_Expr {
public:
    enum like_type {
        CompoundTypeLike        = 39,
        CompoundTypeNotLike     = 40,
    };

public:
    explicit Ast_LikeExpr(like_type like_type, Ast_Expr *lhs, Ast_Expr *rhs);
    virtual ~Ast_LikeExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    like_type like_type;
    Ast_Expr *lhs;
    Ast_Expr *rhs;
};

class Ast_RegexpExpr : public Ast_Expr {
public:
    enum regexp_type {
        CompoundTypeRegexp      = 41,
        CompoundTypeNotRegexp   = 42,
    };

public:
    explicit Ast_RegexpExpr(regexp_type regexp_type, Ast_Expr *lhs, Ast_Expr *rhs);
    virtual ~Ast_RegexpExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:    
    regexp_type regexp_type;
    Ast_Expr *lhs;
    Ast_Expr *rhs;
};

class Ast_BinaryExpr : public Ast_Expr {
public:
    explicit Ast_BinaryExpr(Ast_Expr *expr);
    virtual ~Ast_BinaryExpr();

public:
    virtual Ast_Expr eval() const;
    virtual std::string format();

private:
    Ast_Expr *expr;
};

#endif
