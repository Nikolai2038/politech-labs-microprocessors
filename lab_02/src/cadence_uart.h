#ifndef CADENCE_UART_H_
#define CADENCE_UART_H_

#define BIT(x) (1UL << (x))

/* Register offsets for the UART. */
#define CDNS_UART_CR		0x00  /* Control Register */
#define CDNS_UART_MR		0x04  /* Mode Register */
#define CDNS_UART_IER		0x08  /* Interrupt Enable */
#define CDNS_UART_IDR		0x0C  /* Interrupt Disable */
#define CDNS_UART_IMR		0x10  /* Interrupt Mask */
#define CDNS_UART_ISR		0x14  /* Interrupt Status */
#define CDNS_UART_BAUDGEN	0x18  /* Baud Rate Generator */
#define CDNS_UART_RXTOUT	0x1C  /* RX Timeout */
#define CDNS_UART_RXWM		0x20  /* RX FIFO Trigger Level */
#define CDNS_UART_MODEMCR	0x24  /* Modem Control */
#define CDNS_UART_MODEMSR	0x28  /* Modem Status */
#define CDNS_UART_SR		0x2C  /* Channel Status */
#define CDNS_UART_FIFO		0x30  /* FIFO */
#define CDNS_UART_BAUDDIV	0x34  /* Baud Rate Divider */
#define CDNS_UART_FLOWDEL	0x38  /* Flow Delay */
#define CDNS_UART_IRRX_PWIDTH	0x3C  /* IR Min Received Pulse Width */
#define CDNS_UART_IRTX_PWIDTH	0x40  /* IR Transmitted pulse Width */
#define CDNS_UART_TXWM		0x44  /* TX FIFO Trigger Level */
#define CDNS_UART_RXBS		0x48  /* RX FIFO byte status register */

/* Control Register Bit Definitions */
#define CDNS_UART_CR_STOPBRK	0x00000100  /* Stop TX break */
#define CDNS_UART_CR_STARTBRK	0x00000080  /* Set TX break */
#define CDNS_UART_CR_TX_DIS	0x00000020  /* TX disabled. */
#define CDNS_UART_CR_TX_EN	0x00000010  /* TX enabled */
#define CDNS_UART_CR_RX_DIS	0x00000008  /* RX disabled. */
#define CDNS_UART_CR_RX_EN	0x00000004  /* RX enabled */
#define CDNS_UART_CR_TXRST	0x00000002  /* TX logic reset */
#define CDNS_UART_CR_RXRST	0x00000001  /* RX logic reset */
#define CDNS_UART_CR_RST_TO	0x00000040  /* Restart Timeout Counter */
#define CDNS_UART_RXBS_PARITY    0x00000001 /* Parity error status */
#define CDNS_UART_RXBS_FRAMING   0x00000002 /* Framing error status */
#define CDNS_UART_RXBS_BRK       0x00000004 /* Overrun error status */

/*
 * Mode Register:
 * The mode register (MR) defines the mode of transfer as well as the data
 * format. If this register is modified during transmission or reception,
 * data validity cannot be guaranteed.
 */
#define CDNS_UART_MR_CLKSEL		0x00000001  /* Pre-scalar selection */
#define CDNS_UART_MR_CHMODE_L_LOOP	0x00000200  /* Local loop back mode */
#define CDNS_UART_MR_CHMODE_NORM	0x00000000  /* Normal mode */
#define CDNS_UART_MR_CHMODE_MASK	0x00000300  /* Mask for mode bits */

#define CDNS_UART_MR_STOPMODE_2_BIT	0x00000080  /* 2 stop bits */
#define CDNS_UART_MR_STOPMODE_1_BIT	0x00000000  /* 1 stop bit */

#define CDNS_UART_MR_PARITY_NONE	0x00000020  /* No parity mode */
#define CDNS_UART_MR_PARITY_MARK	0x00000018  /* Mark parity mode */
#define CDNS_UART_MR_PARITY_SPACE	0x00000010  /* Space parity mode */
#define CDNS_UART_MR_PARITY_ODD		0x00000008  /* Odd parity mode */
#define CDNS_UART_MR_PARITY_EVEN	0x00000000  /* Even parity mode */

