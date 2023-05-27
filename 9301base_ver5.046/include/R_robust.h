/******************************************************************************
	R_ROBUST.H / R_ROBUST.C
	
	変更履歴
		Ver 3.41	2004/01/14	正式リリース
******************************************************************************/

#ifndef	__R_ROBUST_H__
#define	__R_ROBUST_H__

#ifdef __cplusplus
extern "C" {
#endif

#define	ROBUST_MODE			0x01	//ヒストグラムの評価条件	最大値
#define	ROBUST_SQUARE		0x02	//							二乗平均

//ロバスト推定条件構造体
typedef struct{
	int deg_min;			//最小角度 10倍値[deg]
	int deg_max;			//最大角度 10倍値[deg]
	int deg_step;			//角度ステップ 10倍値[deg]
	int frequency_rate_min;	//座標点数に対する最頻値の度数の最低割合(0-100%)
	int moving_average;		//ヒストグラム移動平均区間
	int method;				//ヒストグラム評価条件
}ROBUST_CONDITION;

//ロバスト推定結果構造体
typedef struct{
	int angle;				//座標点群の角度 10倍値[deg]
	int frequency;			//最頻値の度数
	int position;			//最頻値の座標(アフィン変換後)
}ROBUST_RESULT;

#define	ROBUST_BUFF_MAX_NB	3
#ifdef __x86_64__
typedef struct {
	int*	RobustBuff[ROBUST_BUFF_MAX_NB];
	int*	RobustPrt;
	int		RobustBuffMaxSize;
} R_ROBUST_OBJ;
#endif

#ifdef __x86_64__
R_ROBUST_OBJ*	R_robust_open(int max_number);
int	R_robust_close(R_ROBUST_OBJ*);
int	R_robust_get_angle(R_ROBUST_OBJ*, int n, R_POINT* p, ROBUST_CONDITION condition, ROBUST_RESULT* result);
int	R_robust_sampling_line(R_ROBUST_OBJ*, int src_n, R_POINT* src_p, int* dst_n, R_POINT* dst_p, ROBUST_CONDITION condition, int number);
int	R_robust_get_line(R_ROBUST_OBJ*, int n, R_POINT* p, ROBUST_CONDITION condition, R_LINE* l);
int	R_robust_get_max_number(R_ROBUST_OBJ*);
#else
int R_robust_open( int max_number );
int R_robust_close( void );
int R_robust_get_angle(int n, R_POINT *p, ROBUST_CONDITION condition, ROBUST_RESULT *result);
int R_robust_sampling_line(int src_n, R_POINT *src_p, int *dst_n, R_POINT *dst_p, ROBUST_CONDITION condition, int number);
int R_robust_get_line(int n, R_POINT *p, ROBUST_CONDITION condition, R_LINE *l);
int R_robust_get_max_number( void );																							//最大可能データセット数の取得(2007.12.29 Nakahara)
#endif

#ifdef __cplusplus
}
#endif

#endif	// __R_ROBUST_H__
