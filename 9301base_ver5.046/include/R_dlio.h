/******************************************************************************
	R_dlio.h / R_dlio.c
	
	変更履歴
	Ver 3.46	2005/06/20	正式リリース
	Ver 3.50	2009/05/29	r_dlio_reg_port関数追加
	Ver 4.01	2011/04/05	IN-CY-116対応
	Ver 5.030	2020/06/22	PCI基板検索対象のバス番号最大値を修正
	Ver 5.045	2021/05/19	R_dlio_board_type関数追加
******************************************************************************/

#ifndef	R_DLIO_H_
#define	R_DLIO_H_

#include "R_dlcom.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	DLIO_MAX_BOARD	(int)8

// PORT ADDRESS
#define	DLIO_OUTPORT_D0	(int)0	// 差動出力データポート（８ビット）
#define	DLIO_OUTPORT_D1	(int)1	// 差動出力コントロールポート（４ビット）
#define	DLIO_OUTPORT_G0	(int)2	// 汎用出力ポート（下位８ビット）
#define	DLIO_OUTPORT_G1	(int)3	// 汎用出力ポート（上位４ビット）
#define	DLIO_INPORT_D0	(int)0	// 差動入力データポート（８ビット）
#define	DLIO_INPORT_D1	(int)1	// 差動入力コントロールポート（４ビット）
#define	DLIO_INPORT_G0	(int)2	// 汎用入力ポート（下位８ビット）
#define	DLIO_INPORT_G1	(int)3	// 汎用入力ポート（上位４ビット）

int	R_dlio_open(int nID);
int	R_dlio_outp(int nHandle, int nPort, int nData);
int	R_dlio_inp(int nHandle, int nPort);
int	R_dlio_bit_on(int nHandle, int nPort, int nBit);
int	R_dlio_bit_off(int nHandle, int nPort, int nBit);
int	R_dlio_outpw(int nHandle, int nPort, int nData);
int	R_dlio_inpw(int nHandle, int nPort);




/*!
 * @brief R_dlio_board_type
 *			Installed board type
 * @param	handle
 * @retval	Board Type ID
 *			DEVICE_ID_INCY84	IN-CY-84 is installed.
 *			DEVICE_ID_INCY116	IN-CY-116 is installed.
 *			0	error	(Not opend, Invalid handle ...)
 */

int	R_dlio_board_type(int nHandle);

unsigned long	r_dlio_reg_port(int nHandle);
int	RIncy84Open(int nBusNo, int nDeviceNo);

#ifdef __GNUC__
void	r_dlio_init_channel(void);
int	r_dlio_regist_channel(int virtual_channel, int physical_channel);
int	r_dlio_virtual_channel(int physical_channel);
int	r_dlio_physical_channel(int virtual_channel);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_DLIO_H_
