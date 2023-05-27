/******************************************************************************
	R_GPIO.H / R_GPIO.C
	
	変更履歴
	Ver 3.10	1999/07/30	release
	Ver 3.??	2000/09/??	R_incy54.cの包括。INCY92対応
	Ver 3.??	2000/11/21	大幅追加 GPIOへの統一化 APICEEPROM 対応
	Ver 3.28	2001/02/07	関数のOBJ化
	Ver 3.28	2001/03/08	GPIO構造の見直し
	Ver 3.30	2002/01/10	DR-CY-82,DR-CY-83対応
	Ver 3.32	2002/09/18	DR-CY-83A/B対応、複数枚対応
	Ver 3.35	2003/02/20	DR-CY-88対応
	Ver 3.35	2003/04/07	R_gpio_open_pciにDAC初期化追加
	Ver 3.42	2004/04/06	DR-CY-95対応
	Ver 3.43	2004/06/11	R_gpio_read_output_status追加
	Ver 3.46	2005/08/04	DR-CY-101対応
	Ver 3.49	2007/02/19	DR-CY-101-2/3/4対応
	Ver 3.49	2007/02/22	DR-CY-105対応
	Ver 3.50	2008/08/19	R_gpio_open、ISAバス存在確認
	Ver 3.50	2009/02/12	DOS & LINUX ソース統合
	Ver 4.01	2009/08/10	R_gpio_board_type関数、_BOARD_RDSW()マクロ追加
	Ver 4.01	2010/03/26	DR-CY-83マルチプロセス暫定対応
	Ver 4.01	2010/05/11	IN-CY-76マルチプロセス暫定対応
	Ver 4.01	2012/07/31	DR-CY-101マルチプロセス暫定対応
	Ver 4.01	2013/02/08	DR-CY-95マルチプロセス対応
	Ver 4.01	2014/04/22	DR-CY-140対応
	Ver 4.01	2015/06/29	-io0=V コマンド暫定対応
	Ver 4.01	2015/08/24	GPIOデバイスドライバ対応バグフィクス
	Ver 4.01	2015/09/15	PCI-2724C Rev16対応
	Ver 4.01	2015/09/25	IN-CY-119対応
	Ver 4.01	2015/12/21	IN-CY-122対応
	Ver 4.01	2016/01/13	IN-CY-122対応バグフィクス
	Ver 4.01	2016/05/31	r_gpio_inpバグフィクス
	Ver 4.01	2017/01/12	DR-CY-80マルチプロセス時DACバグ修正
	Ver 4.01	2017/05/23	R_gpio_set_writable_64bits、対応ドライババージョン修正
******************************************************************************/

#ifndef	R_GPIO_H_
#define	R_GPIO_H_

#include <stdint.h>

#include "R_incy54.h"
#include "R_drcy67.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _BOARD_RDSW_0	0x00000
#define _BOARD_RDSW_1	0x01000
#define _BOARD_RDSW_2	0x02000
#define _BOARD_RDSW_3	0x03000
#define _BOARD_RDSW_4	0x04000
#define _BOARD_RDSW_5	0x05000
#define _BOARD_RDSW_6	0x06000
#define _BOARD_RDSW_7	0x07000
#define _BOARD_RDSW_8	0x08000
#define _BOARD_RDSW_9	0x09000
#define _BOARD_RDSW_A	0x0A000
#define _BOARD_RDSW_B	0x0B000
#define _BOARD_RDSW_C	0x0C000
#define _BOARD_RDSW_D	0x0D000
#define _BOARD_RDSW_E	0x0E000
#define _BOARD_RDSW_F	0x0F000
#define _BOARD_RDSW(sw)	((sw)<<12)	// 2009/08/10

#define	PCI_GPIO_INCY76		1
#define	PCI_GPIO_INCY92		2
#define	PCI_GPIO_DRCY80		3
#define	PCI_GPIO_DRCY82		4		//shin
#define	PCI_GPIO_DRCY83		5		//shin 01/09/19
#define	PCI_GPIO_DRCY88		6		//shinohara 03/01/30
#define	PCI_GPIO_DRCY95		7		//ohshima
#define	PCI_GPIO_DRCY101	8		//ohshima	05/08/04
#define	PCI_GPIO_DRCY105	9		//tanaka	07/02/22
#define	PCI_GPIO_DRCY140	10		// 2014.04.21
#define	PCI_GPIO_DRCY143	11		// 2014.11.26
#define	PCI_GPIO_PCI2724C	12		// 2014.11.26
#define	PCI_GPIO_INCY119	13		// 2015.09.25
#define	PCI_GPIO_INCY122	14		// 2015.12.18

