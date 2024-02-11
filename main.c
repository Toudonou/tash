#include "tash.h"

/*
 *  1 - make a REPL (Read Eval Print Loop) to get user command line and to give the prompt
 *      to the user once the command is executed
 *  2 - split the command according space
 *  3 - executed the result by using execvp in other thead
 * */


int main() {

    tash_loop();

    return 0;
}
