#include <time.h>
#include "ast_expr.h"
#include "ast_select_stmt.h"

Ast_CaseList::Ast_CaseList(Ast_Expr *when, Ast_Expr *then) {
    case_item item;
    item.when_expr = when;
    item.then_expr = then;
    cases.push_back(item);
}

Ast_CaseList::~Ast_CaseList() {
    for (std::vector<Ast_CaseList::case_item>::iterator it = cases.begin(); 
        it != cases.end();
        it ++) 
    {
        if (it->when_expr)
            delete it->when_expr;
        if (it->then_expr)
            delete it->then_expr;
    }
}

void Ast_CaseList::addCase(Ast_Expr *when, Ast_Expr *then) {
    case_item item;
    item.when_expr = when;
    item.then_expr = then;
    cases.push_back(item);
}

std::string Ast_CaseList::format() {
    std::string str;

    for (std::vector<Ast_CaseList::case_item>::iterator it = cases.begin();
        it != cases.end();
        it ++) 
    {
        str += this->rawf("WHEN %s THEN %s\n",
            it->when_expr->format().c_str(),
            it->then_expr->format().c_str()
        );
    }
    
    return str;
}


Ast_IntervalExp::Ast_IntervalExp(enum Ast_IntervalExp::interval_type interval_type, Ast_Expr *expr)
    : interval_type(interval_type), expr(expr)
{
}

Ast_IntervalExp::~Ast_IntervalExp() {
    if (expr) delete expr;
}

const char * Ast_IntervalExp::intervalTypeName(enum Ast_IntervalExp::interval_type interval_type) {
    static const char names[9][32] = {
        "DAY HOUR",
        "DAY MICROSECOND",
        "DAY MINUTE",
        "DAY SECOND",
        "YEAR MONTH",
        "YEAR",
        "HOUR MICROSECOND",
        "HOUR MINUTE",
        "HOUR SECOND"
    };

    return names[interval_type-1];
}

std::string Ast_IntervalExp::format() {
    return this->rawf("INTERVAL %s %s",
        this->expr->format().c_str(),
        this->intervalTypeName(this->interval_type)
    );
}


Ast_ValList::Ast_ValList(Ast_Expr *expr) {
    this->exprs.push_back(expr);
}

Ast_ValList::~Ast_ValList() {
    for (std::vector<Ast_Expr *>::iterator it = this->exprs.begin();
        it != this->exprs.end();
        ++ it)
    {
        if (*it) delete (*it);
    }
}

void Ast_ValList::addExpr(Ast_Expr *expr) {
    this->exprs.push_back(expr);
}

std::string Ast_ValList::format() {
    std::string str;

    if (!this->exprs.empty()) {
        str = this->exprs.back()->format();

        for (std::vector<Ast_Expr *>::reverse_iterator it = this->exprs.rbegin() + 1;
            it != this->exprs.rend();
            ++ it)
        {
            if (*it) 
                str += (", " + (*it)->format());
        }
    }

    return str;
}


Ast_Expr::Ast_Expr() {}

Ast_Expr::~Ast_Expr() {}

std::string Ast_Expr::format() { return ""; }

Ast_Expr Ast_Expr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_LiteralExpr::Ast_LiteralExpr(enum Ast_LiteralExpr::literal_type literal_type)
    : literal_type(literal_type)
{
}

Ast_LiteralExpr::Ast_LiteralExpr(enum Ast_LiteralExpr::literal_type literal_type, 
    const char *first, const char *second)
    : literal_type(literal_type)
{
    this->name.first = first ? first : "";
    this->name.second = second;
}

Ast_LiteralExpr::Ast_LiteralExpr(int int_var)
    : literal_type(Ast_LiteralExpr::LiteralTypeIntNum), int_var(int_var)
{
}

Ast_LiteralExpr::Ast_LiteralExpr(double float_var)
    : literal_type(Ast_LiteralExpr::LiteralTypeApproxNum), float_var(float_var)
{
}

