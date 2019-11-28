#include "ast_table_reference.h"
#include "ast_expr.h"
#include "ast_select_stmt.h"
#include "ast_column_list.h"

Ast_IndexList::Ast_IndexList(const char *name) {
    names.push_back(name);
}

Ast_IndexList::~Ast_IndexList() {}

std::string Ast_IndexList::format() {
    std::string tmp;

    for (std::vector<std::string>::iterator it = names.begin();
        it != names.end();
        it ++)
    {
        tmp += (*it + ", ");
    }

    return tmp;
}

void Ast_IndexList::addName(const char *name) {
    names.push_back(name);
}

Ast_IndexHint::Ast_IndexHint(bool use, bool for_join, Ast_IndexList *index_list) 
    : use(use), for_join(for_join), index_list(index_list)
{
}

Ast_IndexHint::~Ast_IndexHint() {
    if (index_list) delete index_list;
}

std::string Ast_IndexHint::format() {
    return this->rawf("%s KEY %s (%s)", 
        this->use ? " USE " : "IGNORE",
        this->for_join ? " FOR JOIN " : "",
        this->index_list->format().c_str()
    );
}

Ast_JoinCondition::Ast_JoinCondition(Ast_Expr *expr) 
    : expr(expr), column_list(NULL)
{
}

Ast_JoinCondition::Ast_JoinCondition(Ast_ColumnList *column_list)
    : expr(NULL), column_list(column_list)
{
}

Ast_JoinCondition::~Ast_JoinCondition() {
    if (expr) delete expr;
    if (column_list) delete column_list;
}

std::string Ast_JoinCondition::format() {
    if (expr) 
        return this->rawf("ON %s", this->expr->format().c_str());
    if (column_list) 
        return this->rawf("USING (%s)", this->column_list->format().c_str());
    return "";
}


Ast_TableSubquery::Ast_TableSubquery(Ast_SelectStmt *select_stmt)
    : select_stmt(select_stmt)
{
}

Ast_TableSubquery::~Ast_TableSubquery()
{
    if (select_stmt) delete select_stmt;
}

std::string Ast_TableSubquery::format() {
    return this->rawf("(%s)", this->select_stmt->format().c_str());
}

Ast_TableFactor::TableFactorNormal::TableFactorNormal(const char *database_name, 
    const char *name, const char *alias, Ast_IndexHint *index_hint)
    : database_name(database_name), name(name), alias(alias), index_hint(index_hint)
{
}

Ast_TableFactor::TableFactorNormal::~TableFactorNormal()
{
    if (this->index_hint) delete index_hint;
}

Ast_TableFactor::TableFactorSubquery::TableFactorSubquery(Ast_TableSubquery *subquery, const char *name)
    : subquery(subquery), name(name)
{
}

Ast_TableFactor::TableFactorSubquery::~TableFactorSubquery() 
{
    if (this->subquery) delete this->subquery;
}

Ast_TableFactor::TableFactorReferences::TableFactorReferences(Ast_TableReferences *references)
    : references(references)
{
}

Ast_TableFactor::TableFactorReferences::~TableFactorReferences()
{
    if (this->references) delete this->references;
}

Ast_TableFactor::Ast_TableFactor(const char *name, const char *alias, Ast_IndexHint *index_hint)
    : factor_type(Ast_TableFactor::FACTOR_TYPE_NORMAL),
        normal(new TableFactorNormal("", name, alias ? alias : "", index_hint))
{
}

Ast_TableFactor::Ast_TableFactor(const char *database_name, const char *name, 
    const char *alias, Ast_IndexHint *index_hint)
    : factor_type(Ast_TableFactor::FACTOR_TYPE_NORMAL),
        normal(new TableFactorNormal(database_name, name, alias ? alias : "", index_hint))
{
}

Ast_TableFactor::Ast_TableFactor(Ast_TableSubquery *subquery, const char *name)
    : factor_type(Ast_TableFactor::FACTOR_TYPE_SUBQUERY),
        subquery(new TableFactorSubquery(subquery, name))
{
}

Ast_TableFactor::Ast_TableFactor(Ast_TableReferences *references)
    : factor_type(Ast_TableFactor::FACTOR_TYPE_REFERENCES),
        references(new TableFactorReferences(references))
{
}

Ast_TableFactor::~Ast_TableFactor() {
    switch (this->factor_type)
    {
    case Ast_TableFactor::FACTOR_TYPE_NORMAL:
        delete this->normal;
        break;
    case Ast_TableFactor::FACTOR_TYPE_SUBQUERY:
        delete this->subquery;
        break;
    case Ast_TableFactor::FACTOR_TYPE_REFERENCES:
        delete this->references;
        break;
    default:
        break;
    }
}

