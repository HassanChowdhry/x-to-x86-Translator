.globl debug

debug:
    push %rsp
    push %rbp
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx
    push %rax
    mov %rsp, %rdi

    call outregs

    pop %rax
    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
    pop %rbp
    pop %rsp
    ret

.globl runit
runit:
    push %rbp
    mov %rsp, %rbp
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx
    push %rax

    mov $0, %r15
    mov $0, %r14
    mov $0, %r13
    mov $0, %r12
    mov $0, %r11
    mov $0, %r10
    mov $0, %r9
    mov $0, %r8
    mov $0, %rdi
    mov $0, %rsi
    mov $0, %rdx
    mov $0, %rcx
    mov $0, %rbx
    mov $0, %rax

    call test

    pop %rax
    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
    pop %rbp
    ret

.globl outchar
outchar:
    push %rsp
    push %rbp
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx
    push %rax

    call do_outchar

    pop %rax
    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
    pop %rbp
    pop %rsp
    ret
