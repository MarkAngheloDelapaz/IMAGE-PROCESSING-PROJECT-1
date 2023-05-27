/******************************************************************************
	R_STRING.H / R_STRING.C
	
	変更履歴
					1994/03/16	org
	Ver.3.50		2009/02/10	DOS & LINUX ソース統合
	Ver.4.01b119	2014/01/24	R_trim，R_trim_s関数追加				by komatsu
******************************************************************************/

#ifndef	R_STRING_H_
#define	R_STRING_H_

void	R_conv_han2zen( char * );
int 	R_trim(char *str);
char* 	R_trim_s(char *src,char *dst);

#endif	// R_STRING_H_
