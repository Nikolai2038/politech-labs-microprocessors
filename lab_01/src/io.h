#ifndef IO_H_
#define IO_H_

#include "types.h"

/* set __ARM_ARCH__ as needed */
#define __ARM_ARCH__ 7
/* DMA with outer cached memory */
#define ARM_DMA_MEM_BUFFERABLE
/* Use symmetric multiprocessor synchronization barriers */
/* #define SMP */

#define __iomem
#define __force

static inline void __raw_writew(u16 val, volatile void __iomem *addr)
{
	asm volatile("strh %1, %0"
		     : : "Q" (*(volatile u16 __force *)addr), "r" (val));
}

static inline u16 __raw_readw(const volatile void __iomem *addr)
{
	u16 val;
	asm volatile("ldrh %0, %1"
		     : "=r" (val)
		     : "Q" (*(volatile u16 __force *)addr));
	return val;
}

static inline void __raw_writeb(u8 val, volatile void __iomem *addr)
{
	asm volatile("strb %1, %0"
		     : : "Qo" (*(volatile u8 __force *)addr), "r" (val));
}

static inline void __raw_writel(u32 val, volatile void __iomem *addr)
{
	asm volatile("str %1, %0"
		     : : "Qo" (*(volatile u32 __force *)addr), "r" (val));
}

static inline u8 __raw_readb(const volatile void __iomem *addr)
{
	u8 val;
	asm volatile("ldrb %0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile u8 __force *)addr));
	return val;
}

static inline u32 __raw_readl(const volatile void __iomem *addr)
{
	u32 val;
	asm volatile("ldr %0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile u32 __force *)addr));
	return val;
}

#define readb_relaxed(c) ({ u8  __r = __raw_readb(c); __r; })
#define readw_relaxed(c) ({ u16 __r = le16_to_cpu((__force __le16) \
					__raw_readw(c)); __r; })
#define readl_relaxed(c) ({ u32 __r = le32_to_cpu((__force __le32) \
					__raw_readl(c)); __r; })

#define writeb_relaxed(v,c)	__raw_writeb(v,c)
#define writew_relaxed(v,c)	__raw_writew((__force u16) cpu_to_le16(v),c)
#define writel_relaxed(v,c)	__raw_writel((__force u32) cpu_to_le32(v),c)


#if __ARM_ARCH__ >= 7

#define isb(option) __asm__ __volatile__ ("isb " #option : : : "memory")
#define dsb(option) __asm__ __volatile__ ("dsb " #option : : : "memory")
#define dmb(option) __asm__ __volatile__ ("dmb " #option : : : "memory")

#elif __ARM_ARCH__ == 6
#define isb(x) __asm__ __volatile__ ("mcr p15, 0, %0, c7, c5, 4" \
				    : : "r" (0) : "memory")
#define dsb(x) __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 4" \
				    : : "r" (0) : "memory")
#define dmb(x) __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" \
				    : : "r" (0) : "memory")
#else
#define isb(x) __asm__ __volatile__ ("" : : : "memory")
#define dsb(x) __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 4" \
				    : : "r" (0) : "memory")
#define dmb(x) __asm__ __volatile__ ("" : : : "memory")

#endif

#define barrier() asm volatile ("" ::: "memory") /* compiler-only optimization barrier */
#define __arm_heavy_mb(x...) dsb(x) /* redefine as needed by platform ...  */

/* synchronization barriers adoptive implemetation */
#if defined(DMA_MEM_BUFFERABLE) || defined(SMP)
#define mb()		__arm_heavy_mb()
#define rmb()		dsb()
#define wmb()		__arm_heavy_mb(st)
#define dma_rmb()	dmb(osh)
#define dma_wmb()	dmb(oshst)
#else
#define mb()		barrier()
#define rmb()		barrier()
#define wmb()		barrier()
#define dma_rmb()	barrier()
#define dma_wmb()	barrier()
#endif


#ifdef ARM_DMA_MEM_BUFFERABLE
#define __iormb()		rmb()
#define __iowmb()		wmb()
#else
#define __iormb()		do { } while (0)
#define __iowmb()		do { } while (0)
#endif

#define readb(c)		({ u8  __v = readb_relaxed(c); __iormb(); __v; })
#define readw(c)		({ u16 __v = readw_relaxed(c); __iormb(); __v; })
#define readl(c)		({ u32 __v = readl_relaxed(c); __iormb(); __v; })

#define writeb(v,c)		({ __iowmb(); writeb_relaxed(v,c); })
#define writew(v,c)		({ __iowmb(); writew_relaxed(v,c); })
#define writel(v,c)		({ __iowmb(); writel_relaxed(v,c); })

#endif /* IO_H_ */
