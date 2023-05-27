/******************************************************************************
	R_P9054.H / R_P9054.C
	
	変更履歴
	Ver 3.42	2004/02/17	正式リリース
	Ver 3.45	2005/01/20	内部関数P9054SetCS/ResetCSバグフィクス
	Ver 3.48	2006/08/04	PCI9056対応
******************************************************************************/

#ifndef	R_P9054_H_
#define	R_P9054_H_

// Local Configuration Space Local Configuration Registers
#define	PCI9054_LAS0RR		0x000
#define	PCI9054_LAS0BA		0x004
#define	PCI9054_MARBR		0x008
#define	PCI9054_BIGEND		0x00C
#define	PCI9054_LBRD0		0x018	// Local Address Space 0/Expansion ROM Bus Region Descriptor
#define	PCI9054_DMRR		0x01C	// Local Range Register for PCI Initiator-to-PCI
#define	PCI9054_DMLBAM		0x020	// Local Bus Base Address Register for PCI Initiator-to-PCI Memory
#define	PCI9054_DMLBAI		0x024	// Local Bus Base Address Register for PCI Initiator-to-PCI I/O Configuration
#define	PCI9054_DMPBAM		0x028
#define	PCI9054_CNTRL		0x06C
#define	PCI9054_LAS1RR		0x0F0
#define	PCI9054_LAS1BA		0x0F4

// Local Configuration Space DMA Registers
#define	PCI9054_DMAMODE0	0x080	// DMA Channel 0 Mode
#define	PCI9054_DMAPADR0	0x084
#define	PCI9054_DMALADR0	0x088
#define	PCI9054_DMASIZ0		0x08C
#define	PCI9054_DMADPR0		0x090	// DMA Channel 0 Descriptor Pointer
#define	PCI9054_DMACSR0		0x0A8	// DMA Channel 0 Command/Status

#define	PCI9054_DMAMODE1	0x094	// DMA Channel 1 Mode
#define	PCI9054_DMAPADR1	0x098
#define	PCI9054_DMALADR1	0x09C
#define	PCI9054_DMASIZ1		0x0A0
#define	PCI9054_DMADPR1		0x0A4	// DMA Channel 1 Descriptor Pointer
#define	PCI9054_DMACSR1		0x0A9	// DMA Channel 1 Command/Status

#define	PCI9054_DMATHR		0x0B0	// DMA Threshold
#define	PCI9054_DMATHR0		0x0B0
#define	PCI9054_DMATHR1		0x0B2
#define	PCI9054_DMADAC0		0x0B4	// DMA Channel 0 PCI Dual Address Cycle Upper Address
#define	PCI9054_DMADAC1		0x0B8	// DMA Channel 1 PCI Dual Address Cycle Upper Address

int	R_P9054_eeprom_putw(int, int, int);
int	R_P9054_eeprom_getw(int, int);

#endif	// R_P9054_H_
