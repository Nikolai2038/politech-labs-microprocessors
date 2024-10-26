#include "types.h"
#include "io.h"
#include "cadence_uart.h"

#define UARTBASE 0xE0000000

struct uart_info {
	void *iobase;
};

static struct uart_info uart0_data;
static struct uart_info *uart0 = &uart0_data;

static void UART_Tx(struct uart_info *uart, char c) {
	void *iobase;
	u32 sr;
	iobase = uart->iobase;
	do {
		sr = readl(iobase + CDNS_UART_SR);
	} while (sr & CDNS_UART_SR_TXFULL);
	writel(c, iobase +  CDNS_UART_FIFO);
}

static void UART_Init(struct uart_info *uart, void *init_iobase) {
	void *iobase;
	u32 cr;
	iobase = uart->iobase = init_iobase;
	cr = readl(iobase + CDNS_UART_CR);
	writel(cr |= CDNS_UART_CR_TX_DIS, iobase +  CDNS_UART_CR);
	writel(cr |= CDNS_UART_CR_TXRST, iobase +  CDNS_UART_CR);
	do
		cr = readl(iobase + CDNS_UART_CR);
	while (cr & CDNS_UART_CR_TXRST);
	writel(cr = (cr & ~CDNS_UART_CR_TX_DIS) | CDNS_UART_CR_TX_EN, iobase +  CDNS_UART_CR);
}

static void PrintSZ(char *p) {
	char c;
	while ((c = *(p++))) {
		UART_Tx(uart0, c);
	}
}

void start(void) {
	UART_Init(uart0, (void *)UARTBASE);
	PrintSZ("Hello, World!\n");
	return;
}
