#include <stdio.h>
char *reg_map64[] = {
        "%rax",
        "%rbx",
        "%rcx",
        "%rdx",
        "%rsi",
        "%rdi",
        "%r8 ",
        "%r9 ",
        "%r10",
        "%r11",
        "%r12",
        "%r13",
        "%r14",
        "%r15",  // flags
        "%rbp",  // r14  frame pointer
        "%rsp",  // r15  stack pointer
        "%rip"
};

extern void runit();
int main() {
    runit();
    return 0;
}

void do_outchar(char c) {
    putchar(c);
}

void outregs(unsigned long *regs) {
    printf("Registers:\n  ");
    for (int i = 0; i < 14; i++) {
        if (i && !(i % 2)) {
            printf("\n  ");
        }

        printf("%s: 0x%16.16lx    ", reg_map64[i], regs[i]);
    }
    printf("\n");
}

char * get_hello() {
    return "Hello World!";
}
