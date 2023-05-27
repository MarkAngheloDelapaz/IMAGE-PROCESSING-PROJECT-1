/******************************************************************************

	R_CS_COM.H

									Authered. by	Kazuya Matsunaga
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN
									e-mail:ipg_system@rohm.co.jp

	履歴
	<version>	<date>		<comment>
	000			2007.11.15	新規追加
	001			2007.11.22	R_CS_ComChangeDirectory、R_CS_ComCheckFileNumberInDir、
							R_CS_ComCheckDirectory、R_CS_ComCheckDriveFreeSpace追加
	1.07b1.4
				2008.08.07	R_CS_ComRemoveDirectory追加
	1.07b3.1	2011.04.27	R_CS_getch(), R_CS_kbhit()暫定追加
******************************************************************************/
#ifndef __R_CS_COM_H__
#define __R_CS_COM_H__


// --- define ---
#define R_CS_EXTENDER_DOS4GW	0		// DOS4G/W
#define R_CS_EXTENDER_DOS32A	1		// DOS32/A

// --- variable ---


// --- prototype ---
int R_CS_ComCheckExtender(void);
int R_CS_ComChangeDirectory(char *sDirName);
int R_CS_ComCheckFileNumberInDir(char *sDirName);
int R_CS_ComCheckDirectory(char *sAbsDir);
int R_CS_ComCheckDriveFreeSpace(char *sFullPath, int nFileSize);
int R_CS_ComRemoveDirectory(char *sDirName);		// 1.07b1.4		2008.08.07 追加

int R_CS_getch(void);			//<1.07b3.1> コンソールからの入力を取得する
int R_CS_kbhit(void);			//<1.07b3.1> コンソールからの入力があるかをチェックする

#endif
