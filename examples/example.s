.globl test
test:
    push %rbp
    mov %rsp, %rbp
    mov $2, %rax
    mov $3, %rbx
    mov $4, %rcx
    mov $5, %rdx
    mov $7, %rdi
    call debug
    add %rcx, %rdx
    call debug
    imul %rcx, %rbx
    call debug
    neg %rax
    inc %rdi
    pop %rbp
    ret
