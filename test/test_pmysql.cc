#include <stdio.h>

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

void print_help() {
    printf("test_pmysql <sql>\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_help();
        return -1;
    }

    sqlparse(argv[1]);
    return 0;
}
