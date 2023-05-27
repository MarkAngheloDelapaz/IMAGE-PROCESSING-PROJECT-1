/******************************************************************************

	I/Oチェック関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2004.04.07.000		オリジナル
				2004.04.23.001		DrawPlane描画関数(R_draw系)を組み込み
				2004.06.11.002		バイナリのステータス表示不具合修正
				2005.xx.xx.003		VESA系移植							by matsu
				2005.10.12.004		IN-CY-84対応(DLIO使用)				by matsu
				2005.10.17.005		DLIO用に基板登録関数追加			by matsu
				2020.07.06.006		R_cs/R_cs_ioとの共存廃止、完全統合

******************************************************************************/

// --- define ---
#define BaseIOCheck		IOCheck

#define	R_CS_IOCheck()	R_IOCheck()
#define	R_CS_IOCheckRegisterBoard(nId,nHandle,name)	R_IOCheckRegisterBoard(nId,nHandle,name)
#define	R_CS_IOCheck39()	R_CS_IOCheck39()

#include "R_iochk.h"
