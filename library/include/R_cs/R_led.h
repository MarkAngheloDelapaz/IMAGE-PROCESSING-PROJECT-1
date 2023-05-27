/******************************************************************************

	LED調整関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version			name	comment
	2004.03.15.000	hirata	オリジナル
	2004.04.08.001	matsu	LedOn(),LedOff()追加
	2007.09.18.002	matsu	R_LedAdjustDemo系追加
	2007.09.21		matsu	R_LedAdjustのサンプリング表示にMODEとSTDEV追加
	2007.09.27		matsu	R_LedAdjustのレイアウト計算式変更(VGAカメラでは画面外に表示)
					Ver.1.07b1.0
	2007.10.26		matsu	以下の関数追加
						R_LedAdjustDemoCurrentLimitMaxSet/Get	// LED調整デモ電流最大値の設定、取得
						R_LedAdjustSingleModeSet/Get			// シングルモード設定、取得
						R_LedAdjustLoopTimerSet/Get				// ループタイマの設定、取得
					Ver.1.07b1.1
	2008.01.07		matsu	以下の関数追加
						R_LedAdjustSamplingModeSet/Get			// サンプリングモードの設定、取得

	2008.04.18		matsu	照明調整時の表示色の設定・取得関数追加
								R_LedAdjustDrawColorSet/Get追加
	2008.04.26		matsu	LED_CH*の宣言修正
	2008.05.28		matsu	R_LED_PARAのチャンネル設定をR_LED_ADJUSTと同じ形式に対応

	1.07b1.4	2008.08.07	matsu	R_LedAdjustSamplingAverageGet()追加
	1.07b1.7	2009.06.xx	matsu	ユーザボタン追加対応

******************************************************************************/

#ifndef __R_LED_H__
#define __R_LED_H__

#include "R_button.h"