#define CDNS_UART_MR_CHARLEN_6_BIT	0x00000006  /* 6 bits data */
#define CDNS_UART_MR_CHARLEN_7_BIT	0x00000004  /* 7 bits data */
#define CDNS_UART_MR_CHARLEN_8_BIT	0x00000000  /* 8 bits data */

/*
 * Interrupt Registers:
 * Interrupt control logic uses the interrupt enable register (IER) and the
 * interrupt disable register (IDR) to set the value of the bits in the
 * interrupt mask register (IMR). The IMR determines whether to pass an
 * interrupt to the interrupt status register (ISR).
 * Writing a 1 to IER Enables an interrupt, writing a 1 to IDR disables an
 * interrupt. IMR and ISR are read only, and IER and IDR are write only.
 * Reading either IER or IDR returns 0x00.
 * All four registers have the same bit definitions.
 */
#define CDNS_UART_IXR_TOUT	0x00000100 /* RX Timeout error interrupt */
#define CDNS_UART_IXR_PARITY	0x00000080 /* Parity error interrupt */
#define CDNS_UART_IXR_FRAMING	0x00000040 /* Framing error interrupt */
#define CDNS_UART_IXR_OVERRUN	0x00000020 /* Overrun error interrupt */
#define CDNS_UART_IXR_TXFULL	0x00000010 /* TX FIFO Full interrupt */
#define CDNS_UART_IXR_TXEMPTY	0x00000008 /* TX FIFO empty interrupt */
#define CDNS_UART_ISR_RXEMPTY	0x00000002 /* RX FIFO empty interrupt */
#define CDNS_UART_IXR_RXTRIG	0x00000001 /* RX FIFO trigger interrupt */
#define CDNS_UART_IXR_RXFULL	0x00000004 /* RX FIFO full interrupt. */
#define CDNS_UART_IXR_RXEMPTY	0x00000002 /* RX FIFO empty interrupt. */
#define CDNS_UART_IXR_RXMASK	0x000021e7 /* Valid RX bit mask */

	/*
	 * Do not enable parity error interrupt for the following
	 * reason: When parity error interrupt is enabled, each Rx
	 * parity error always results in 2 events. The first one
	 * being parity error interrupt and the second one with a
	 * proper Rx interrupt with the incoming data.  Disabling
	 * parity error interrupt ensures better handling of parity
	 * error events. With this change, for a parity error case, we
	 * get a Rx interrupt with parity error set in ISR register
	 * and we still handle parity errors in the desired way.
	 */

#define CDNS_UART_RX_IRQS	(CDNS_UART_IXR_FRAMING | \
				 CDNS_UART_IXR_OVERRUN | \
				 CDNS_UART_IXR_RXTRIG |	 \
				 CDNS_UART_IXR_TOUT)

/* Goes in read_status_mask for break detection as the HW doesn't do it*/
#define CDNS_UART_IXR_BRK	0x00002000

#define CDNS_UART_RXBS_SUPPORT BIT(1)
/*
 * Modem Control register:
 * The read/write Modem Control register controls the interface with the modem
 * or data set, or a peripheral device emulating a modem.
 */
#define CDNS_UART_MODEMCR_FCM	0x00000020 /* Automatic flow control mode */
#define CDNS_UART_MODEMCR_RTS	0x00000002 /* Request to send output control */
#define CDNS_UART_MODEMCR_DTR	0x00000001 /* Data Terminal Ready */

/*
 * Channel Status Register:
 * The channel status register (CSR) is provided to enable the control logic
 * to monitor the status of bits in the channel interrupt status register,
 * even if these are masked out by the interrupt mask register.
 */
#define CDNS_UART_SR_RXEMPTY	0x00000002 /* RX FIFO empty */
#define CDNS_UART_SR_TXEMPTY	0x00000008 /* TX FIFO empty */
#define CDNS_UART_SR_TXFULL		0x00000010 /* TX FIFO full */
#define CDNS_UART_SR_RXTRIG		0x00000001 /* Rx Trigger */
#define CDNS_UART_SR_TACTIVE	0x00000800 /* TX state machine active */

#endif /* CADENCE_UART_H_ */
