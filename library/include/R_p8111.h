/******************************************************************************
	R_P8111.H / R_P8111.C
	
	変更履歴
	Ver 3.49	2006/09/06	正式リリース
	Ver 3.50	2009/02/13	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_P8111_H_
#define	R_P8111_H_

#define	SPI_OPCODE_READ		3	// Read data from memory array beginning ata selected address
#define	SPI_OPCODE_WRITE	2	// Write data to memory array beginning at selected address
#define	SPI_OPCODE_WREN		6	// Set tge write enable latch (enable write operations)
#define	SPI_OPCODE_WRDI		4	// Reset the write enable latch (disable write operations)
#define	SPI_OPCODE_RDSR		5	// Read Status register
#define	SPI_OPCODE_WRSR		1	// Write Status register

int	R_P8111_eeprom_putc(int nPort, int nAdr, int nData);
int	R_P8111_eeprom_getc(int nPort, int nAdr);

#endif	// R_P8111_H_
