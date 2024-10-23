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
     ldr pc, vect2
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
   b vec_swi

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

   msr CPSR_c, #MODE_SYS|I_BIT|F_BIT
   ldr sp, =ram_end

   mov     r0, #0
   ldr     r1, =_bss_start
   ldr     r2, =_bss_end
zero_bss:
   cmp     r1, r2
   strlo   r0, [r1], #4
   blo     zero_bss

   ldr lr, =start_ret
   ldr r0, =start
   bx r0

start_ret:
   wfi
   b start_ret

.end
