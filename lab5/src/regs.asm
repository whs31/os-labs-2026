global _start
section .text
_start:
    mov rsi, 1
    mov rdx, 2
task1:                  ; 1. обмен RSI и RDX
    xchg rsi, rdx
task1_done:
    mov rsi, 40
    mov rdx, 2
task2:                  ; 2. RSI = RSI + RDX
    add rsi, rdx
task2_done:
    mov rsi, 212
task3:                  ; 3. RSI = 5 * (RSI - 32) / 9
    mov rax, rsi
    sub rax, 32
    mov rcx, 5
    mul rcx             ; RDX:RAX = RAX * 5
    mov rcx, 9
    div rcx             ; RAX = RDX:RAX / 9
    mov rsi, rax
task3_done:
    mov rsi, 7
    mov rdx, 9
task4:                  ; 4. обмен без других регистров
    xor rsi, rdx
    xor rdx, rsi
    xor rsi, rdx
task4_done:
    mov eax, 60         ; sys_exit(0)
    xor edi, edi
    syscall
