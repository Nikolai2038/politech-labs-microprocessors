/* Биты и поля режимов PSR регистров для ARMv4T */

/* Поле режима */
.set MODE_MASK, 0x1F   /* Маска поля режима */
.set MODE_USR,  0x10   /* User */
.set MODE_FIQ,  0x11   /* FIQ */
.set MODE_IRQ,  0x12   /* IRQ */
.set MODE_SVC,  0x13   /* Supervisor */
.set MODE_ABT,  0x17   /* Abort */
.set MODE_UND,  0x1B   /* Undefined */
.set MODE_SYS,  0x1F   /* System */

/* Биты режимов */
.set I_BIT, 0x80      /* Маскирование IRQ */
.set F_BIT, 0x40      /* Маскирование FIQ */
.set T_BIT, 0x20      /* Режим Thumb (16-битная кодировка инструкций) */

/* Флаги */
.set N_FLG, 0x80000000  /* Результат < 0  */
.set Z_FLG, 0x40000000  /* Результат == 0 */
.set C_FLG, 0x20000000  /* Перенос */
.set V_FLG, 0x10000000  /* Переполнение */
