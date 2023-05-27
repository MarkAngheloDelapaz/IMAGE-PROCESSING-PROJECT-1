#ifndef R_CS_IOSUBWRAPPER_H
#define R_CS_IOSUBWRAPPER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define COMMUNICATION_DR_CY_83		0
#define COMMUNICATION_DR_CY_80		1
#define COMMUNICATION_MEMORY1		2		// 2013/02/07 by sassa
#define COMMUNICATION_CUNET1		3		// 2013/02/25 by sassa
#define COMMUNICATION_CUNET_SHARE	4		// 2015/01/15 by araki
#define COMMUNICATION_GPIO_SHARE	5		// 2015/01/29 by araki
#define COMMUNICATION_2724			6		// 2724C用 2015/02/06 by kitamura
#define COMMUNICATION_LAN			7		// LAN用 2015/02/19 by kitamura
#define COMMUNICATION_DR_CY_140		8		// 2015/04/13 by kitamura

#define LED_CONTROLL_DR_X_124				0			//
#define LED_CONTROLL_DR_CY_83				1			// Max 100mA
#define LED_CONTROLL_DR_CY_80				1			//DR-CY-83と同様の処理を行う
#define LED_CONTROLL_DR_CY_95				2			// 2015/01/23 by kitamura Max 200mA


// STATION_TYPE_COMMON用 通信入力用構造体 kitamura
typedef struct {
	int						nStartBitStart;								// スタートビットが何ビット目か、1-で指定する。
	int						nInspectBitStart;							// 検査番号ビットの先頭位置							20170414 komatsu
	int						nInspectBitSize;							// 検査番号に何ビット使うか。						20170414 komatsu
	int						nRetryStartBitStart;						// リトライビットが何ビット目か。
	int						nWorkStateBitStart;							// 何ビット目からか、1-で指定する。Normal,NoWork,NGWork
	int						nWorkStateBitSize;							// 何ビット使うか。Normal,NoWork,NGWork
	int						nDirectionBitStart;							// 製品方向角度が何ビット目からか、1-で指定する。
	int						nDirectionBitSize;							// 製品方向角度が何ビット使うか。
	int						nColletNumberBitStart;						// コレット番号が何ビット目からか、1-で指定する。
	int						nColletNumberBitSize;						// コレット番号が何ビット使うか。
	int						nCounterClearBitStart;						// カウンタクリアが何ビット目からか、1-で指定する。
	int						nCounterClearBitSize;						// カウンタクリアが何ビット使うか。
	int						nRegistrationBitStart;						// 教示モード
	int						nRegistrationBitSize;						// 教示モード
	int						nByDirectionalDiodeBitStart;				// 双方向ダイオードモード
	int						nByDirectionalDiodeBitSize;					// 双方向ダイオードモード
	int						nAutoNozzleCheckBitStart;					// 1.63 2015/02/25 kitamura
	int						nAutoNozzleCheckBitSize;					// 1.63 2015/02/25 kitamura
	int						nManualStartBitStart;						// 1.63 2015/02/25 kitamura
	int						nManualStartBitSize;						// 1.63 2015/02/25 kitamura
	int						nLotStartBitStart;							// 1.90b 2015/10/02 kitamura
	int						nLotStartBitSize;							// 1.90b 2015/10/02 kitamura
	int						nFuncUserBitStart;
	int						nFuncUserBitSize;
	int						nCheckStartBit;								// スタートビットONの時の値
	int						nCheckRetryStartBit;						// RetryStartBitONの時の値
	int						nCheckWorkStateNormal;						//
	int						nCheckWorkStateNoWork;
	int						nCheckWorkStateNGWork;
	int						nCheckDirectionBit_0;
	int						nCheckDirectionBit_90;
	int						nCheckDirectionBit_180;
	int						nCheckDirectionBit_270;
	int						nCheckCounterClear;
	int						nCheckRegistration_0;
	int						nCheckRegistration_90;
	int						nCheckRegistration_180;
	int						nCheckRegistration_270;
	int						nCheckByDirectionalDiode;
	int						nCheckAutoNozzleCheck;						// 1.63 2015/02/25 kitamura
	int						nCheckManualStart;							// 1.63 2015/02/25 kitamura
	int						nCheckLotStart;								// 1.90b 2015/10/02 kitamur
} COMMUNICATION_INPUT;

