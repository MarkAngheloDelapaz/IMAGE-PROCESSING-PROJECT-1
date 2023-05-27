/******************************************************************************
	R_APIC21.H / R_APIC21.C
	
	変更履歴
	Ver 3.28	2001/02/07	release
	Ver 3.33	2003/01/16	R_apic21_eeprom_getw/putw追加
	Ver 3.42	2004/02/17	R_Apic21_getw/putw_eepromバグフィクス
	Ver.3.48	2006/05/08	R_Apic21_putw_eepromプロトタイプ修正
	Ver 3.50	2009/02/12	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_APIC_H_
#define	R_APIC_H_

#ifdef __cplusplus
extern "C" {
#endif

//EEPROM (APIC)
#define	APIC_EEPROM_SK	0x1
#define	APIC_EEPROM_CS	0x2				// CS up
#define	APIC_EEPROM_DI	0x4				//
#define	APIC_EEPROM_DO	0x8

int R_Apic21_getw_eeprom(int nPort, int adr);
int R_Apic21_putw_eeprom(int nPort, int adr, unsigned short int data);		// int data -> unsigned short int data  2006.05.08  by matsu
int R_apic21_eeprom_getw(int nPort, int adr);
int R_apic21_eeprom_putw(int nPort, int adr, int data);

int RApic21OpenAll(int num, int bMultiProcess);
unsigned int	r_apic21_inp(int fd, unsigned long port);
unsigned int	r_apic21_outp(int fd, unsigned long port, int value);
unsigned int	r_apic21_inpw(int fd, unsigned long port);
unsigned int	r_apic21_outpw(int fd, unsigned long port, unsigned int value);
unsigned int	r_apic21_inpd(int fd, unsigned long port);
unsigned int	r_apic21_outpd(int fd, unsigned long port, unsigned int value);

#define R_write_EEPROM_incy92	R_Apic21_putw_eeprom
#define R_read_EEPROM_incy92	R_Apic21_getw_eeprom

#ifdef __cplusplus
}
#endif

#endif	// R_APIC_H_
