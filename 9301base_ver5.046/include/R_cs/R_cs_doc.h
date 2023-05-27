/******************************************************************************
	
	R_CS_DOC.H

	ドキュメント作成関数ソース
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Takashi Ohshima
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2005.08.05.000		オリジナル

******************************************************************************/
#ifndef __R_CS_DOC_H__
#define __R_CS_DOC_H__

// --- define ---

// --- variable ---

// --- prototype ---
int R_CS_Document(char *title_j,char *title_e,char *comment_j,char *comment_e,char *f_name,...);

#endif
