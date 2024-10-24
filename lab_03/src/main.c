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

// Функция для инициализации UART (TX и RX)
static void UART_Init(struct uart_info *uart, void *init_iobase) {
    void *iobase;
    u32 cr;

    // Установка базового адреса ввода-вывода UART
    iobase = uart->iobase = init_iobase;

    // Чтение текущего значения регистра управления
    cr = readl(iobase + CDNS_UART_CR);

    // Отключение передачи и приёма
    writel(cr |= (CDNS_UART_CR_TX_DIS | CDNS_UART_CR_RX_DIS), iobase + CDNS_UART_CR);

    // Сброс логики передачи и приёма
    writel(cr |= (CDNS_UART_CR_TXRST | CDNS_UART_CR_RXRST), iobase + CDNS_UART_CR);

    do {
        // Ожидание завершения сброса
        cr = readl(iobase + CDNS_UART_CR);
    } while ((cr & (CDNS_UART_CR_TXRST | CDNS_UART_CR_RXRST)) != 0);

    // Включение передачи и приёма
    writel(cr = (cr & ~(CDNS_UART_CR_TX_DIS | CDNS_UART_CR_RX_DIS)) | (CDNS_UART_CR_TX_EN | CDNS_UART_CR_RX_EN), iobase + CDNS_UART_CR);
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

// Функция для приёма одного символа через UART
static char UART_Rx(struct uart_info *uart) {
	void *iobase;
	u32 sr;
	char c;

	// Получение адреса ввода-вывода UART
	iobase = uart->iobase;

	do {
        // Чтение регистра состояния UART
        sr = readl(iobase + CDNS_UART_SR);
    // Ожидание символа в FIFO (пока FIFO не станет пустым)
	} while (sr & CDNS_UART_SR_RXEMPTY);

	// Чтение символа из FIFO
	c = readb(iobase + CDNS_UART_FIFO);
	return c;
}

// Функция для чтения числа из UART
static int ReadNumber(void) {
    char c;
    int number = 0;

    // Чтение символов, пока не встретится символ новой строки
    while ((c = UART_Rx(uart0)) != '\n') {
        // Проверка, является ли символ цифрой
        if (c >= '0' && c <= '9') {
            // Преобразование символа в число
            number = number * 10 + (c - '0');
            // Эхо-сообщение: отправка обратно через UART
            // UART_Tx(uart0, c);
        }
    }

    return number;
}

// Функция для преобразования целого числа в строку
static void itoa(int num, char *str) {
    int i = 0;
    int is_negative = 0;

    // Обработка отрицательных чисел
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Генерация цифр в обратном порядке
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    // Добавление знака минус для отрицательных чисел
    if (is_negative) {
        str[i++] = '-';
    }

    // Добавление завершающего нуля
    str[i] = '\0';

    // Переворот строки
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

// Основная функция старта
void start(void) {
    int num1, num2, sum;
    char buffer[16];

    UART_Init(uart0, (void *)UARTBASE);

    // Запрос первого числа
    PrintSZ("Enter first number: ");
    num1 = ReadNumber();

    UART_Init(uart0, (void *)UARTBASE);

    // Запрос второго числа
    PrintSZ("Enter second number: ");
    num2 = ReadNumber();

    UART_Init(uart0, (void *)UARTBASE);

    // Вычисление суммы
    sum = num1 + num2;

    // Печать результата
    PrintSZ("Sum: ");

    // Преобразование суммы в строку и печать через UART
    itoa(sum, buffer);
    PrintSZ(buffer);
    PrintSZ("\n");
}