Ast_LiteralExpr::Ast_LiteralExpr(bool bool_var)
    : literal_type(Ast_LiteralExpr::LiteralTypeBool), bool_var(bool_var)
{
}

Ast_LiteralExpr::~Ast_LiteralExpr() {}

std::string Ast_LiteralExpr::format() {
    switch (this->literal_type)
    {
    case Ast_LiteralExpr::LiteralTypeName:
    case Ast_LiteralExpr::LiteralTypeDetailName:
        if (this->name.first.empty())
            return this->rawf("%s.%s", this->name.first.c_str(), this->name.second.c_str());
        else
            return this->name.second;
    case Ast_LiteralExpr::LiteralTypeString:
        return this->rawf("%s", this->name.second.c_str());
    case Ast_LiteralExpr::LiteralTypeUserVar:
        return this->rawf("@%s", this->name.second.c_str());
    case Ast_LiteralExpr::LiteralTypeIntNum:
        return this->rawf("%d", this->int_var);
    case Ast_LiteralExpr::LiteralTypeApproxNum:
        return this->rawf("%lf", this->float_var);
    case Ast_LiteralExpr::LiteralTypeBool:
        return this->bool_var ? "true" : "false";
    case Ast_LiteralExpr::LiteralTypeCurTs:
        return "CURRENT_TIMESTAMP";
    case Ast_LiteralExpr::LiteralTypeCurDate:
        return "CURRENT_DATE";
    case Ast_LiteralExpr::LiteralTypeCurTime:
        return "CURRENT_TIME";
    default:
        return "";
    }
}

Ast_Expr Ast_LiteralExpr::eval() const {
    return Ast_LiteralExpr(0);
}

Ast_ArithmeticExpr::Ast_ArithmeticExpr(enum Ast_ArithmeticExpr::arithmetic_type arithmetic_type, Ast_Expr *expr)
    : arithmetic_type(arithmetic_type), inner_type(InnerTypeUnaryOp), unary_op(expr)
{
}

Ast_ArithmeticExpr::Ast_ArithmeticExpr(enum Ast_ArithmeticExpr::arithmetic_type arithmetic_type, 
    Ast_Expr *lhs, Ast_Expr *rhs)
    : arithmetic_type(arithmetic_type), inner_type(Ast_ArithmeticExpr::InnerTypeBinaryOp),
        lhs(lhs), rhs(rhs)
{
}

Ast_ArithmeticExpr::~Ast_ArithmeticExpr() {
    if (this->inner_type == Ast_ArithmeticExpr::InnerTypeUnaryOp)
        delete this->unary_op;
    else {
        delete this->lhs;
        delete this->rhs;
    }
}

const char * Ast_ArithmeticExpr::arithmeticTypeName(enum Ast_ArithmeticExpr::arithmetic_type arithmetic_type) {
    static const char names[22][32] = {
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "MOD",
        "NEG",
        "AND",
        "OR",
        "XOR",
        "BITOR",
        "BITAND",
        "BITXOR",
        "LEFT SHIFT",
        "RIGHT SHIFT",
        "NOT",
        "COMPARE EQUAL",
        "COMPARE NULL SAFE EQUAL",
        "COMPARE GREATER EQUAL",
        "COMPARE GREATER THAN",
        "COMPARE LESSER EQUAL",
        "COMPARE LESSER THAN",
        "COMPARE NOT EQUAL"
    };
    return names[arithmetic_type];
}

const char * Ast_ArithmeticExpr::arithmeticTypeStr(enum Ast_ArithmeticExpr::arithmetic_type arithmetic_type) {
    static const char names[22][32] = {
        "+",
        "-",
        "*",
        "/",
        "%",
        "-",
        "and",
        "or",
        "xor",
        "|",
        "&",
        "^",
        "<<",
        ">>",
        "NOT",
        "=",
        "<=>",
        ">=",
        ">",
        "<=",
        "<",
        "!="
    };
    return names[arithmetic_type];
}

