/******************************************************************************
	R_DRCY83.H / R_DRCY83.C
	
	変更履歴
		Ver 3.31	2002/07/31	正式リリース
		Ver.3.32	2002/11/08	ヘッダ引き数明確に変更
		Ver.3.33	2003/1/16	内部関数プロトタイプを記載
		Ver 3.34	2003/03/10	r_drcy83_dac_open内部関数化
		Ver 3.35	2003/04/7	r_drcy83_set_current追加
		Ver 3.39	2003/06/24	r_drcy83_get_revision追加
		Ver 3.42	2004/04/14	DR-CY-95対応
		Ver 3.50	2009/02/12	DOS & LINUX ソース統合
		Ver 4.01	2010/04/22	DR-CY-83マルチプロセス暫定対応
		Ver 4.01	2013/02/08	DR-CY-95マルチプロセス対応
******************************************************************************/

#ifndef	R_DRCY83_H_
#define	R_DRCY83_H_

int		r_drcy83_dac_open(int nID);
void	R_drcy83_dac_close(int nHandle);
int		R_drcy83_dac_set(int nHandle, int nChannel, int nData);

int r_drcy83_get_apic21_port(int nHandle);
int r_drcy83_get_register_port(int nHandle);
int r_drcy83_set_ref(int nHandle, int nChannel, int nData);
int r_drcy83_dac_current_initial(int nHandle);
int r_drcy83_set_current(int nHandle,int port,int data);
int r_drcy83_get_revision(int nHandle);

int		r_drcy95_dac_open(int nID);
int		r_drcy95_get_apic21_port(int nHandle);
int		r_drcy95_get_register_port(int nHandle);

#define R_drcy95_dac_set		R_drcy83_dac_set
#define R_drcy95_dac_close		R_drcy83_dac_close

#endif	// R_DRCY83_H_
