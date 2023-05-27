/******************************************************************************

	R_cs_cod.h
	
	コード優先順位処理ヘッダ

									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	<version>	<date>		<name>		<comment>
	1.07b3.1	2011.05.18	matsu		新規作成

******************************************************************************/

#ifndef __R_CS_CODE_H__
#define __R_CS_CODE_H__


#include "R_cs_cnt.h"

typedef struct {
	int  nCode;							// NGコード
	char *sMess;						// 付帯メッセージ
} R_CS_CODE_PRIORITY_DATA;

typedef struct {
	R_CS_CODE_PRIORITY_DATA *pData;		// appendされたデータリスト
	int *pPriority;						// 優先順位リスト
	int *pStatus;						// 各コードの状態
	int nCodeMax;						// NGコード最大数
	int nMessMax;						// 付帯メッセージ最大文字数
	int nDataMax;						// NGコード登録最大数
	int nMode;							// モード
	int nDataPos;						// データ保持ポインタ
} R_CS_CODE_PRIORITY;


int R_CS_CodePriorityOpen(R_CS_CODE_PRIORITY *pPriority, int nCodeMax, int nMessMax, int nDataMax);		// コード優先順位処理オープン
int R_CS_CodePriorityClose(R_CS_CODE_PRIORITY *pPriority);												// コード優先順位処理クローズ
int R_CS_CodePriorityClear(R_CS_CODE_PRIORITY *pPriority);												// コード優先順位処理データクリア
int R_CS_CodePriorityLoad(R_CS_CODE_PRIORITY *pPriority, char *pFileName, int nId, int nMode);			// コード優先順位処理データ読み込み
int R_CS_CodePrioritySave(R_CS_CODE_PRIORITY *pPriority, char *pFileName, int nId, int nMode);			// コード優先順位処理データ保存
int R_CS_CodePrioritySetOrder(R_CS_CODE_PRIORITY *pPriority, int nCode, int nOrder);					// コード優先順位処理順位設定
int R_CS_CodePriorityGetOrder(R_CS_CODE_PRIORITY *pPriority, int nCode);								// コード優先順位処理順位取得
int R_CS_CodePrioritySetMode(R_CS_CODE_PRIORITY *pPriority, int nMode);									// コード優先順位処理モード設定
int R_CS_CodePriorityGetMode(R_CS_CODE_PRIORITY *pPriority);											// コード優先順位処理モード取得
int R_CS_CodePriorityAppend(R_CS_CODE_PRIORITY *pPriority, int nCode, char *sMess);						// コード優先順位処理ＮＧコード追加
int R_CS_CodePriorityCheck(R_CS_CODE_PRIORITY *pPriority, int *pResultCode, char *pResultMess);			// コード優先順位処理結果判定
int R_CS_CodePriorityEdit(R_CS_CODE_PRIORITY* pPriority, R_CS_COUNTER_NG_CODE* pCode);					// コード優先順位処理順位設定

#endif
