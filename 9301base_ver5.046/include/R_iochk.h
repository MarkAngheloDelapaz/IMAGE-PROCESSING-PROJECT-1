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
				2006.06.07.006		画像基板対応(CO-CZ-33/35/39)		by matsu
				2010.04.12.007		Linuxコンパイル時ワーニング対策		by matsu
	1.07b3.0	2010.12.10.008		IN-CY-84 OC/PC I/O追加				by matsu
				2011.04.27.009		IOCheckMain()内の Intel Compiler remark回避のため２行に分割	by matsu
	1.07b4.1	2012.05.10.010		Linuxマルチプロセス時にR_dlio_open()の戻り値が常に０なのでDLIO系は自動スキャンしないように変更	by matsunaga
									(シングルプロセス時は従来同様自動スキャンします)
				2013.03.08.006		9301base化関数リネーム				by tanaka
				2014.06.17.011		DR-CY-140対応
	1.07b6.4	2015.04.30.012		IN-CY-120(PCI-2724C)のIO確認対応
	1.07b7.1	2015.12.28.013		IN-CY-122対応
	5.030		2020.07.01			全てのR_Draw～関数をR_Drawにリネーム
	5.031		2020.07.06			R_cs/R_cs_ioとの共存廃止、完全統合
	5.031		2020.07.06			CO-CZ-47対応
******************************************************************************/

#ifndef	R_IOCHK_H_
#define	R_IOCHK_H_

#ifdef __cplusplus
extern "C" {
#endif

void R_IOCheck( void );
int R_IOCheckRegisterBoard( int nId, int nHandle, char *name );
void R_IOCheck39( void );

#ifdef __cplusplus
}
#endif

#endif	// R_IOCHK_H_