std::string Ast_ArithmeticExpr::format() {
    if (this->inner_type == Ast_ArithmeticExpr::InnerTypeUnaryOp) {
        return this->rawf("%s %s",
            this->arithmeticTypeStr(this->arithmetic_type),
            this->unary_op->format().c_str()
        );
    } else {
        return this->rawf("%s %s %s",
            this->lhs->format().c_str(),
            this->arithmeticTypeStr(this->arithmetic_type),
            this->rhs->format().c_str()
        );
    }
}

Ast_Expr Ast_ArithmeticExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_CompareExpr::Ast_CompareExpr(enum Ast_CompareExpr::compare_type compare_type, 
    Ast_Expr *lhs, Ast_SelectStmt *rhs)
    : compare_type(compare_type), compare_subtype(Ast_CompareExpr::CompareSubTypeNone),
        lhs(lhs), rhs(rhs)
{
}

Ast_CompareExpr::Ast_CompareExpr(enum Ast_CompareExpr::compare_type compare_type, 
    enum Ast_CompareExpr::compare_subtype compare_subtype, Ast_Expr *lhs, Ast_SelectStmt *rhs)
    : compare_type(compare_type), compare_subtype(compare_subtype), lhs(lhs), rhs(rhs)
{
}

Ast_CompareExpr::~Ast_CompareExpr() {
    if (lhs) delete lhs;
    if (rhs) delete rhs;
}

const char * Ast_CompareExpr::compareTypeName(enum Ast_CompareExpr::compare_type compare_type) {
    static const char names[7][32] = {
        "COMPARE EQUAL",
        "COMPARE NULL SAFE EQUAL",
        "COMPARE GREATER EQUAL",
        "COMPARE GREATER THAN",
        "COMPARE LESSER EQUAL",
        "COMPARE LESSER THAN",
        "COMPARE NOT EQUAL"
    };
    return names[compare_type];
}

const char * Ast_CompareExpr::compareTypeStr(enum Ast_CompareExpr::compare_type compare_type) {
    static const char names[7][32] = {
        "=",
        "<=>",
        ">=",
        ">",
        "<=",
        "<",
        "!="
    };
    return names[compare_type];
}

const char * Ast_CompareExpr::compareSubTypeName(enum Ast_CompareExpr::compare_subtype compare_subtype) {
    static const char names[4][32] = {
        "",
        "ANY",
        "SOME",
        "ALL",
    };
    return names[compare_subtype];
}

std::string Ast_CompareExpr::format() {
    return this->rawf("%s %s %s %s",
        this->lhs->format().c_str(),
        this->compareTypeStr(this->compare_type),
        this->compareSubTypeName(this->compare_subtype),
        this->rhs->format().c_str()
    );
}

Ast_Expr Ast_CompareExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_IsExpr::Ast_IsExpr(enum Ast_IsExpr::is_type is_type, Ast_Expr *lhs)
    : is_type(is_type), is_null(lhs)
{
}

Ast_IsExpr::Ast_IsExpr(enum Ast_IsExpr::is_type is_type, Ast_Expr *lhs, bool bool_var)
    : is_type(is_type), lhs(lhs), bool_var(bool_var)
{
}

Ast_IsExpr::~Ast_IsExpr() {
    switch (this->is_type)
    {
    case Ast_IsExpr::CompoundTypeIsNull:
    case Ast_IsExpr::CompoundTypeIsNotNull:
        delete this->is_null;
        break;
    case Ast_IsExpr::CompoundTypeIsBool:
    case Ast_IsExpr::CompoundTypeIsNotBool:
        delete this->lhs;
        break;
    default:
        break;
    }
}

std::string Ast_IsExpr::format() {
    switch (this->is_type)
    {
    case Ast_IsExpr::CompoundTypeIsNull:
        return this->rawf("%s IS NULL", this->is_null->format().c_str());
    case Ast_IsExpr::CompoundTypeIsNotNull:
        return this->rawf("%s IS NOT NULL", this->is_null->format().c_str());
    case Ast_IsExpr::CompoundTypeIsBool:
        return this->rawf("%s IS %s",
            this->lhs->format().c_str(), 
            this->bool_var ? "TRUE" : "FALSE"
        );
    case Ast_IsExpr::CompoundTypeIsNotBool:
        return this->rawf("%s IS NOT %s",
            this->lhs->format().c_str(), 
            this->bool_var ? "TRUE" : "FALSE"
        );
    default:
        return "";
    }
}

