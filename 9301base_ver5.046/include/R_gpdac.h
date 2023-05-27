/******************************************************************************
	R_GPDAC.H / R_GPDAC.C
	
	変更履歴
		Ver 3.28	2001/02/07	正式リリース
		Ver 3.30	2002/01/10	DR-CY-82、DR-CY-83対応
		Ver 3.32	2002/09/18	DR-CY-83A/B対応、複数枚対応
		Ver 3.34	2002/12/24	DR-CY-80対応
		Ver 3.34	2003/03/10	r_drcy83_dac_open内部関数化
		Ver 3.35	2003/04/7	R_gpdac_set_cc,R_gpdac_open追加
		Ver 3.35	2003/04/7	R_set_DAC_drcy88追加
		Ver 3.36	2003/05/13	DR-CY-80A対応
		Ver 3.39	2003/06/24	R_gpdac_check_board追加
		Ver 3.46	2004/08/08	DR-CY-101対応
		Ver 3.50	2009/02/12	DOS & LINUX ソース統合
		Ver 4.01	2014/06/05	DR-CY-140対応
		Ver 4.01	2014/08/25	R_set_DAC_drcy140関数追加
		Ver 4.01	2015/09/29	DR-CY-143 DOS対応
		Ver 5.042	2021/03/02	R_gpdac_disable_bias_shifting関数追加
******************************************************************************/

#ifndef	R_GPDAC_H_
#define	R_GPDAC_H_

#ifdef __cplusplus
extern "C" {
#endif

struct gpdac_info_t {		//定電流基板の情報
	int board_type;			//基板の種類
	int revision;			//リビジョン
	int channel;			//チャンネル数
	int current_max[8];		//各チャンネルの最大電流値[uA]
	int voltage_max[8];		//各チャンネルの最大電圧値[mV]
};


int R_gpdac_set_current(int channel,int DacChannel,int data);
int R_gpdac_open(int channel);
int R_gpdac_check_board(int channel);
int R_gpdac_get_info(int channel, struct gpdac_info_t *info );
int R_set_DAC_drcy64(int channel, int ch, int data);
int R_set_DAC_incy67( int nChannel, int nCh, int nData );
int R_set_DAC_drcy82(int channel, int ch, int data);		//shin DRCY82 2001/7/12
void R_set_DAC_drcy67(int channel, int nData0, int nData1, int nData2, int nData3);
void R_set_DAC_drcy80(int channel, int nData0, int nData1, int nData2, int nData3);
void R_set_DAC_drcy83(int channel,int DacChannel,int data);		//shin DRCY83 00/09/20
int R_set_DAC_drcy88(int channel,int DacChannel,int data);
int R_set_DAC_drcy101(int channel,int DacChannel,int data);
int	R_set_DAC_drcy140(int channel, int DacChannel, int data);	// 2014.08.25
int CheckDacDRCY83( int nChannel );
int InitialDacTLV5630(int nChannel);

int	R_gpdac_get_supply_voltage(int channel, int dac);
int	R_gpdac_get_force_voltage(int channel, int dac);
int	R_gpdac_get_temperature(int channel, int dac);

int	R_gpdac_make_own_dacs(int channel, int dacs);

// D/Aコンバータのチャンネル数を取得する。
// 開発中のため未対応基板有り
// 戻り値	0以上	チャンネル数。DACを搭載していないI/O基板は0を返す。
//			-1		基板なし、または未対応基板
/*!
 * @brief R_gpdac_channels_of_dac@n
 *			channels of D/A converter
 * @param	channel of board (ex._BOARD_RDSW_0)
 * @retval	channels@n
 *			0	no dac channel
 *			0<	channels of dac
 *			<0	no board, or the board is not supported in this function.
 */
int	R_gpdac_channels_of_dac(int channel);

/*!
 * @brief R_gpdac_disable_bias_shifting@n
 *			disable bias-shifting for current zero
 * @param	channel of board (ex._BOARD_RDSW_0)
 * @retval	OK
 *			ERROR	no board or not DR-CY-80 board
 *	定電流出力回路のBIASシフト機能を無効にする。
 *	2011年以前に製作されたDR-CY-80A/92の中には、R_gpdac_set_currentで0を設定してもLEDが完全に消えない物がある。
 *	ベースライブラリVer.5.042以降ではBIAS調整用のD/Aコンバータを制御して、0設定時の漏れ電流を防ぐ機能が追加されている。
 *	ただしDAC制御に30～50usかかるため、この機能を無効にしたい場合はこの関数を使用する。
 *	DR-CY-80A/92よりも古いDR-CY-80無印にはDACが搭載されていないためBIASシフト機能は動作しません。
 */
int	R_gpdac_disable_bias_shifting(int channel);

#define	R_set_DAC_drcy82		R_set_DAC_drcy64

#ifdef __cplusplus
}
#endif

#endif	// R_GPDAC_H_
