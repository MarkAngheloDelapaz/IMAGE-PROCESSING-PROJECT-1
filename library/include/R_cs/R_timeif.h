/******************************************************************************

	R_TIMEIF.H

	時間情報取得関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2004.06.13.000		オリジナル
				2006.10.09			<test> ltm追加

******************************************************************************/

#include <time.h>

// --- define ---
//日時構造体
typedef struct{
	int		year;				//年
	int		month;				//月
	int		day;				//日
	int		hour;				//時
	int		minute;				//分
	int		second;				//秒
	struct tm	ltm;			// ※ctime,gmtime呼び出しで上書きされるので実体を宣言
}R_TIME_INFO;

// --- variable ---

// --- prototype ---
void R_TimeInfoGetNow( R_TIME_INFO *date );
int R_TimeInfoGetFile( char *filename, R_TIME_INFO *date );

