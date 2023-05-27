// 暫定ソースです。仕様変更する場合があります。
// ソースを変更しないでください。なにかあれば荒木まで。

/******************************************************************************
	R_CPUID.H / R_CPUID.ASM / R_CPUIDC.C
	
	変更履歴
	Ver 3.49	2006/10/23	r_cpuid_is_implemented追加
	Ver 3.49	2006/10/23	r_cpuid_info追加
	Ver 3.49	2006/10/24	r_delay_rdtscプロトタイプ宣言削除
	Ver 3.50	2008/07/10	SSE4対応
	Ver 3.50	2009/03/11	r_cpuid_info仕様変更
	Ver 4.01	2009/09/16	x86_64 ソース統合
	Ver 4.01	2010/11/11	AVX対応、SSE4バグフィクス
******************************************************************************/

#ifndef	R_CPUID_H_
#define	R_CPUID_H_

#ifdef __cplusplus	// 2010/11/11 by araki
extern "C" {
#endif

struct R_cpuid_t {
	int		nMaxInputValueforBasicInfo;
	char	strVendorID[13];
	int		nFamilyID;
	int		nModel;
	int		nSteppingID;
	int		nProcessorType;
	int		bFPU;
	int		bTSC;
	int		bMMX;
	int		bSSE;
	int		bSSE2;
	int		bSSE3;
	int		bSSSE3;
	int		bSSE41;
	int		bSSE42;
	int		bAVX;
	int		bMCE;
	int		bCX8;
	int		bHTT;
};

int		r_cpuid_init(void);
int		r_cpuid_is_implemented(void);
//int	r_cpuid_info(int n, union REGS*);
void	r_cpuid_info(int CPUInfo[4], int InfoType);

int r_get_cpuid(char*);
int r_get_rdtsc(DLONG*);

extern struct R_cpuid_t		R_CPUID;
extern struct R_cpuid_t*	pRCpuId;

#ifdef __cplusplus	// 2010/11/11 by araki
}
#endif

#endif	// R_CPUID_H_
