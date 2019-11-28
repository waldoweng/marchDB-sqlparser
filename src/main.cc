#include "ast.h"
#include "pmysql.hh"
#include "pmysql.tab.hh"


int main(int argc, char *argv[]) {
    extern FILE *yyin;

    if( argc > 1 && !strcmp(argv[1], "-d")) {
        //yydebug = 1;
        argc --;
        argv ++;
    }

    if ( argc > 1 && (yyin = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        exit(1);
    }

    if (!yyparse())
        printf("\n");
    else
        printf("\nSQL parse failed\n");
}