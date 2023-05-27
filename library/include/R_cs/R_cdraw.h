/******************************************************************************

	画面描画関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2004.04.19.000		オリジナル
				2005.02.17.001		グローバル変数が他のオブジェクトとかぶらないように、static化+名前変更
				2006.05.09.002		R_DrawClipLine,R_DrawChrdisp,R_DrawInputData追加
				2007.09.18.004		CoCz39Flag、CoCz43Flagマクロ化
									R_CS_DISPLAY追加
				2008.02.12			R_DrawWinMessage()追加
				2008.02.19			R_DrawGetDisplayPlane()追加
				2008.03.05			R_CS_DISPLAY -> R_CDRAW_DISPLAYに変更
				2008.03.20			R_CDRAW_DISPLAY,struct display_window修正
									R_DrawWinbMenu() -> R_DrawWinbMenuFix()に変更
									R_DrawWinbMenu()追加
				2008.05.08			R_DrawUpdateBoardStatus()追加
	1.07b1.4	2009.09.19			R_DrawWindowConvertPoint(),R_DrawWindowConvertRect()追加
	1.07b3.1	2011.05.17			R_DrawAffine系追加
	v5.007		2018.11.14			R_drawc(9301lib)との共存対策実施
	5.030		2020.06.30			R_cs/R_cdrawとの共存廃止、完全統合
******************************************************************************/

#ifndef R_CDRAW_H
#define R_CDRAW_H

#include "R_drawc.h"

#endif
