.section .init, "ax"
.global _start
_start:
    .cfi_startproc
    .cfi_undefined ra
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop
    la sp, __stack_top
    add s0, sp, zero
    jal ra, init
    nop
    jal ra, main
    .cfi_endproc


.section .text, "ax"
.global Timer, TimerInterval, ModeSelect, VideoCtrBG, VideoCtrLS, VideoCtrSS, Ctr_Status, Mode_Status, Text_Convert, Color_Random, CleanDot,
Timer:
TimerInterval:
ModeSelect:
VideoCtrBG:
VideoCtrLS:
VideoCtrSS:
Ctr_Status:
Mode_Status:
Text_Convert:
Color_Random:
CleanDot:
    ecall
    ret
    .end
