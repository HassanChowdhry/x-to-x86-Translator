#include <stdio.h>
#include "xtra.h"

void prologue() {
    printf(".globl test\n"
           "test:\n"
           "\tpush %%rbp\n"
           "\tmov %%rsp, %%rbp\n");
}

void epilogue() {
    printf("\tpop %%rbp\n"
           "\tret\n"
            );
}

int main(int argc, char **argv) {
    prologue();

    // Get the file and run translator method
    char * file_name = argv[argc-1];
    translator(file_name);

    epilogue();
    return 0;
}