Ast_Expr Ast_IsExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_AsgnExpr::Ast_AsgnExpr(const char *name, Ast_Expr *expr)
    : name(name), expr(expr)
{
}

Ast_AsgnExpr::~Ast_AsgnExpr() {
    if (expr) delete expr;
}

std::string Ast_AsgnExpr::format() {
    return this->rawf("%s = %s", 
        this->name.c_str(),
        this->expr->format().c_str()
    );
}

Ast_Expr Ast_AsgnExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_BetweenExpr::Ast_BetweenExpr(Ast_Expr *expr, Ast_Expr *low, Ast_Expr *high) 
    : expr(expr), low(low), high(high)
{
}
Ast_BetweenExpr::~Ast_BetweenExpr() {
    if (expr) delete expr;
    if (low) delete low;
    if (high) delete high;
}

std::string Ast_BetweenExpr::format() {
    return this->rawf("%s BETWEEN %s AND %s",
        this->expr->format().c_str(),
        this->low->format().c_str(),
        this->high->format().c_str()
    );
}

Ast_Expr Ast_BetweenExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_InExpr::Ast_InExpr(enum Ast_InExpr::in_type in_type, Ast_Expr *expr, Ast_ValList *val_list)
    : in_type(in_type), inner_type(Ast_InExpr::InnerTypeOptWithValList), 
        expr(expr), val_list(val_list)
{
}

Ast_InExpr::Ast_InExpr(enum Ast_InExpr::in_type in_type, Ast_Expr *expr, Ast_SelectStmt *select)
    : in_type(in_type), inner_type(Ast_InExpr::InnerTypeOptWithSelect),
        expr(expr), select(select)
{
}

Ast_InExpr::~Ast_InExpr() {
    if (expr) delete expr;
    switch (this->inner_type)
    {
    case Ast_InExpr::InnerTypeOptWithValList:
        delete this->val_list;
        break;
    case Ast_InExpr::InnerTypeOptWithSelect:
        delete this->select;
        break;
    default:
        break;
    }
}

std::string Ast_InExpr::format() {

    std::string pattern = "";
    
    switch (this->in_type)
    {
    case Ast_InExpr::CompoundTypeIn:
        pattern = "%s IN %s";
        break;
    case Ast_InExpr::CompoundTypeNotIn:
        pattern = "%s NOT IN %s";
        break;
    default:
        return "";
    }
    
    switch (this->inner_type)
    {
    case Ast_InExpr::InnerTypeOptWithValList:
        return this->rawf(pattern.c_str(), 
            this->expr->format().c_str(),
            this->val_list->format().c_str());
    case Ast_InExpr::InnerTypeOptWithSelect:
        return this->rawf(pattern.c_str(), 
            this->expr->format().c_str(),
            this->select->format().c_str());
    }

    return "";
}

Ast_Expr Ast_InExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_ExistExpr::Ast_ExistExpr(enum Ast_ExistExpr::exist_type exist_type, Ast_SelectStmt *stmt)
    : exist_type(exist_type), stmt(stmt)
{
}

Ast_ExistExpr::~Ast_ExistExpr() {
    if (stmt) delete stmt;
}

std::string Ast_ExistExpr::format() {
    return this->rawf("%s (%s)",
        this->exist_type ? "EXISTS" : "NOT EXISTS",
        this->stmt->format().c_str()
    );
}

Ast_Expr Ast_ExistExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_RegularFunctionExpr::Ast_RegularFunctionExpr(const char *func_name, Ast_ValList *params)
    : func_name(func_name), params(params)
{
}

Ast_RegularFunctionExpr::~Ast_RegularFunctionExpr() {
    if (params) delete params;
}

