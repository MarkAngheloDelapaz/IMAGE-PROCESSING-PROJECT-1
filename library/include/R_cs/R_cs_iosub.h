#ifndef R_CS_IOSUB_H
#define R_CS_IOSUB_H

#include <inttypes.h>

#include "R_cs_iosubwrapper.h"
#include "R_lxsys.h"

class R_cs_iosub
{
protected:
#define SHARE_MEMORY_OPEN_WAIT		3000000								// 通信用1　共有メモリオープン時の待機時間(限界時間)[μsec]
#define SHARE_MEMORY_ID_COMMU1		'c'									// 通信用1　共有メモリID
#define SHARE_MEMORY_KEYFILE_COMMU1	"/home/ipg/user/key_commu1.txt"		// 通信用1　共有メモリ 識別用ファイル名
#define MEMORY_COMMU1_WORK_NUMBER	8									// １回の通信で扱うワークの最大個数

	COMMUNICATION_INPUT _tInputPara;
	COMMUNICATION_OUTPUT _tOutputPara;
	int64_t _data;
	int32_t _boardCh;
	int _comMode;
	int _sharedID;
	int _sharedStartBit;
	int _sharedBitLen;
	int32_t _gpioInPort;
	int32_t _gpioOutPort;
	static const int nMaxBitNumber = 64;							// 入出力ビット最大値

	SHARDMEMORORY_PARA _shMemPara;
	R_LXSYS_SHM_STRUCT	ShareMemoryStructCommu1;					// 通信用1　共有メモリ構造体
	char* ShareMemoryAddrIn_Commu1;									// 通信用1　共有メモリINアドレス
	char* ShareMemoryAddrOut_Commu1;								// 通信用1　共有メモリOUTアドレス
	int ShareMemorySynchronism_Commu1;								// 通信用1　同期処理(通信ソフトとのハンドシェイク)の有無	2013/07/08 by sassa

	CUNET_PARA _cunetPara;
	int CUnetReadOffset;
	int CUnetWriteOffset;


	bool _isOpen;

public:
	explicit R_cs_iosub(int CommunicationMode, int BoardCh, int LedControllMode=0, int CommunicationDeviceShareId=-1, int CommunicationDeviceStartBit=-1, int CommunicationDeviceUseLen=0);
	explicit R_cs_iosub(SHARDMEMORORY_PARA para);
	explicit R_cs_iosub(int CommunicationMode, int BoardCh, CUNET_PARA para);

public:
	int64_t InputByte(void);
	int64_t GetStart(void);
	int64_t GetRetryStart(void);
	int64_t GetColletNumber(void);
	int64_t GetDirection(void);
	int64_t GetWorkState(void);
	int64_t GetInspectNumbar(void);
	int64_t GetCounterClear(void);
	int64_t GetRegistration(void);
	int64_t GetByDirectionalDiode(void);
	int64_t GetAutoNozzleCheck(void);
	int64_t GetManualStart(void);
	int64_t GetLotStart(void);
	int64_t GetBit(int nBit, int nSize);
	int64_t GetFuncUser(void);
	int64_t GetData(void);

	void OutputBinCode(int nData);
	void OutputBusyOff(void);
	void OutputBusyOn(void);
	void OutputLockOff(void);
	void OutputLockOn(void);
	void OutputJudgeGo(void);
	void OutputJudgeNg(void);
	void OutputPresenceYes(void);
	void OutputPresenceNo(void);

	int SetBit(int nBit, int nFlag);

	static void GetDefaultSetting(COMMUNICATION_INPUT *input, COMMUNICATION_OUTPUT *output);
	void SetCommunicatePara(COMMUNICATION_INPUT input, COMMUNICATION_OUTPUT output);

	bool IsOpen;
};

#endif // R_CS_IOSUB_H
