set disable-randomization off
set pagination off
break task1
commands
silent
printf "1) xchg   до:    rsi=%d rdx=%d\n", $rsi, $rdx
continue
end
break task1_done
commands
silent
printf "1) xchg   после: rsi=%d rdx=%d\n", $rsi, $rdx
continue
end
break task2_done
commands
silent
printf "2) 40+2   = rsi=%d\n", $rsi
continue
end
break task3_done
commands
silent
printf "3) 212F   = rsi=%dC\n", $rsi
continue
end
break task4
commands
silent
printf "4) xor    до:    rsi=%d rdx=%d rax=%d\n", $rsi, $rdx, $rax
continue
end
break task4_done
commands
silent
printf "4) xor    после: rsi=%d rdx=%d rax=%d\n", $rsi, $rdx, $rax
continue
end
run
