#include "gic.h"

GIC_Type * get_gic_base(void)
{
	GIC_Type *dst;

	__asm volatile ("mrc p15, 4, %0, c15, c0, 0" : "=r" (dst)); 

	return dst;
}

void gic_init(void)
{
	u32 i, irq_num;

	GIC_Type *gic = get_gic_base();

	/* the maximum number of interrupt IDs that the GIC supports */
	/* 读出GIC支持的最大的中断号 */
	/* 注意: 中断个数 = irq_num * 32 */
	irq_num = (gic->D_TYPER & 0x1F) + 1;

	/* Disable all PPI, SGI and SPI */
	/* 禁止所有的PPI、SIG、SPI中断 */
	for (i = 0; i < irq_num; i++)
	  gic->D_ICENABLER[i] = 0xFFFFFFFFUL;

	/* all set to group0 */
	/* 这些中断, 都发给group0 */
	for (i = 0; i < irq_num; i++)
	  gic->D_IGROUPR[i] = 0x0UL;

	/* all spi interrupt target for cpu interface 0 */
	/* 所有的SPI中断都发给cpu interface 0 */
	for (i = 32; i < (irq_num << 5); i++)
	  gic->D_ITARGETSR[i] = 0x01UL;

	/* all spi is level sensitive: 0-level, 1-edge */
	/* it seems level and edge all can work */
	/* 设置GIC内部的中断触发类型 */
	for (i = 2; i < irq_num << 1; i++)
	  gic->D_ICFGR[i] = 0x01010101UL;

	/* The priority mask level for the CPU interface. If the priority of an 
	 * interrupt is higher than the value indicated by this field, 
	 * the interface signals the interrupt to the processor.
	 */
	/* 把所有中断的优先级都设为最高 */
	gic->C_PMR = (0xFFUL << (8 - 5)) & 0xFFUL;
	
	/* No subpriority, all priority level allows preemption */
	/* 没有"次级优先级" */
	gic->C_BPR = 7 - 5;
	
	/* Enables the forwarding of pending interrupts from the Distributor to the CPU interfaces.
	 * Enable group0 distribution
	 */
	/* 使能:   Distributor可以给CPU interfac分发中断 */
	gic->D_CTLR = 1UL;
	
	/* Enables the signaling of interrupts by the CPU interface to the connected processor
	 * Enable group0 signaling 
	 */
	/* 使能:	 CPU interface可以给processor分发中断 */
	gic->C_CTLR = 1UL;
}

void gic_enable_irq(uint32_t nr)
{
	GIC_Type *gic = get_gic_base();

	/* The GICD_ISENABLERs provide a Set-enable bit for each interrupt supported by the GIC.
	 * Writing 1 to a Set-enable bit enables forwarding of the corresponding interrupt from the
	 * Distributor to the CPU interfaces. Reading a bit identifies whether the interrupt is enabled.
	 */
	gic->D_ISENABLER[nr >> 5] = (uint32_t)(1UL << (nr & 0x1FUL));

}

void gic_disable_irq(uint32_t nr)
{
	GIC_Type *gic = get_gic_base();

	/* The GICD_ICENABLERs provide a Clear-enable bit for each interrupt supported by the
	 * GIC. Writing 1 to a Clear-enable bit disables forwarding of the corresponding interrupt from
     * the Distributor to the CPU interfaces. Reading a bit identifies whether the interrupt is enabled. 
	 */
	gic->D_ICENABLER[nr >> 5] = (uint32_t)(1UL << (nr & 0x1FUL));
}


int get_gic_irq(void)
{
	int nr;

	GIC_Type *gic = get_gic_base();
	/* The processor reads GICC_IAR to obtain the interrupt ID of the
	 * signaled interrupt. This read acts as an acknowledge for the interrupt
	 */
	nr = gic->C_IAR;

	return nr;
}

int clear_gic_irq(int nr)
{

	GIC_Type *gic = get_gic_base();

	/* write GICC_EOIR inform the CPU interface that it has completed 
	 * the processing of the specified interrupt 
	 */
	gic->C_EOIR = nr;
}


