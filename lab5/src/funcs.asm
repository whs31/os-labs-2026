global swap, min, pow
section .text
swap:                   ; swap(long *a, long *b)
    mov rax, [rdi]
    mov rdx, [rsi]
    mov [rdi], rdx
    mov [rsi], rax
    ret
min:                    ; min(unsigned a, unsigned b) -> RAX
    mov rax, rdi
    cmp rdi, rsi
    jbe .done
    mov rax, rsi
.done:
    ret
pow:                    ; pow(x, p) -> RAX = x^p
    mov rax, 1
.loop:
    test rsi, rsi
    jz .done
    imul rax, rdi
    dec rsi
    jmp .loop
.done:
    ret
