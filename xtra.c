#include "xtra.h"
#include <string.h>

// Array of 64-bit register names
char *reg_map64[] = {
        "%rax", "%rbx", "%rcx", "%rdx",
        "%rsi", "%rdi", "%r8 ", "%r9 ",
        "%r10", "%r11", "%r12", "%r13",
        "%r14", "%r15", "%rbp", "%rsp",
        "%rip", "%al", "%cl", "%dl",
        "%bl", "%si", "%di", "%sp",
        "%bp", "%r08b", "%r09b", "%r10b",
        "%r11b", "%r12b", "%r13b",
        "%r14b", "%15b"
};

// Array of opcode zero instructions
char *opcode_zero_instructions[] = {
        "", "ret", "cld", "std"
};

// Array of opcode one instructions
char *opcode_one_instructions[] = {
        "", "neg", "not", "push", "pop",
        "", "", "out", "inc", "dec"
};

// Array of opcode one immediate instructions
char *opcode_one_immediate_instructions[] = {
        "", "br", "jr"
};

// Array of opcode two instructions
char *opcode_two_instructions[] = {
        "", "add", "sub", "imul", "", "and", "or",
        "xor", "", "", "test", "cmp", "equ", "mov",
        "load", "stor", "loadb", "storb"
};

// Array of opcode three instructions
char *opcode_three_instructions[] = {
        "", "jmp", "call"
};

void opcode_zero(unsigned short encoding);
void opcode_one(unsigned short encoding);
void opcode_two(unsigned short encoding);
void opcode_three(unsigned short encoding, unsigned short extended);
unsigned short little_to_big(unsigned short value);

static unsigned int curr_address = 0; // variable to keep track of address
static int is_in_debug = 0; // debug flag

// Translate the input file to assembly code
void translator(char * file_name) {
    FILE *f = fopen(file_name, "r");

    while (1) {
        unsigned short encoding;
        fread(&encoding, sizeof(short), 1, f);

        // print then update address by 2
        printf(".L%04x:\n", curr_address);
        curr_address+=2;

        if (is_in_debug) {
            printf("\tcall debug\n");
        }

        if (encoding == 0) {
            break;
        }

        unsigned char opcode = (encoding>>6) & 0x3;

        // Decode and execute instructions based on opcode
        if (opcode == 0) {
            opcode_zero(encoding);
        } else if (opcode == 1) {
            opcode_one(encoding);
        } else if (opcode == 2) {
            opcode_two(encoding);
        } else if (opcode == 3) {
            unsigned short extended;
            fread(&extended, sizeof(short), 1, f);
            opcode_three(encoding, extended);
        }
    }
    fclose(f);
}

// Handle opcode zero instructions
void opcode_zero(unsigned short encoding) {
    // get instruction code from encoding and get instruction from table
    unsigned char instruction_code = (encoding & 0x3f);
    char *instruction = opcode_zero_instructions[instruction_code];

    // three cases for opcode 0
    if (strcmp(instruction, "std") == 0) {
        is_in_debug = 1;
    } else if (strcmp(instruction, "cld") == 0) {
        is_in_debug = 0;
    } else if (strcmp(instruction, "ret") == 0){
        printf("\tret\n");
    }
}

// opcode one handler
void opcode_one(unsigned short encoding) {
    unsigned char is_immediate = (encoding>>5) & 1;
    unsigned char instruction_code = (encoding & 0x1f);

    // Check if immediate instruction is branch or jump
    if (is_immediate) {
        char * instruction = opcode_one_immediate_instructions[instruction_code];
        unsigned char value = encoding>>8;

        if (strcmp(instruction, "br") == 0) {
            // branch on condition
            printf("\ttest %%r15, %%r15\n"
                   "\tjnz .L%04x\n"
                   ,curr_address+value-2);
        } else {
            // Jump to relative address
            printf("\tjmp .L%x\n", curr_address+value-2);
        }
    } else {
        char *instruction = opcode_one_instructions[instruction_code];
        unsigned char reg_index = encoding>>12;
        char *reg = reg_map64[reg_index];

        if (strcmp(instruction, "out") == 0) {
            // Output the value stored in register, using rdi as intermediete
            printf("\tmov %%rdi, -0x4(%%rbp)\n"
                   "\tmov %s, %%rdi\n"
                   "\tcall outchar\n"
                   "\tmov -0x4(%%rbp), %%rdi\n", reg);
        } else {
            printf("\t%s %s\n", instruction, reg);
        }
    }
}

// opcode two instruction handler
void opcode_two(unsigned short encoding) {
    // extract register part of encoding
    unsigned char reg_byte = encoding>>8;

    // get reg from array
    char *reg1 = reg_map64[reg_byte&0xf];
    int reg2_byte = (reg_byte>>4)&0xf;
    char *reg2 = reg_map64[reg2_byte];

    // x86 instruction and x(cpu) instruction
    char *instruction = opcode_two_instructions[encoding&0x3f];
    char *x_instruction = instruction;

    // Handle 'equ' as a special case, change the instruction to 'cmp'
    if (strcmp(instruction, "equ") == 0) {
        x_instruction = "equ";
        instruction = "cmp";
    }

    // handle load/stor instructions
    if (strcmp(x_instruction, "load") == 0) {
        printf("\tmov (%s), %s\n", reg2, reg1);
    } else if (strcmp(x_instruction, "stor") == 0) {
        printf("\tmov %s, (%s)\n", reg2, reg1);
    }  else if (strcmp(x_instruction, "loadb") == 0) {
        printf("\tmovzb (%s), %s\n", reg2, reg1);
    }  else if (strcmp(x_instruction, "storb") == 0) {
        // Store the lower byte of reg2 to the memory address pointed by reg1
        char * reg_8 = reg_map64[reg2_byte + 16];
        printf("\tmovb %s, (%s)\n", reg_8, reg1);
    } else {
        printf("\t%s %s, %s\n", instruction, reg2, reg1);
    }

    // Set flags based on the result of the operation
    if (strcmp(x_instruction, "test") == 0) {
        printf("\tsetne %%r15b\n");
    } else if (strcmp(x_instruction, "cmp") == 0) {
        printf("\tseta %%r15b\n");
    } else if (strcmp(x_instruction, "equ") == 0) {
        printf("\tsetz %%r15b\n");
    }
}

//opcode three instruction handler
void opcode_three(unsigned short encoding, unsigned short extended) {
    unsigned char is_immediate = (encoding>>5) & 1;
    unsigned short val = little_to_big(extended);
    unsigned char instruction_code = (encoding & 0x1f);
    char *instruction = opcode_three_instructions[instruction_code];

    curr_address+=2;
    if (is_immediate) {
        unsigned char reg_byte = encoding>>8;
        unsigned char reg_index = reg_byte>>4;
        char *reg = reg_map64[reg_index];

        // Move the immediate value to the register
        printf("\tmov $%d, %s\n", val, reg);
    } else {
        // Jump/call to the address specified by the extended value
        printf("\t%s .L%04x\n", instruction, val);
    }
}

// Convert little endian to big endian
unsigned short little_to_big(unsigned short value) {
    return ((value >> 8) | (value << 8));
}