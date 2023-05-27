/******************************************************************************

	R_CS_CNT.H
	
	カウンタ処理関数ヘッダ
	counter.c/counter.h
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kazuya Matsunaga
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN
	履歴
	2004.03.23	Ver.0.01			新規作成
	2007.09.04	Ver.0.02	matsu	COUNTER_CLASSxx宣言に 10～19追加

******************************************************************************/
#ifndef __R_CS_CNT_H__
#define __R_CS_CNT_H__

// --- define ---

// クラス分類
#define COUNTER_CLASS0		0
#define COUNTER_CLASS1		1
#define COUNTER_CLASS2		2
#define COUNTER_CLASS3		3
#define COUNTER_CLASS4		4
#define COUNTER_CLASS5		5
#define COUNTER_CLASS6		6
#define COUNTER_CLASS7		7
#define COUNTER_CLASS8		8
#define COUNTER_CLASS9		9
#define COUNTER_CLASS10		10		// 10～19追加  2007.09.04  by matsu
#define COUNTER_CLASS11		11
#define COUNTER_CLASS12		12
#define COUNTER_CLASS13		13
#define COUNTER_CLASS14		14
#define COUNTER_CLASS15		15
#define COUNTER_CLASS16		16
#define COUNTER_CLASS17		17
#define COUNTER_CLASS18		18
#define COUNTER_CLASS19		19

//クラス分類にORするフラグ
#define COUNTER_NO_INC	0x10000000

// カウンタデータ取得モード
#define COUNTER_NUMBER		0x10000000
#define COUNTER_NAME_JP		0x00000000
#define COUNTER_NAME_EN		0x00000001

#ifdef __linux__
#include "R_alarm.h"
#define	R_CS_COUNTER_CLASS		R_ALARM_COUNTER_CLASS
#define	R_CS_COUNTER_NG_CODE	R_ALARM_COUNTER_NG_CODE
#else
// カウンタ分類構造体
typedef struct {
	int nClass;						// カウンタ分類番号
	char* sMessJp;					// 分類名称(日本語)
	char* sMessEn;					// 分類名称(英語)
} R_CS_COUNTER_CLASS;

// カウンタＮＧ番号構造体
typedef struct {
	int   nCode;					// ＮＧ番号
	char* sMessJp;					// ＮＧ名称(日本語)
	char* sMessEn;					// ＮＧ名称(英語)
	int   nClass;					// 分類番号
} R_CS_COUNTER_NG_CODE;
#endif

// カウンタ構造体
typedef struct {
	int  nTotal;					// トータルカウンタ
	int  nGo;						// ＧＯカウンタ
	int  nNg;						// ＮＧ全数カウンタ
	int  nCodeNum;					// ＮＧ番号数
	int* pCodeCount;				// ＮＧカウンタ本体
	int  nClassNum;					// 分類カウンタ数
	int* pClassCount;				// 分類カウンタ本体
	R_CS_COUNTER_NG_CODE* pCodeName;		// ＮＧ名称
	R_CS_COUNTER_CLASS*   pClassName;	// 分類名称
	int  nAlarmNum;					// アラーム発生までの数
	int  nAlarmCount;				// 現在のアラームカウンタの数
	int  nAlarmCode;				// 現在のアラーム番号
} R_CS_COUNTER_DATA;

// --- prototype ---
int R_CS_CounterOpen(R_CS_COUNTER_DATA* pCounter, R_CS_COUNTER_NG_CODE* pCode);
int R_CS_CounterClose(R_CS_COUNTER_DATA* pCounter);

int R_CS_CounterSetClass(R_CS_COUNTER_DATA* pCounter, R_CS_COUNTER_CLASS* pClass);
int R_CS_CounterSetAlarm(R_CS_COUNTER_DATA* pCounter, int nNumber);
int R_CS_CounterCheckAlarm(R_CS_COUNTER_DATA* pCounter, int nCode);

int R_CS_CounterClear(R_CS_COUNTER_DATA* pCounter);
int R_CS_CounterUp(R_CS_COUNTER_DATA* pCounter, int nCode);
int R_CS_CounterDown(R_CS_COUNTER_DATA* pCounter, int nCode);
int R_CS_CounterCalculate(R_CS_COUNTER_DATA* pCounter, int nCode, int nDiff);

int R_CS_CounterGetNgNum(R_CS_COUNTER_DATA* pCounter);
int R_CS_CounterGetClassNum(R_CS_COUNTER_DATA* pCounter);

int R_CS_CounterGetTotalCount(R_CS_COUNTER_DATA* pCounter);
int R_CS_CounterGetGoCount(R_CS_COUNTER_DATA* pCounter);
int R_CS_CounterGetNgTotalCount(R_CS_COUNTER_DATA* pCounter);
int R_CS_CounterGetNgCount(R_CS_COUNTER_DATA* pCounter, int nCode);

int R_CS_CounterGetClass(R_CS_COUNTER_DATA* pCounter, int nCode);

char* R_CS_CounterGetCodeName(R_CS_COUNTER_DATA* pCounter, int nCode, int nMode);

int R_CS_CounterSetTotalCount(R_CS_COUNTER_DATA* pCounter, int nCount);
int R_CS_CounterSetGoCount(R_CS_COUNTER_DATA* pCounter, int nCount);
int R_CS_CounterSetNgTotalCount(R_CS_COUNTER_DATA* pCounter, int nCount);
int R_CS_CounterSetNgCount(R_CS_COUNTER_DATA* pCounter, int* pCount);

int R_CS_CounterGetResult(R_CS_COUNTER_DATA* pCounter, char* sBuffer);
int R_CS_CounterGetResultLabel(R_CS_COUNTER_DATA* pCounter, char* sBuffer, int nMode);
int R_CS_CounterGetResultClass(R_CS_COUNTER_DATA* pCounter, char* sBuffer);
int R_CS_CounterGetResultClassLabel(R_CS_COUNTER_DATA* pCounter, char* sBuffer, int nMode);

double R_CS_CounterGetRateNg(R_CS_COUNTER_DATA* pCounter);

int	R_CS_CounterProductionOutputWidgetOpen(void);
void	R_CS_CounterProductionOutputWidgetClose(void);
void	R_CS_CounterProductionOutputWidgetCountUp(R_CS_COUNTER_DATA* pCounter, int delta);
void	R_CS_CounterProductionOutputWidgetSetCounter(R_CS_COUNTER_DATA* pCounter);

// 未実装
//int  CounterView(R_CS_COUNTER_DATA* pCounter);
//int CounterLoad(R_CS_COUNTER_DATA* pCounter, char* sName);
//int CounterSave(R_CS_COUNTER_DATA* pCounter, char* sName);
//int COunterSetLotName(R_CS_COUNTER_DATA* pCounter, char* sLotName);

#endif
