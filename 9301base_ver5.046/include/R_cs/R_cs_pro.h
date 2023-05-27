/******************************************************************************

	R_CS_PRO.H
	
	生産管理関数ヘッダ
	バージョンアップ時は、必ずVersion[]の変更、履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	<version>	<date>		<name>		<comment>
	000			2004.04.07	sassa		オリジナル
	001			2004.06.13	sassa		PRODUCT_DATAに生産開始時間を追加
	002			2005.02.17	sassa		グローバル変数が他のオブジェクトとかぶらないように、static化+名前変更
	1.07b3.0	2010.11.16	sassa		R_CS_ProductCounterOpen追加

******************************************************************************/

// --- define ---
//生産情報構造体
typedef struct{
	int			nProductNb;			//生産数
	int			nRpmAverage;		//平均回転数
	int			nRpmCurrent;		//直近の回転数
	int			nChangeTime;		//ロット切り替え時間
	R_TIME_INFO	tStartTime;			//生産開始時間情報
	char		sLotNo[32];			//ロット名
}R_CS_PRODUCT_DATA;

// --- variable ---

// --- prototype ---
int R_CS_ProductCounterOpen(R_CS_PRODUCT_DATA *pData);		// 2010.11.16 by sassa
int R_CS_ProductCounterClear(R_CS_PRODUCT_DATA *pData);
int R_CS_ProductCounterUp(R_CS_PRODUCT_DATA *pData);
int R_CS_ProductSetCycleNum(int number);