std::string Ast_RegularFunctionExpr::format() {
    return this->rawf("%s(%s)", 
        this->func_name.c_str(),
        this->params->format().c_str()
    );
}

Ast_Expr Ast_RegularFunctionExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_CountFuncExpr::Ast_CountFuncExpr(Ast_Expr *expr)
    : expr(expr)
{
}

Ast_CountFuncExpr::~Ast_CountFuncExpr() {
    if (expr) delete expr;
}

std::string Ast_CountFuncExpr::format() {
    if (expr) {
        return this->rawf("COUNT(%s)", this->expr->format().c_str());
    } else {
        return "COUNT(*)";
    }
}

Ast_Expr Ast_CountFuncExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_SubtringFuncExpr::Ast_SubtringFuncExpr(Ast_ValList *val_list)
    : inner_type(Ast_SubtringFuncExpr::InnerTypeOptWithValList), val_list(val_list)
{
}

Ast_SubtringFuncExpr::Ast_SubtringFuncExpr(Ast_Expr *str, Ast_Expr *begin, Ast_Expr *length)
    : inner_type(Ast_SubtringFuncExpr::InnerTypeOptWithExpr),
        str(str), begin(begin), length(length)
{
}

Ast_SubtringFuncExpr::~Ast_SubtringFuncExpr() {
    switch (this->inner_type)
    {
    case Ast_SubtringFuncExpr::InnerTypeOptWithExpr:
        if (this->str) delete this->str;
        if (this->begin) delete this->begin;
        if (this->length) delete this->length;
        break;
    case Ast_SubtringFuncExpr::InnerTypeOptWithValList:
        if (this->val_list) delete this->val_list;
        break;
    default:
        break;
    }
}

std::string Ast_SubtringFuncExpr::format() {
    switch (this->inner_type)
    {
    case Ast_SubtringFuncExpr::InnerTypeOptWithExpr:
        return this->rawf("SUBSTRING(%s FROM %s %s)",
            this->str ? this->str->format().c_str() : "",
            this->begin ? this->begin->format().c_str() : "",
            this->length ? (" FOR " + this->length->format()).c_str() : "" 
        );
    case Ast_SubtringFuncExpr::InnerTypeOptWithValList:
        return this->rawf("SUBSTRING(%s)",
            this->val_list ? this->val_list->format().c_str() : ""
        );
    default:
        break;
    }

    return "";
}

Ast_Expr Ast_SubtringFuncExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_TrimFuncExpr::Ast_TrimFuncExpr(Ast_ValList *val_list)
    : val_list(val_list), pattern(nullptr)
{
}

Ast_TrimFuncExpr::Ast_TrimFuncExpr(enum Ast_TrimFuncExpr::trim_ltb trim_ltb,
    Ast_Expr *pattern, Ast_ValList *val_list)
    : val_list(val_list), _trim_ltb(trim_ltb), pattern(pattern) 
{
}

Ast_TrimFuncExpr::~Ast_TrimFuncExpr() {
    if (this->val_list) delete this->val_list;
    if (this->pattern) delete this->pattern;
}

const char * Ast_TrimFuncExpr::trimLtbName(enum Ast_TrimFuncExpr::trim_ltb trim_ltb) {
    static const char names[3][32] = {
        "LEADING",
        "TRAILING",
        "BOTH"
    };
    return names[trim_ltb-1];
}

std::string Ast_TrimFuncExpr::format() {
    if (this->pattern) {
        return this->rawf("TRIM(%s %s FROM %s)", 
            this->trimLtbName(this->_trim_ltb),
            this->pattern->format().c_str(),
            this->val_list->format().c_str()
        );
    } else {
        return this->rawf("TRIM(%s)", this->val_list->format().c_str());
    }
}

Ast_Expr Ast_TrimFuncExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_DateFuncExpr::Ast_DateFuncExpr(enum Ast_DateFuncExpr::date_func_type date_func_type, 
    Ast_Expr *expr, Ast_IntervalExp *interval_exp)
    : date_func_type(date_func_type), expr(expr), interval_exp(interval_exp)
{
}