std::string Ast_TableFactor::format() {
    switch (this->factor_type)
    {
    case Ast_TableFactor::FACTOR_TYPE_NORMAL:
        return this->rawf("%s %s %s", 
            normal->database_name.empty() 
                ? normal->name.c_str() 
                : (normal->database_name + '.' + normal->name).c_str(), 
            normal->alias.empty() 
                ? ""
                : ("AS " + normal->alias).c_str(),
            normal->index_hint->format().c_str()
        );
    case Ast_TableFactor::FACTOR_TYPE_SUBQUERY:
        return this->rawf("%s %s",
            this->subquery->subquery->format().c_str(),
            this->subquery->name.empty() ? "" : ("AS " + this->subquery->name).c_str()
        );
    case Ast_TableFactor::FACTOR_TYPE_REFERENCES:
        return this->rawf("(%s)", this->references->references->format().c_str());
    default:
        return "";
    }
}

Ast_JoinTable::Ast_JoinTable(Ast_TableReference *reference, enum Ast_JoinTable::join_type join_type, 
    Ast_TableFactor *table_factor, Ast_JoinCondition *join_condition)
    : reference(reference), join_type(join_type), table_factor(table_factor), 
        join_condition(join_condition)
{
}

Ast_JoinTable::Ast_JoinTable(Ast_TableReference *reference, Ast_TableFactor *table_factor)
    : reference(reference), join_type(Ast_JoinTable::JOIN_TABLE_STRAIGHT_JOIN),
        table_factor(table_factor), join_condition(nullptr)
{
}

Ast_JoinTable::Ast_JoinTable(Ast_TableReference *reference, enum Ast_JoinTable::join_type join_type, 
    Ast_TableFactor *table_factor, Ast_Expr *expr)
    : reference(reference), join_type(join_type), table_factor(table_factor), 
        join_condition(new Ast_JoinCondition(expr))
{
}

Ast_JoinTable::Ast_JoinTable(Ast_TableReference *reference, enum Ast_JoinTable::join_type join_type, 
    Ast_TableFactor *table_factor)
    : reference(reference), join_type(join_type), table_factor(table_factor), join_condition(nullptr)
{
}

Ast_JoinTable::~Ast_JoinTable() {
    if (reference) delete reference;
    if (table_factor) delete table_factor;
    if (join_condition) delete join_condition;
}

/* TODO */
const char * Ast_JoinTable::joinTypeName(enum Ast_JoinTable::join_type join_type) {
    static const char names[5][32] = {
        "INNER JOIN",
        "CROSS JOIN",
        "LEFT OUTER JOIN",
        "RIGHT OUTER JOIN",
        "STRAIGHT_JOIN"
    };
    return names[join_type];
}

std::string Ast_JoinTable::format() {
    return this->rawf("%s %s %s %s", 
        this->reference->format().c_str(),
        this->joinTypeName(this->join_type),
        this->table_factor->format().c_str(),
        this->join_condition ? this->join_condition->format().c_str() : ""
    );
}

Ast_TableReference::Ast_TableReference(Ast_TableFactor *table_factor)
    : table_factor(table_factor), join_table(nullptr)
{
}

Ast_TableReference::Ast_TableReference(Ast_JoinTable *join_table)
    : table_factor(nullptr), join_table(join_table)
{
}

Ast_TableReference::~Ast_TableReference()
{
    if (this->table_factor) delete this->table_factor;
    if (this->join_table) delete this->join_table;
}

std::string Ast_TableReference::format() {
    if (this->table_factor) 
        return this->table_factor->format();
    if (this->join_table)
        return this->join_table->format();
    return "";
}

Ast_TableReferences::Ast_TableReferences(Ast_TableReference *reference) {
    this->references.push_back(reference);
}

Ast_TableReferences::~Ast_TableReferences() {
    for (std::vector<Ast_TableReference *>::iterator it = this->references.begin();
        it != this->references.end();
        it ++)
    {
        if (*it) delete (*it);
    }
}

void Ast_TableReferences::addTableReference(Ast_TableReference *reference) {
    this->references.push_back(reference);
}

std::string Ast_TableReferences::format() {
    std::string str;

    if (!this->references.empty()) {
        str = this->references[0]->format();

        for (std::vector<Ast_TableReference *>::iterator it = this->references.begin() + 1;
            it != this->references.end();
            it ++)
        {
            if (*it) 
                str += (",\n" + (*it)->format());
        }
    }

    return str;
}
