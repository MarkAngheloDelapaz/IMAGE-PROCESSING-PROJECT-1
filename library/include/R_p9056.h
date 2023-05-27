/******************************************************************************
	R_p9056.h / R_p9056.c
	
	ïœçXóöó
	Ver 3.50	2007/05/23	ê≥éÆÉäÉäÅ[ÉX
******************************************************************************/

#ifndef R_P9056_H_
#define R_P9056_H_

#ifdef __cplusplus
extern "C" {
#endif

// Local Configuration Space Local Configuration Registers
#define	PCI9056_LAS0RR		0x000
#define	PCI9056_LAS0BA		0x004
#define	PCI9056_MARBR		0x008
#define	PCI9056_BIGEND		0x00C
#define	PCI9056_LMISC2		0x00F	// Local Miscellaneous Control 2
#define	PCI9056_EROMBA		0x014	// Direct Slave Expansion ROM Local Base Address (Remap) and BREQo Control
#define	PCI9056_LBRD0		0x018	// Local Address Space 0/Expansion ROM Bus Region Descriptor
#define	PCI9056_DMRR		0x01C	// Local Range Register for PCI Initiator-to-PCI
#define	PCI9056_DMLBAM		0x020	// Local Bus Base Address Register for PCI Initiator-to-PCI Memory
#define	PCI9056_DMLBAI		0x024	// Local Bus Base Address Register for PCI Initiator-to-PCI I/O Configuration
#define	PCI9056_DMPBAM		0x028
#define	PCI9056_INTCSR		0x068	// Interrupt Control/Status
#define	PCI9056_CNTRL		0x06C
#define	PCI9056_LAS1RR		0x0F0
#define	PCI9056_LAS1BA		0x0F4
#define	PCI9056_PABTADR		0x104	// PCI Abort Address

// Local Configuration Space DMA Registers
#define	PCI9056_DMAMODE0	0x080	// DMA Channel 0 Mode
#define	PCI9056_DMAPADR0	0x084
#define	PCI9056_DMALADR0	0x088
#define	PCI9056_DMASIZ0		0x08C
#define	PCI9056_DMADPR0		0x090	// DMA Channel 0 Descriptor Pointer
#define	PCI9056_DMACSR0		0x0A8	// DMA Channel 0 Command/Status

#define	PCI9056_DMAMODE1	0x094	// DMA Channel 1 Mode
#define	PCI9056_DMAPADR1	0x098
#define	PCI9056_DMALADR1	0x09C
#define	PCI9056_DMASIZ1		0x0A0
#define	PCI9056_DMADPR1		0x0A4	// DMA Channel 1 Descriptor Pointer
#define	PCI9056_DMACSR1		0x0A9	// DMA Channel 1 Command/Status

#define	PCI9056_DMATHR		0x0B0	// DMA Threshold
#define	PCI9056_DMATHR0		0x0B0
#define	PCI9056_DMATHR1		0x0B2
#define	PCI9056_DMADAC0		0x0B4	// DMA Channel 0 PCI Dual Address Cycle Upper Address
#define	PCI9056_DMADAC1		0x0B8	// DMA Channel 1 PCI Dual Address Cycle Upper Address

int	R_P9056_eeprom_putw(unsigned long, int, int);
int	R_P9056_eeprom_getw(unsigned long, int);

#ifdef __cplusplus
}
#endif

#endif	// R_P9056_H_