#ifdef __cplusplus
extern "C"
{
#endif

// --- define ---
#define		LED_FLAG		0x80000000			// 追加  2008.04.26
#define		LED_CH1			(0x01|LED_FLAG)		// DacChannel1	0x00000001 → 0x80000001  宣言修正  2008.04.26
#define		LED_CH2			(0x02|LED_FLAG)		// DacChannel2	0x00000002 → 0x80000002
#define		LED_CH3			(0x04|LED_FLAG)		// DacChannel3	0x00000004 → 0x80000004
#define		LED_CH4			(0x08|LED_FLAG)		// DacChannel4	0x00000008 → 0x80000008
#define		LED_CH5			(0x10|LED_FLAG)		// DacChannel5	0x00000010 → 0x80000010
#define		LED_CH6			(0x20|LED_FLAG)		// DacChannel6	0x00000020 → 0x80000020
#define		LED_CH7			(0x40|LED_FLAG)		// DacChannel7	0x00000040 → 0x80000040
#define		LED_CH8			(0x80|LED_FLAG)		// DacChannel8	0x00000080 → 0x80000080

#define		LIGHT_MAX		10				// 制御する最大照明数

typedef struct {
	int channel;				//定電流基板のチャネル(0～7)
	int *data;					//出力する電流値[uA](0～100,000)
} LED_PARA_ITEM;

typedef struct {
	int port;					//定電流基板のRDSWの番号
	LED_PARA_ITEM item[8];		//定電流基板の各チャネルの設定
} R_LED_PARA;

//　定電流基板出力パラメータ設定構造体
typedef struct {
	char	*name_j;		//	チャンネル名称（日本語）
	char	*name_e;		//	チャンネル名称（英語）
	int		ch;				//	変更を加えたいＤＡＣのチャンネルを指定（複数の場合ORすること）
							//	値		定数			意味（定電流基板のDACチャンネル）
							//	───────────────────────────────────
							//	0x01	LED_CH1			0
							//	0x02	LED_CH2			1
							//	0x04	LED_CH3			2
							//	0x08	LED_CH4			3
							//	0x10	LED_CH5			4
							//	0x20	LED_CH6			5
							//	0x40	LED_CH7			6
							//	0x80	LED_CH8			7
	int		*current;		//	チャンネルの出力電流値
	int		min_current;	//	電流最小値
	int		max_current;	//	電流最大値
	int		step;			//	電流値変更時の最小ステップ数
}R_LED_ADJUST_ITEM;

//　定電流基板出力パラメータ設定構造体
typedef struct {
//	int		board_channel;	//	定電流基板のDSW位置とｱﾄﾞﾚｽのOR
	char	*title_j;		//	タイトル名称（日本語）
	char	*title_e;		//	タイトル名称（英語）
	void	(*func)(int mode);
							//	オプション機能実行関数（本関数にない動作を行う場合は、
							//  この関数内で設定する）
							//	注１）画像取込時に照明毎にパラメータを変更したい場合、
							//	引き数modeで切り替えられるように予め関数を作成する。
							//	但し、mode　は、R_LED_ADJUST_ITEM　light[]　に対応させること。
							//	（mode=0は、light[0]での画像取込時の条件）
							//	例えば、ランダムシャター時間を照明毎に設定する場合は、
							//		void func(int mode)
							//		{
							//			switch( mode ){
							//				case 0: R_video_set_shutter_speed(4000);	break;
							//				case 1: R_video_set_shutter_speed(10000);	break;
							//		}
							//			R_change_camera_mode( STANDBY|XCHR50);
							//			R_get_video( SHUTTER|XCHR50, GRAY_PLAHE);
							//		}
	void	(*end_func)(int mode);
							//	オプション機能終了実行関数（上記のfunc関数とLED OFF後に動作を
							//	行う場合は、この関数内で設定する。
							//	例	露光終了後、ＬＥＤをＯＦＦしてから取込終了待ちを行う場合
							//	void func(int mode){
							//		R_get_video_auto(SHUTTER|XCHR50, GRAY_PLAHE);
							//	}
							//	
							//	void end_func(int mode){
							//		for(;;){
							//			if(OK==R_get_video_auto_end_check())	break;
							//		}
							//	}
	R_LED_ADJUST_ITEM	light[LIGHT_MAX];
							//	照明調整パラメータ構造体
	int		*camera;		//	カメラ構造体（ダミー）
} R_LED_ADJUST;

// --- variable ---

// --- prototype ---
int R_LedOpen(int channel);
int R_LedOn(R_LED_PARA *para);
int R_LedOff(R_LED_PARA *para);
int R_LedAdjust( int channel, R_LED_ADJUST *led);
int R_LedAdjustDemo(void);
int R_LedAdjustDemoFunctionSet(void (*func)(int mode), void (*end_func)(int mode));
int R_LedAdjustDemoMenu(void);
int R_LedAdjustDemoRdswSet(int channel);
int R_LedAdjustDemoRdswGet(void);
int R_LedAdjustDemoChannelDataSet(int num, int *pGroupList, int *pCurrentList);
int R_LedAdjustDemoChannelDataGet(int *num, int *pGroupList, int *pCurrentList);
int R_LedAdjustDemoCurrentLimitMaxSet(int ch, int value);		// LED調整デモ電流最大値の設定  2007.10.26  by matsu
int R_LedAdjustDemoCurrentLimitMaxGet(int ch);					// LED調整デモ電流最大値の取得  "

int R_LedAdjustSamplingModeSet(int nMode);						// サンプリングモードの設定  2008.01.07  by matsu
int R_LedAdjustSamplingModeGet(void);							// サンプリングモードの取得  "
int R_LedAdjustSamplingAreaSet(R_RECT *area, int num);			// サンプリング領域の設定
int R_LedAdjustSamplingAreaGet(R_RECT *area, int *num);			// サンプリング領域の取得
int R_LedAdjustSamplingAreaNumberMaxSet(int num);				// サンプリング領域最大数の設定  2007.04.20  by matsu
int R_LedAdjustSamplingAreaNumberMaxGet(void);					// サンプリング領域最大数の取得  "
#define R_LedAdjustSingleMode R_LedAdjustSingleModeSet			// 関数名変更  2007.10.26  by matsu
int R_LedAdjustSingleModeSet(int mode);							// シングルモードの設定  追加  2007.10.26  by matsu
int R_LedAdjustSingleModeGet(void);								// シングルモードの取得  "
int R_LedAdjustSamplingAverageGet(int *pNum, int *pAvg);		//<1.07b1.4> サンプリング領域平均明るさの取得

int R_LedAdjustUserButtonSet(struct button_t *pUserBtn);		//<1.07b1.7> 
int R_LedAdjustUserButtonGet(void);								//<1.07b1.7> 

int R_LedAdjustFocusMode(int mode);
int R_LedAdjustInitiatorPageSet(int nPage);
int R_LedAdjustInitiatorPageGet(void);
int R_LedAdjustLoopTimerSet(int time);							// ループタイマの設定  2007.10.26  by matsu
int R_LedAdjustLoopTimerGet(void);								// ループタイマの取得  "

int R_LedAdjustDrawColorSet(int nColor);						// 照明調整時の表示色の設定  2008.04.18
int R_LedAdjustDrawColorGet(void);								// 照明調整時の表示色の取得  2008.04.18

int R_LedAdjustGetCurrent(int no);								// 外部から調整中の電流値を取得する		2013.04.10		komatsu
void R_LedAdjustUserLight(int mode);							// ユーザ手設定の発光規則を使用する		2013.04.10		komatsu

#endif

#ifdef __cplusplus
}
#endif
