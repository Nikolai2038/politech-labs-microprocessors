// Подключение заголовочного файла для определения типов данных
#include "types.h"
// Подключение заголовочного файла для ввода-вывода
#include "io.h"
// Подключение заголовочного файла для работы с UART Cadence
#include "cadence_uart.h"

// Определение базового адреса UART
#define UARTBASE 0xE0000000

// Структура для хранения информации о UART
struct uart_info {
	// Указатель на базовый адрес ввода-вывода UART
	void *iobase;
};

// Статическая переменная для хранения данных UART0
static struct uart_info uart0_data;

// Указатель на данные UART0
static struct uart_info *uart0 = &uart0_data;

// Функция для передачи одного символа через UART
static void UART_Tx(struct uart_info *uart, char c) {
	void *iobase;
	u32 sr;

	// Получение адреса ввода-вывода UART
	iobase = uart->iobase;

	do {
        // Чтение регистра состояния UART
        sr = readl(iobase + CDNS_UART_SR);
    // Ожидание, пока не освободится место в FIFO
	} while (sr & CDNS_UART_SR_TXFULL);

	// Запись символа в FIFO для передачи
	writel(c, iobase + CDNS_UART_FIFO);
}

// Функция для инициализации UART
static void UART_Init(struct uart_info *uart, void *init_iobase) {
	void *iobase;
	u32 cr;

	// Установка базового адреса ввода-вывода UART
	iobase = uart->iobase = init_iobase;

	// Чтение текущего значения регистра управления
	cr = readl(iobase + CDNS_UART_CR);

	// Отключение передачи
	writel(cr |= CDNS_UART_CR_TX_DIS, iobase +  CDNS_UART_CR);

	// Сброс передачи
	writel(cr |= CDNS_UART_CR_TXRST, iobase +  CDNS_UART_CR);

	do {
        // Ожидание завершения сброса
        cr = readl(iobase + CDNS_UART_CR);
    // Проверка, что сброс завершен
    } while (cr & CDNS_UART_CR_TXRST);

	// Включение передачи
	writel(cr = (cr & ~CDNS_UART_CR_TX_DIS) | CDNS_UART_CR_TX_EN, iobase +  CDNS_UART_CR);
}

// Функция для печати строки через UART
static void PrintSZ(char *p) {
	char c;

	// Цикл, пока не достигнут конец строки
	while ((c = *(p++))) {
        // Передача каждого символа
        UART_Tx(uart0, c);
	}
}

// Основная функция старта
void start(void) {
    int num1, num2, sum;

    UART_Init(uart0, (void *)UARTBASE);

    PrintSZ("Enter first number: ");
    // ...

    PrintSZ("Enter second number: ");
    // ...

    PrintSZ("Sum: ");
    // ...
}
