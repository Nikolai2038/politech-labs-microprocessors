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
   @ 2.1. Выполнение арифметических операций в регистрах (режим процессора может быть любым)

   mov r0, #7     @ Записываем число 7 в R0
   mov r1, #5     @ Записываем число 5 в R1
   add r2, r0, r1 @ Складываем и помещаем результат в R2

   @ 2.2. Чтение и запись слов в памяти

   ldr r3, =data   @ Записываем адрес переменной data в регистр R3
   ldr r4, [r3]    @ Загружаем значение переменной в R4
   add r4, r4, #10 @ Увеличиваем значение в R4 на 10
   str r4, [r3]    @ Сохраняем изменённое значение обратно в память

   @ 2.3. Проверка условия и условное выполнение инструкций

   mov   r5, #7 @ Записываем число 7 в R5
   cmp   r5, #8 @ Сравниваем R5 с 8
   moveq r6, #1 @ Если R5 == 8, то записываем 1 в R6
   movne r6, #0 @ Если R5 != 8, то записываем 0 в R6

   @ 2.4. Передача управления коду функции и возврат по регистру LR.

   bl func

   @ 2.5. Организация стека, помещение и извлечение слов из/в регистры - аналоги действий PUSH и POP. Регистр указателя стека и направление роста выберите самостоятельно.

   stmfd sp!, {r0-r3} @ Сохраняем регистры R0-R3 в стек
   mov   r0, #11      @ Изменяем значения в R0
   mov   r1, #21      @ Изменяем значения в R1
   mov   r2, #31      @ Изменяем значения в R2
   mov   r2, #41      @ Изменяем значения в R3
   ldmfd sp!, {r0-r3} @ Восстанавливаем регистры R0-R3 из стека

   @ 2.6. Переключение режима работы процессора, изменяя младшие 5 бит регистра CPSR. Начальный режим - режим SVC по сбросу. Покажите работу переключения банков регистров на примере значений регистра SP (R13).

   mrs r8, cpsr      @ Сохраняем текущее состояние CPSR в R8
   bic r8, r8, #0x1f @ Очищаем биты режима в CPSR
   orr r8, r8, #0x13 @ Устанавливаем режим SVC (0x13)
   msr cpsr_c, r8    @ Переключаем процессор в режим SVC

   @ 2.7. Автоматическая смена режима работы процессора при возникновении исключения, а также переход к обработке и возврат с сохранением и восстановлением состояния CPSR.

   swi #0 @ Генерация системного вызова

   b nothing_to_do

func:
   mov r0, #17         @ Записываем число 17 в R0
   mov r1, #15         @ Записываем число 15 в R1
   add r2, r0, r1      @ Складываем и помещаем результат в R2

   @ Переменные
   .section .data
data:
   .word 0x000000ff   @ Инициализация переменной

nothing_to_do:
   wfi
   b nothing_to_do

.end
