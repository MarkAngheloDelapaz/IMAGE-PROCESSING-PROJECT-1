/******************************************************************************
	R_DR101.H / R_DR101.C
	
	変更履歴
		Ver 3.46	2005/08/04	正式リリース
		Ver 3.49	2007/02/19	-2/3/4対応
		Ver 3.50	2009/02/12	DOS & LINUX ソース統合
		Ver 4.01	2012/09/06	DR-CY-101マルチプロセス暫定対応
******************************************************************************/

#ifndef	R_DRCY101_H_
#define	R_DRCY101_H_

#ifdef __cplusplus
extern "C" {
#endif

int		r_drcy101_dac_open(int nID);
void	R_drcy101_dac_close(int nHandle);
int		R_drcy101_dac_set(int nHandle, int nChannel, int nData);

int r_drcy101_get_apic21_port(int nHandle);
int r_drcy101_get_register_port(int nHandle);
int r_drcy101_set_ref(int nHandle, int nChannel, int nData);
int r_drcy101_dac_current_initial(int nHandle);
int r_drcy101_set_current(int nHandle,int port,int data);
int r_drcy101_get_revision(int nHandle);

int		r_drcy101_dac_open(int nID);
int		r_drcy101_get_apic21_port(int nHandle);
int		r_drcy101_get_register_port(int nHandle);

#ifdef __cplusplus
}
#endif

#endif	// R_DRCY101_H_
