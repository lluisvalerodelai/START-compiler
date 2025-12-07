.global main
main:
  push %rbp
  mov %rsp, %rbp
  mov $6, %rax
  push %rax
  mov $5, %rax
  pop %rbx
  imul %rbx, %rax
  push %rax
  mov $4, %rax
  pop %rbx
  add %rbx, %rax
  push %rax
  mov $3, %rax
  pop %rbx
  add %rbx, %rax
  mov %rax, -8(%rbp)
  mov $3, %rax
  mov %rax, -16(%rbp)
  mov $4, %rax
  push %rax
  mov -16(%rbp), %rax
  pop %rbx
  cmp %rbx, %rax
  setle %al
  movzbl %al, %eax
  cmp $0, %rax
  je .L0
  mov $10, %rax
  push %rax
  mov $10, %rax
  pop %rbx
  cmp %rbx, %rax
  setne %al
  movzbl %al, %eax
  cmp $0, %rax
  sete %al
  movzbl %al, %eax
  cmp $0, %rax
  setne %al
  movzbl %al, %eax
.L0:
  cmp $0, %rax
  je .L1
  mov $13, %rax
  push %rax
  mov $13, %rax
  pop %rbx
  cmp %rbx, %rax
  sete %al
  movzbl %al, %eax
  cmp $0, %rax
  setne %al
  movzbl %al, %eax
.L1:
  cmp $0, %rax
  je .L2
  mov $13, %rax
  push %rax
  mov $14, %rax
  pop %rbx
  cmp %rbx, %rax
  setge %al
  movzbl %al, %eax
  cmp $0, %rax
  setne %al
  movzbl %al, %eax
.L2:
  cmp $0, %rax
  jne .L3
  mov $4, %rax
  push %rax
  mov $3, %rax
  pop %rbx
  cmp %rbx, %rax
  setl %al
  movzbl %al, %eax
  cmp $0, %rax
  jne .L4
  mov $5, %rax
  push %rax
  mov $10, %rax
  pop %rbx
  add %rbx, %rax
  push %rax
  mov $4, %rax
  pop %rbx
  cmp %rbx, %rax
  setl %al
  movzbl %al, %eax
  cmp $0, %rax
  setne %al
  movzbl %al, %eax
.L4:
  cmp $0, %rax
  setne %al
  movzbl %al, %eax
.L3:
  mov %rax, -24(%rbp)
  mov $3, %rax
  push %rax
  mov $3, %rax
  pop %rbx
  cmp %rbx, %rax
  setle %al
  movzbl %al, %eax
  cmp $0, %rax
  je .L6
  mov $4, %rax
  push %rax
  mov $4, %rax
  pop %rbx
  cmp %rbx, %rax
  setge %al
  movzbl %al, %eax
  cmp $0, %rax
  setne %al
  movzbl %al, %eax
.L6:
  cmp $0, %rax
  jne .L7
  mov $10, %rax
  push %rax
  mov $10, %rax
  pop %rbx
  cmp %rbx, %rax
  setne %al
  movzbl %al, %eax
  cmp $0, %rax
  sete %al
  movzbl %al, %eax
  cmp $0, %rax
  setne %al
  movzbl %al, %eax
.L7:
  cmp $0, %rax
  je .L5
  mov $13, %rax
  push %rax
  mov $25, %rax
  pop %rbx
  add %rbx, %rax
  mov %rax, -8(%rbp)
.L5:
  mov $0, %rax
  mov %rax, -32(%rbp)
.L8:
  mov $10, %rax
  push %rax
  mov -32(%rbp), %rax
  pop %rbx
  cmp %rbx, %rax
  setl %al
  movzbl %al, %eax
  cmp $0, %rax
  je .L9
  mov $1, %rax
  push %rax
  mov -32(%rbp), %rax
  pop %rbx
  add %rbx, %rax
  mov %rax, -32(%rbp)
  jmp .L8
.L9:
  mov $0, %rax
  mov %rbp, %rsp
  pop %rbp
  ret
