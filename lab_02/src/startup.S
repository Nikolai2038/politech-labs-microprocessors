.global vector_base
.global vect0
.extern start
.extern ram_end

.include "psr.inc"

.section .init_code
.arm

vector_base:
     ldr pc, vect0
     ldr pc, vect1
     ldr pc, vect2  @ set breakpoint here too
     ldr pc, vect3
     ldr pc, vect4
     ldr pc, vect5
     ldr pc, vect6
     ldr pc, vect7

vect0: .word vec_reset
vect1: .word vec_undefined_instruction
vect2: .word vec_swi
vect3: .word vec_prefetch_abort
vect4: .word vec_data_abort
vect5: .word vec_reserved
vect6: .word vec_irq_interrupt
vect7: .word vec_fast_irq_interrupt

.text
.arm

vec_reset:
   b init_reset

vec_undefined_instruction:
   b vec_undefined_instruction

vec_swi:
   nop   @ set BREAKPOINT here to verify SWI operation
   nop   @ watch LR, CPSR and SPSR here
   stmdb sp!, {r0-r12,lr} @ save all registers
   nop   @ have a look on SVC stack here -- open memory browser at $sp in GDB
   nop
   mrs r1, SPSR @ check SPSR in R1
   nop
   ldr r0,=0x11223344 @ check restore on return
   nop
   nop
   ldmia sp!, {r0-r12,pc}^ @ restotre all registers and return
   nop                     @ unreachable - will jump back before this instruction

vec_prefetch_abort:
   wfi
   b vec_prefetch_abort

vec_data_abort:
   wfi
   b vec_data_abort

vec_reserved:
   wfi
   b vec_reserved

vec_irq_interrupt:
   wfi
   b vec_irq_interrupt

vec_fast_irq_interrupt:
   wfi
   b vec_fast_irq_interrupt

init_reset:

   msr CPSR_c, #MODE_SVC|I_BIT|F_BIT
   ldr sp, =ram_end - 0x10000

   msr CPSR_c, #MODE_SYS|I_BIT|F_BIT
   ldr sp, =ram_end

   mrc   p15, 0,  r0, c1, c0, 0        @ Чтение CP15 SCTLR - биты управления системой
   bic   r0,  r0, #(1 << 13)           @ Бит 13 (бит V) = 0 - база векторов не в верху памяти (0xffff0000)
   mcr   p15, 0,  r0, c1, c0, 0	       @ Запись CP15 SCTLR
   ldr   r0,  =vector_base
   mcr   p15, 0,  r0, c12, c0, 0	   @ Запись базы векторов (VBAR)

   nop
   ldr r0,=0x12345678   @ check restore of R0 on return
   ldr lr,=0x55667788   @ check restore on LR return
   nop

   swi #1  @ test exception

   nop     @ expect to return here after SWI
   nop


nothing_to_do:
   wfi
   b nothing_to_do

.end