// STATION_TYPE_COMMON用 通信出力用構造体 kitamura
typedef struct {
	int						nBusyBitStart;			// BusyBitが何ビット目か
	int						nLockBitStart;			// LockBitが何ビット目か
	int						nResultBitStart;		// GO/NGビットが何ビット目か
	int						nPresenceBitStart;		// Presenceビットが何ビット目か
	int						nBinBitStart;			// Binビットが何ビット目からか
	int						nBinBitSize;			// Binビットが何ビットか
	int						nBusyBitOn;				// BusyBitONの時の値
	int						nLockBitOn;				// LockBitONの時の値
	int						nResultBitPass;			// GOの時のBitの値
	int						nPresenceBitYes;		// PresenceBitがYESの時の値(NoWorkでない)
	int						nDirection_0;			// 方向結果を出力するときの0度の値
	int						nDirection_90;			// 						   90度の値
	int						nDirection_180;			// 						   180度の値
	int						nDirection_270;			// 						   270度の値
} COMMUNICATION_OUTPUT;

typedef struct{
	int						nShareMemorySizeCommu1;						// 共有メモリサイズ　通信用1		2013/02/07 by sassa
	int						nShareMemoryInAddrCommu1;					// 共有メモリINアドレス　通信用1		2013/02/07 by sassa
	int						nShareMemoryOutAddrCommu1;					// 共有メモリOUTアドレス　通信用1		2013/02/07 by sassa
}SHARDMEMORORY_PARA;

typedef struct{
	int						nCUnet1_StationAddressIn;					// CUnet通信1 IN  SA番号(0⇒63)			2013/02/25 by sassa
	int						nCUnet1_StationAddressOut;					// CUnet通信1 OUT SA番号(0⇒63)			2013/02/25 by sassa
	int						CUnetShareByte;								// 2016.11.14 by tanaka
	int						CUnetReadOffset;							// 2015.02.04 by araki
	int						CUnetWriteOffset;							// 2015.02.04 by araki
	int						CUnetWriteByte;
}CUNET_PARA;

typedef enum
{
	IOSUB_DEG0,
	IOSUB_DEG90,
	IOSUB_DEG180,
	IOSUB_DEG270,
	NUM_IOSUB_DIRECTION
} R_CS_IOSUB_DIRECTION;

typedef enum
{
	IOSUB_STATE_NORMAL,
	IOSUB_STATE_NOWORK,
	IOSUB_STATE_NGWORK,
	IOSUB_STATE_FUNCTION,
	NUM_IOSUB_STATE
} R_CS_IOSUB_STATE;


void R_CS_IoSubOpen(int mode, int sharedid, int startbit, int uselen);
void R_CS_IoSubGetDefaultSetting(COMMUNICATION_INPUT *input, COMMUNICATION_OUTPUT *output);
void R_CS_IoSubSetSetting(COMMUNICATION_INPUT input, COMMUNICATION_OUTPUT output);
int R_CS_IoSubSync();
int R_CS_IoSubGetStart();
int R_CS_IoSubGetWorkState();
int R_CS_IoSubGetFuncUser();
int R_CS_IoSubGetColletNo();
int R_CS_IosubIsCounterClear();


void R_CS_IoSubSetBinCode(int nData);
void R_CS_IoSubSetBusyOff(void);
void R_CS_IoSubSetBusyOn(void);
void R_CS_IoSubSetLockOff(void);
void R_CS_IoSubSetLockOn(void);
void R_CS_IoSubSetJudgeGo(void);
void R_CS_IoSubSetJudgeNg(void);
void R_CS_IoSubSetPresenceYes(void);
void R_CS_IoSubSetPresenceNo(void);

void R_CS_IoSubSetBit(int bit, int flag);

int R_CS_IoSubIsFunction();

#ifdef __cplusplus
}
#endif


#endif // R_CS_IOSUBWRAPPER_H
