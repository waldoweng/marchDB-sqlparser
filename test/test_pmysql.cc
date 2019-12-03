#include <stdio.h>

#include "ast.h"
#include "pmysql.hh"
#include "pmysql.tab.hh"

int sqlparse(const char *str) {
    YY_BUFFER_STATE bp;

    bp = yy_scan_string(str);
    if (yyparse()) {
        yy_delete_buffer(bp);
        printf("SQL parse failed for sql [%s]", str);
        return -1;
    }

    yy_delete_buffer(bp);
    return 0;
}

void print_help() {
    printf("test_pmysql <sql>\n");
}

/* About memory leak 
 * (https://stackoverflow.com/questions/43671389/minimal-bison-flex-generated-code-has-memory-leak)
 * 
 * The default flex skeleton allocates an input buffer and a small buffer stack, which it never frees. 
 * You could free the input buffer manually with yy_delete_buffer(YY_CURRENT_BUFFER); but there is no 
 * way to delete the buffer stack. (It's only 8 bytes in your application, so it's not a disaster.)
 *
 * If you want to write a clean application, you should generate a reentrant scanner, which puts all 
 * persistent data into a scanner context object. Your code must allocate and free this object, and 
 * freeing it will free all memory allocations. (You might also want to generate a pure parser, which 
 * works roughly the same way.)
 *
 * However, the reentrant scanner has a very different API, so you will need to get your parser to pass
 * through the scanner context object. If you use a reentrant (pure) parser as well, you'll need to 
 * modify your scanner actions because with the reentrant parser, yylval is a YYSTYPE* instead of YYSTYPE.
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_help();
        return 0;
    }
    if (sqlparse(argv[1])) {
        return -1;
    }

    return 0;
}
