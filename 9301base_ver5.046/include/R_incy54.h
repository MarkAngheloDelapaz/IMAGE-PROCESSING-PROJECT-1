/* *****************************************************************************************

		ＩＮ－ＣＹ－５４（汎用ディジタル入出力インターフェース）ライブラリー
		ＩＮ－ＣＹ－６２（汎用ディジタル入出力インターフェース）ライブラリー
		ＩＮ－ＣＹ－７３（汎用ディジタル入出力インターフェース）ライブラリー

	変更履歴
		Ver ?.??	1994/03/23	IN-CY-62 add
		Ver ?.??	1998/02/20	IN-CY-73 add
		Ver 3.??	2000/09/??	IN-CY-92追加、gpioへの統合 by shin
		Ver 3.28	2001/02/07	関数のOBJ化
		Ver 3.49	2006/12/21	C99対応(EOF削除など)		by matsu
		Ver 3.50	2009/02/12	DOS & LINUX ソース統合
******************************************************************************************* */

#ifndef	R_INCY54_H_
#define	R_INCY54_H_

// 基板ＩＤ設定
#define	INCY54_chanel_0	0x00000
#define	INCY54_chanel_1	0x01000
#define	INCY54_chanel_2	0x02000
#define	INCY54_chanel_3	0x03000
#define	INCY54_chanel_4	0x04000
#define	INCY54_chanel_5	0x05000
#define	INCY54_chanel_6	0x06000
#define	INCY54_chanel_7	0x07000
#define	INCY54_chanel_8	0x08000
#define	INCY54_chanel_9	0x09000
#define	INCY54_chanel_A	0x0A000
#define	INCY54_chanel_B	0x0B000
#define	INCY54_chanel_C	0x0C000
#define	INCY54_chanel_D	0x0D000
#define	INCY54_chanel_E	0x0E000
#define	INCY54_chanel_F	0x0F000

#define	INCY62_chanel_0	0x00000
#define	INCY62_chanel_1	0x01000
#define	INCY62_chanel_2	0x02000
#define	INCY62_chanel_3	0x03000
#define	INCY62_chanel_4	0x04000
#define	INCY62_chanel_5	0x05000
#define	INCY62_chanel_6	0x06000
#define	INCY62_chanel_7	0x07000
#define	INCY62_chanel_8	0x08000
#define	INCY62_chanel_9	0x09000
#define	INCY62_chanel_A	0x0A000
#define	INCY62_chanel_B	0x0B000
#define	INCY62_chanel_C	0x0C000
#define	INCY62_chanel_D	0x0D000
#define	INCY62_chanel_E	0x0E000
#define	INCY62_chanel_F	0x0F000

#define	INCY67_chanel_0	0x00000
#define	INCY67_chanel_1	0x01000
#define	INCY67_chanel_2	0x02000
#define	INCY67_chanel_3	0x03000
#define	INCY67_chanel_4	0x04000
#define	INCY67_chanel_5	0x05000
#define	INCY67_chanel_6	0x06000
#define	INCY67_chanel_7	0x07000
#define	INCY67_chanel_8	0x08000
#define	INCY67_chanel_9	0x09000
#define	INCY67_chanel_A	0x0A000
#define	INCY67_chanel_B	0x0B000
#define	INCY67_chanel_C	0x0C000
#define	INCY67_chanel_D	0x0D000
#define	INCY67_chanel_E	0x0E000
#define	INCY67_chanel_F	0x0F000

#define	DRCY64_chanel_0	0x00000
#define	DRCY64_chanel_1	0x01000
#define	DRCY64_chanel_2	0x02000
#define	DRCY64_chanel_3	0x03000
#define	DRCY64_chanel_4	0x04000
#define	DRCY64_chanel_5	0x05000
#define	DRCY64_chanel_6	0x06000
#define	DRCY64_chanel_7	0x07000
#define	DRCY64_chanel_8	0x08000
#define	DRCY64_chanel_9	0x09000
#define	DRCY64_chanel_A	0x0A000
#define	DRCY64_chanel_B	0x0B000
#define	DRCY64_chanel_C	0x0C000
#define	DRCY64_chanel_D	0x0D000
#define	DRCY64_chanel_E	0x0E000
#define	DRCY64_chanel_F	0x0F000

#define	DAC_DATA	1
#define	DAC_CLOCK	2
#define	DAC_LOAD	4
#define	ROM_DI		1
#define	ROM_DO		1
#define	ROM_CS		2
#define	ROM_SK		4

#define	INCY54_pin_1	0x010001
#define	INCY54_pin_2	0x010002
#define	INCY54_pin_3	0x010004
#define	INCY54_pin_4	0x010008
#define	INCY54_pin_5	0x010010
#define	INCY54_pin_6	0x010020
#define	INCY54_pin_7	0x010040
#define	INCY54_pin_8	0x010080
#define	INCY54_pin_9	0x010101

#define	INCY54_8255_1_A_port	0x0100
#define	INCY54_8255_1_B_port	0x0101
#define	INCY54_8255_1_C_port	0x0102
#define	INCY54_8255_1_control	0x0103
#define	INCY54_8255_2_A_port	0x0500
#define	INCY54_8255_2_B_port	0x0501
#define	INCY54_8255_2_C_port	0x0502
#define	INCY54_8255_2_control	0x0503
#define	INCY54_8255_3_A_port	0x0900
#define	INCY54_8255_3_B_port	0x0901
#define	INCY54_8255_3_C_port	0x0902
#define	INCY54_8255_3_control	0x0903
#define	INCY54_8255_4_A_port	0x0D00
#define	INCY54_8255_4_B_port	0x0D01
#define	INCY54_8255_4_C_port	0x0D02
#define	INCY54_8255_4_control	0x0D03

#define	INCY54_8255_1	0x0100
#define	INCY54_8255_2	0x0500
#define	INCY54_8255_3	0x0900
#define	INCY54_8255_4	0x0D00

#define R_read_EEPROM_drcy64	R_read_EEPROM_incy67
#define R_write_EEPROM_drcy64	R_write_EEPROM_incy67

int R_read_EEPROM_incy67 ( int channel, int address );
int R_write_EEPROM_incy67 ( int channel, int address, unsigned short int data );

#include "R_gpio.h"
#include "R_apic21.h"
#include "R_gpdac.h"

#endif	// R_INCY54_H_
