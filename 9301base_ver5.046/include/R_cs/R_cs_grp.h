/******************************************************************************

	R_CS_GRP.H
	
	グラフ表示関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2004.05.05.000		オリジナル
				2007.10.25.001		折れ線グラフ表示を追加

******************************************************************************/

// --- define ---
#define GRAPH_SCATTER	0							//散布図
#define GRAPH_BAR		1							//棒グラフ
#define GRAPH_LINE		2							//折れ線グラフ

#define GRAPH_DATA_NB	4

#ifdef __cplusplus
extern "C"{
#endif


//グラフ表示構造体
typedef struct{
	char			*label_j;					//ラベル(日本語)
	char			*label_e;					//ラベル(英語)
	int				min;						//最小値
	int				max;						//最大値
}R_CS_GRAPH_ITEM_AXIS;

typedef struct{
	char			*tytle_j;					//グラフのタイトル(日本語)
	char			*tytle_e;					//グラフのタイトル(英語)
	int				*data[GRAPH_DATA_NB];		//描画するデータ配列の先頭ポインタ
	int				number;						//描画するデータ配列の個数
	R_CS_GRAPH_ITEM_AXIS	axis_x;						//X軸の情報
	R_CS_GRAPH_ITEM_AXIS	axis_y;						//Y軸の情報
	R_RECT			disp_area;					//描画エリア
	int				mode;						//描画モード
}R_CS_GRAPH_ITEM;

// --- variable ---

// --- prototype ---
int R_CS_GraphDisp( R_CS_GRAPH_ITEM *item );
int R_CS_GraphDispRing( R_CS_GRAPH_ITEM *item, int start, int end, int size );


#ifdef __cplusplus
}
#endif
