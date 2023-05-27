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
#define COMMUNICATION_2724			6		// 2724C�p 2015/02/06 by kitamura
#define COMMUNICATION_LAN			7		// LAN�p 2015/02/19 by kitamura
#define COMMUNICATION_DR_CY_140		8		// 2015/04/13 by kitamura

#define LED_CONTROLL_DR_X_124				0			//
#define LED_CONTROLL_DR_CY_83				1			// Max 100mA
#define LED_CONTROLL_DR_CY_80				1			//DR-CY-83�Ɠ��l�̏������s��
#define LED_CONTROLL_DR_CY_95				2			// 2015/01/23 by kitamura Max 200mA


// STATION_TYPE_COMMON�p �ʐM���͗p�\���� kitamura
typedef struct {
	int						nStartBitStart;								// �X�^�[�g�r�b�g�����r�b�g�ڂ��A1-�Ŏw�肷��B
	int						nInspectBitStart;							// �����ԍ��r�b�g�̐擪�ʒu							20170414 komatsu
	int						nInspectBitSize;							// �����ԍ��ɉ��r�b�g�g�����B						20170414 komatsu
	int						nRetryStartBitStart;						// ���g���C�r�b�g�����r�b�g�ڂ��B
	int						nWorkStateBitStart;							// ���r�b�g�ڂ��炩�A1-�Ŏw�肷��BNormal,NoWork,NGWork
	int						nWorkStateBitSize;							// ���r�b�g�g�����BNormal,NoWork,NGWork
	int						nDirectionBitStart;							// ���i�����p�x�����r�b�g�ڂ��炩�A1-�Ŏw�肷��B
	int						nDirectionBitSize;							// ���i�����p�x�����r�b�g�g�����B
	int						nColletNumberBitStart;						// �R���b�g�ԍ������r�b�g�ڂ��炩�A1-�Ŏw�肷��B
	int						nColletNumberBitSize;						// �R���b�g�ԍ������r�b�g�g�����B
	int						nCounterClearBitStart;						// �J�E���^�N���A�����r�b�g�ڂ��炩�A1-�Ŏw�肷��B
	int						nCounterClearBitSize;						// �J�E���^�N���A�����r�b�g�g�����B
	int						nRegistrationBitStart;						// �������[�h
	int						nRegistrationBitSize;						// �������[�h
	int						nByDirectionalDiodeBitStart;				// �o�����_�C�I�[�h���[�h
	int						nByDirectionalDiodeBitSize;					// �o�����_�C�I�[�h���[�h
	int						nAutoNozzleCheckBitStart;					// 1.63 2015/02/25 kitamura
	int						nAutoNozzleCheckBitSize;					// 1.63 2015/02/25 kitamura
	int						nManualStartBitStart;						// 1.63 2015/02/25 kitamura
	int						nManualStartBitSize;						// 1.63 2015/02/25 kitamura
	int						nLotStartBitStart;							// 1.90b 2015/10/02 kitamura
	int						nLotStartBitSize;							// 1.90b 2015/10/02 kitamura
	int						nFuncUserBitStart;
	int						nFuncUserBitSize;
	int						nCheckStartBit;								// �X�^�[�g�r�b�gON�̎��̒l
	int						nCheckRetryStartBit;						// RetryStartBitON�̎��̒l
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

// STATION_TYPE_COMMON�p �ʐM�o�͗p�\���� kitamura
typedef struct {
	int						nBusyBitStart;			// BusyBit�����r�b�g�ڂ�
	int						nLockBitStart;			// LockBit�����r�b�g�ڂ�
	int						nResultBitStart;		// GO/NG�r�b�g�����r�b�g�ڂ�
	int						nPresenceBitStart;		// Presence�r�b�g�����r�b�g�ڂ�
	int						nBinBitStart;			// Bin�r�b�g�����r�b�g�ڂ��炩
	int						nBinBitSize;			// Bin�r�b�g�����r�b�g��
	int						nBusyBitOn;				// BusyBitON�̎��̒l
	int						nLockBitOn;				// LockBitON�̎��̒l
	int						nResultBitPass;			// GO�̎���Bit�̒l
	int						nPresenceBitYes;		// PresenceBit��YES�̎��̒l(NoWork�łȂ�)
	int						nDirection_0;			// �������ʂ��o�͂���Ƃ���0�x�̒l
	int						nDirection_90;			// 						   90�x�̒l
	int						nDirection_180;			// 						   180�x�̒l
	int						nDirection_270;			// 						   270�x�̒l
} COMMUNICATION_OUTPUT;

typedef struct{
	int						nShareMemorySizeCommu1;						// ���L�������T�C�Y�@�ʐM�p1		2013/02/07 by sassa
	int						nShareMemoryInAddrCommu1;					// ���L������IN�A�h���X�@�ʐM�p1		2013/02/07 by sassa
	int						nShareMemoryOutAddrCommu1;					// ���L������OUT�A�h���X�@�ʐM�p1		2013/02/07 by sassa
}SHARDMEMORORY_PARA;

typedef struct{
	int						nCUnet1_StationAddressIn;					// CUnet�ʐM1 IN  SA�ԍ�(0��63)			2013/02/25 by sassa
	int						nCUnet1_StationAddressOut;					// CUnet�ʐM1 OUT SA�ԍ�(0��63)			2013/02/25 by sassa
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
