#include <stdint.h>

#include "ioports.h"

#define PIC_1_CTRL 0x20  // primary PIC control register
#define PIC_2_CTRL 0xa0  // secondary PIC control register
#define PIC_1_DATA 0x21  // primary PIC data register
#define PIC_2_DATA 0xa1  // secondary PIC data registe

#define ICW_1 0x11  // 00010001 binary. Enables initialization mode and ICW 4

#define IRQ_0 0x20  // IRQs 0-7 mapped to interrupts 0x20-0x27
#define IRQ_8 0x28  // IRQs 8-15 mapped to use interrupts 0x28-0x36

#define ICW_3_PRI 0x04  // bit 2 so use IR line 2 to comunicate with secondary PIC
#define ICW_3_SEC 0x02  // use line 2 to communicate with primary PIC

#define ICW_4 0x01  // bit 0 enables 80x86 mode

#define PIC_EOI 0x20

/* The PIC fires hardware IRQs. There are typically two PICs, a master
 * and a slave, each with 8 pins for different hardware interrupts.
 * The PICs must be initialised so they know which IRs to call. This
 * involves sending a series of initialization control words (ICWs) to
 * the ports defined above.
 * ICW1 is 00010001b (0x11)
 *                +- PIC expects to recieve all 4 ICWs
 *               +-- Two PICs total
 *              +--- Not used in x86
 *             +---- Operate in edge triggered, not level triggered
 *                   mode. An IR corresponds to a single electrical 
 *                   pulse
 *            +----- PIC is being initialized
 *         +++------ Reserved
 * ICW2 gives the base address of the idt for interrupts to use. We
 * are using 32 (0x20) as the first, as this is the first non-preset/
 * reserved interrupt number. As each PIC has 8 pins, the slave PIC
 * starts at interrupt number 0x28.
 * ICW3 tells which IRQ line (pin) on the master PIC connects to the 
 * slave PIC. In x86, this must be line 2, which corresponds to 0x04
 * (bit 2) for the master PIC and 0x02 (binary 2) for the slave PIC.
 * ICW4 tells the PICs that we are operating in x86 mode as bit 0 is 1.
 * Finally, we write to the interrupt mask register (same port as data
 * register), to tell it which IRQs are allowed to fire. Each bit corresponds
 * to an IRQ, with 0 allowing it to fire. Thus we send 0x00 to allow all
 * IRQs to fire.
 *
 * An end of interrupt signal (EOI) must also be sent after each interrupt
 * to allow subsiquent interrupts.
 */

void initializePIC()
{
    port_byte_out(PIC_1_CTRL, ICW_1);
    port_byte_out(PIC_2_CTRL, ICW_1);
    port_byte_out(PIC_1_DATA, IRQ_0);
    port_byte_out(PIC_2_DATA, IRQ_8);
    port_byte_out(PIC_1_DATA, ICW_3_PRI);
    port_byte_out(PIC_2_DATA, ICW_3_SEC);
    port_byte_out(PIC_1_DATA, ICW_4);
    port_byte_out(PIC_2_DATA, ICW_4);
    port_byte_out(PIC_1_DATA, 0x00);
    port_byte_out(PIC_2_DATA, 0x00);
}

void sendPICEOI(uint8_t irq)
{
    port_byte_out(PIC_1_CTRL, PIC_EOI);
    if (irq >=8)
        port_byte_out(PIC_2_CTRL, PIC_EOI);
}