//基板ＰＯＲＴ設定 
#define	CNa_IN_port_1	0x0100
#define	CNa_IN_port_2	0x0101
#define	CNa_IN_port_3	0x0102
#define	CNa_IN_port_4	0x0103	//shinohara	03/01/30
#define	CNa_IN_port_5	0x0104	// 2015.12.21 for IN-CY-122
#define	CNa_OUT_port_1	0x0500
#define	CNa_OUT_port_2	0x0501
#define	CNa_OUT_port_3	0x0502
#define	CNa_OUT_port_4	0x0503	// 2014.11.26 for PCI-2724C/CL/CM
#define	CNa_OUT_port_5	0x0504	// 2015.12.21 for IN-CY-122
#define	CNb_IN_port_1	0x0100
#define	CNb_IN_port_2	0x0101
#define	CNb_IN_port_3	0x0102
#define	CNb_OUT_port_1	0x0500
#define	CNb_OUT_port_2	0x0501
#define	CNb_OUT_port_3	0x0502
#define	CNc_IN_port_1	0x0900
#define	CNc_IN_port_2	0x0901
#define	CNc_IN_port_3	0x0902
#define	CNc_OUT_port_1	0x0D00
#define	CNc_OUT_port_2	0x0D01
#define	CNc_OUT_port_3	0x0D02

extern int	nGpioPort[16];
extern int	nGpromPort[16];

int		RIncy76Open(int i, int j);
int		RIncy92Open(int i, int j);
int		RDrcy80Open(int i, int j);
int		RDrcy82Open(int i, int j);		//shin
int		RDrcy83Open(int i, int j);		//shin
int		RDrcy95Open(int i, int j);		//shin
int		RDrcy101Open(int i, int j);		
int		RDrcy105Open(int i, int j);		

int		R_gpio_open(int nChannel);
int		R_gpio_open_isa(int nChannel);
int		R_gpio_open_pci(int nChannel);
int		R_gpio_board_type(int channel);
int		R_gpio_get_address(int nChannel);
int		R_gpio_write(int channel, int data);
int		R_gpio_read(int channel);
int		R_gpio_write16(int channel, int data);
int		R_gpio_read16(int channel);
int		R_gpio_write32(int channel, int data);
int		R_gpio_read32(int channel);
void	R_gpio_on(int channel, int bit);
void	R_gpio_off(int channel, int bit);
int		R_gpio_read_output_status(int channel);	//	@K@
int		R_gpio_set_writable_bits(int channel, int bits);
int		R_gpio_set_writable_64bits(int channel, uint64_t bits, uint64_t* conflict_bits);

int		R_gpio_ports_of_inp(int channel);
int		R_gpio_bits_of_inp(int channel, int port);
int		R_gpio_ports_of_outp(int channel);
int		R_gpio_bits_of_outp(int channel, int port);
const char*	R_gpio_board_name(int channel);

int	R_gpio_count_of_drcy140(void);
int	R_gpio_count_of_drcy143(void);
int	R_gpio_count_of_incy119(void);
int	R_gpio_count_of_incy122(void);

int R_open_incy54 ( int channel ) ;

int R_open_incy67 ( int channel );
int R_read_incy67 ( int channel ) ;
int R_write_incy67 ( int channel, int data ) ;
void R_on_incy67( int channel, int bit ) ;
void R_off_incy67( int channel, int bit ) ;

int R_open_drcy64 ( int channel );

// 内部関数
int		R_gpio_is_available(int nID);
int		r_gpio_virtual_channel(int physical_channel);
int		r_gpio_physical_channel(int virtual_channel);
unsigned long	r_gpio_base_port(int id);
#ifdef __DOS__
int		r_gpio_print_info(int, int, int, int);
#endif

#define R_open_incy62			R_gpio_open

#define R_write_incy54			R_gpio_write
#define R_read_incy54			R_gpio_read
#define R_on_incy54				R_gpio_on
#define R_off_incy54			R_gpio_off

#define R_write_incy62			R_gpio_write
#define R_read_incy62			R_gpio_read
#define R_on_incy62				R_gpio_on
#define R_off_incy62			R_gpio_off

#define	R_open_incy73			R_open_incy62
#define R_read_incy73			R_gpio_read
#define R_on_incy73				R_gpio_on
#define R_off_incy73			R_gpio_off

#define R_open_incy76			R_gpio_open
#define R_write_incy76			R_gpio_write
#define R_read_incy76			R_gpio_read
#define R_on_incy76				R_gpio_on
#define R_off_incy76			R_gpio_off

#define	R_open_drcy67			R_open_incy62
#define	R_read_drcy67			R_gpio_read
#define	R_write_drcy67			R_gpio_write
#define	R_on_drcy67				R_gpio_on
#define	R_off_drcy67			R_gpio_off

#define R_open_incy92			RIncy92Open
#define R_read_incy92  			R_read_incy67
#define R_write_incy92 			R_write_incy67
#define R_on_incy92				R_on_incy67
#define R_off_incy92			R_off_incy67

#ifdef __GNUC__
void	r_gpio_init_channel(void);
int	r_gpio_regist_channel(int virtual_channel, int physical_channel);
int	r_gpio_regist_share_mode(int channel);
int	r_gpio_virtual_channel(int physical_channel);
int	r_gpio_physical_channel(int virtual_channel);
int	RGpioOpenAll(int num, int multi);
int	r_gpio_device_driver_installed(int bus_no, int device_no);
#endif

int	r_gpio_rdsw_conflicted(int rdsw, int bus, int device);
void	r_gpio_debug_print(void);

#ifdef __cplusplus
}
#endif

#endif	// R_GPIO_H_