Ast_DateFuncExpr::~Ast_DateFuncExpr() {
    if (expr) delete expr;
    if (interval_exp) delete interval_exp;
}

std::string Ast_DateFuncExpr::format() {
    switch (this->date_func_type)
    {
    case Ast_DateFuncExpr::CompoundTypeFuncDateAdd:
        return this->rawf("DATE_ADD(%s, %s)", 
            this->expr->format().c_str(), 
            this->interval_exp->format().c_str()
        );
    case Ast_DateFuncExpr::CompoundTypeFuncDateSub:
        return this->rawf("DATE_SUB(%s, %s)", 
            this->expr->format().c_str(), 
            this->interval_exp->format().c_str()
        );
    default:
        return "";
    }
}

Ast_Expr Ast_DateFuncExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_CaseExpr::Ast_CaseExpr(Ast_Expr *expr, Ast_CaseList *case_list, Ast_Expr *_else)
    : expr(expr), case_list(case_list), _else(_else)
{
}

Ast_CaseExpr::~Ast_CaseExpr() {
    if (this->expr) delete this->expr;
    if (this->case_list) delete this->case_list;
    if (this->_else) delete this->_else;
}

std::string Ast_CaseExpr::format() {
    return this->rawf("CASE %s %s %s END",
        this->expr ? this->expr->format().c_str() : "",
        this->case_list ? this->case_list->format().c_str() : "",
        this->_else ? ("ELSE " + this->_else->format()).c_str() : ""
    );
}

Ast_Expr Ast_CaseExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_LikeExpr::Ast_LikeExpr(enum Ast_LikeExpr::like_type like_type, Ast_Expr *lhs, Ast_Expr *rhs)
    : like_type(like_type), lhs(lhs), rhs(rhs)
{
}

Ast_LikeExpr::~Ast_LikeExpr() {
    if (this->lhs) delete this->lhs;
    if (this->rhs) delete this->rhs; 
}

std::string Ast_LikeExpr::format() {
    switch (this->like_type)
    {
    case Ast_LikeExpr::CompoundTypeLike:
        return this->rawf("%s LIKE %s", this->lhs->format().c_str(), this->rhs->format().c_str());
    case Ast_LikeExpr::CompoundTypeNotLike:
        return this->rawf("%s NOT LIKE %s", this->lhs->format().c_str(), this->rhs->format().c_str());
    default:
        return "";
    }
}

Ast_Expr Ast_LikeExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_RegexpExpr::Ast_RegexpExpr(enum Ast_RegexpExpr::regexp_type regexp_type, Ast_Expr *lhs, Ast_Expr *rhs)
    : regexp_type(regexp_type), lhs(lhs), rhs(rhs)
{
}

Ast_RegexpExpr::~Ast_RegexpExpr() {
    if (this->lhs) delete this->lhs;
    if (this->rhs) delete this->rhs; 
}

std::string Ast_RegexpExpr::format() {
    switch (this->regexp_type)
    {
    case Ast_RegexpExpr::CompoundTypeRegexp:
        return this->rawf("%s REGEXP %s", this->lhs->format().c_str(), this->rhs->format().c_str());
    case Ast_RegexpExpr::CompoundTypeNotRegexp:
        return this->rawf("%s NOT REGEXP %s", this->lhs->format().c_str(), this->rhs->format().c_str());
    default:
        return "";
    }
}

Ast_Expr Ast_RegexpExpr::eval() const {
    return Ast_LiteralExpr(0);
}


Ast_BinaryExpr::Ast_BinaryExpr(Ast_Expr *expr)
    : expr(expr)
{
}

Ast_BinaryExpr::~Ast_BinaryExpr() {
    if (this->expr) delete this->expr;
}

std::string Ast_BinaryExpr::format() {
    return this->rawf("BINARY %s", this->expr->format().c_str());
}

Ast_Expr Ast_BinaryExpr::eval() const {
    return Ast_LiteralExpr(0);
}
