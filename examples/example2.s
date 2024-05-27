.globl test
test:
    push %rbp
    mov %rsp, %rbp
.L0000:
    mov $1, %rax
.L0004:
    jmp .L0010
.L0008:
    mov $3, %rax
.L000c:
    jmp .L0018
.L0010:
    mov $2, %rax
.L0014:
    jmp .L0008
.L0018:
.L001a:
    call debug
    mov $4, %rax
.L001e:
    call debug
    pop %rbp
    ret
