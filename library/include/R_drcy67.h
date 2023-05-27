/******************************************************************************
	ＤＲ－ＣＹ－６７（汎用ディジタル入出力インターフェース）ライブラリー
	R_DRCY67.H / R_DRCY67.C
	
	変更履歴
	Ver 3.20	1998/08/31	release
	Ver 3.??	2000/09/??	gpioへの統合 DR-CY-67関係はR_gpio.hへ移植
	Ver 3.28	2001/02/06	MAX525対応
	Ver 3.50	2009/02/12	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_DRCY67_H_
#define	R_DRCY67_H_

#include "R_gpdac.h"

#define	DRCY67_chanel_0	0x00000
#define	DRCY67_chanel_1	0x01000
#define	DRCY67_chanel_2	0x02000
#define	DRCY67_chanel_3	0x03000
#define	DRCY67_chanel_4	0x04000
#define	DRCY67_chanel_5	0x05000
#define	DRCY67_chanel_6	0x06000
#define	DRCY67_chanel_7	0x07000
#define	DRCY67_chanel_8	0x08000
#define	DRCY67_chanel_9	0x09000
#define	DRCY67_chanel_A	0x0A000
#define	DRCY67_chanel_B	0x0B000
#define	DRCY67_chanel_C	0x0C000
#define	DRCY67_chanel_D	0x0D000
#define	DRCY67_chanel_E	0x0E000
#define	DRCY67_chanel_F	0x0F000

#define	RDCY67_DAC_DATA		1
#define	RDCY67_DAC_CLOCK	2
#define	RDCY67_DAC_LOAD		4
#define	RDCY67_ROM_DI		1
#define	RDCY67_ROM_DO		1
#define	RDCY67_ROM_CS		2
#define	RDCY67_ROM_SK		4

#define	DR67_DACDATA	0x01
#define	DR67_DACCLK		0x02
#define	DR67_DACLD		0x04
#define	DR67_DACGATE	0x10
#define	DR67_DACFULL	50

#include "R_gpio.h"

#endif	// R_DRCY67_H_
