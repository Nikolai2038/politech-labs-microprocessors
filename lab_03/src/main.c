#include "types.h"
#include "io.h"
#include "cadence_uart.h"

#define UARTBASE 0xE0000000

struct uart_info {
    void *iobase;
};

static struct uart_info uart0_data;
static struct uart_info *uart0 = &uart0_data;

static int simple_atoi(const char *str) {
    int result = 0;
    while (*str) {
        result = result * 10 + (*str - '0'); // Преобразуем каждый символ в число
        str++;
    }
    return result;
}

static void int_to_str(int num, char *str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    // Обрабатываем отрицательные числа
    if (num < 0) {
        *ptr++ = '-';
        num = -num;
    }

    // Преобразуем число в строку
    do {
        tmp_value = num;
        num /= base; // В этом месте вызывается деление
        *ptr++ = "0123456789"[tmp_value - num * base];
    } while (num);

    *ptr-- = '\0';

    // Реверс строки
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

// Функция для передачи символа через UART
static void UART_Tx(struct uart_info *uart, char c) {
    void *iobase = uart->iobase;
    u32 sr;
    do {
        sr = readl(iobase + CDNS_UART_SR);
    } while (sr & CDNS_UART_SR_TXFULL); // Ожидаем, пока буфер не освободится
    writel(c, iobase + CDNS_UART_FIFO); // Отправляем символ
}

// Функция для приема символа через UART
static char UART_Rx(struct uart_info *uart) {
    void *iobase = uart->iobase;
    u32 sr;
    char c;
    do {
        sr = readl(iobase + CDNS_UART_SR);
    } while (!(sr & CDNS_UART_SR_RXEMPTY)); // Ждем, пока данные будут готовы
    c = readl(iobase + CDNS_UART_FIFO);     // Читаем полученный символ
    return c;
}

// Функция инициализации UART
static void UART_Init(struct uart_info *uart, void *init_iobase) {
    void *iobase = uart->iobase = init_iobase;
    u32 cr = readl(iobase + CDNS_UART_CR);
    writel(cr |= CDNS_UART_CR_TX_DIS, iobase + CDNS_UART_CR); // Отключаем передачу
    writel(cr |= CDNS_UART_CR_TXRST, iobase + CDNS_UART_CR);  // Сбрасываем передатчик
    do {
        cr = readl(iobase + CDNS_UART_CR);
    } while (cr & CDNS_UART_CR_TXRST); // Ждем завершения сброса
    writel(cr = (cr & ~CDNS_UART_CR_TX_DIS) | CDNS_UART_CR_TX_EN, iobase + CDNS_UART_CR); // Включаем передачу
}

// Вывод строки через UART
static void PrintSZ(char *p) {
    char c;
    while ((c = *(p++))) {
        UART_Tx(uart0, c);
    }
}

// Чтение числа из UART
static int ReadNumber() {
    char buffer[10];
    int i = 0;
    char c;
    while (1) {
        c = UART_Rx(uart0); // Ждем символа от пользователя
        if (c == '\r') {    // Конец ввода по символу возврата каретки
            buffer[i] = '\0';
            break;
        } else {
            buffer[i++] = c;
            UART_Tx(uart0, c); // Эхо-ввод
        }
    }
    return simple_atoi(buffer); // Преобразование строки в число
}

// Преобразование числа в строку
static void int_to_str(int num, char *str, int base);

// Главная функция программы
void start(void) {
    UART_Init(uart0, (void *)UARTBASE);

    while (1) {
        PrintSZ("\r\nEnter first number: ");
        int num1 = ReadNumber(); // Чтение первого числа

        PrintSZ("\r\nEnter second number: ");
        int num2 = ReadNumber(); // Чтение второго числа

        int sum = num1 + num2; // Сложение чисел

        char result[32];
        int_to_str(sum, result, 10);
        PrintSZ("\r\nSum: ");
        PrintSZ(result);
        PrintSZ("\r\n");
    }
}
