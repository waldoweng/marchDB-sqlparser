#include "ast.h"
#include "pmysql.hh"
#include "pmysql.tab.hh"


int sqlparse(const char *str) {
    yy_scan_string(str);
    if (yyparse()) {
        printf("SQL parse failed for sql [%s]", str);
        return -1;
    }
    return 0;
}

int main() {
    // sqlparse("select * from abc;");
    sqlparse("select all distinct distinctrow high_priority straight_join sql_small_result sql_big_result sql_calc_found_rows (1+1) as abc;");
    // sqlparse("select * from abc where a = 1 and b = 'c';");
    return 0;
}