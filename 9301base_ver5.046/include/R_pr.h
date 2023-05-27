/******************************************************************************
	R_pr.h / R_pr.c
	
	変更履歴
	Ver 3.50	2008/03/05	正式リリース
	Ver 3.50	2009/02/10	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_PR_H_
#define	R_PR_H_

//#ifndef __R_STDLIB_H__
//#include "R_STDLIB.h"
//#endif
#include "R_stdlib.h"
//#include "R_matrix.h"

#define R_SVM_TAG_PASS	1.0
#define R_SVM_TAG_FAIL	-1.0
#define R_PR_LIN	0				//線形
#define R_PR_NOLIN	1				//非線形
#define R_PR_CLASS_ELEMENT	-1		//タグ要素
#define R_VECTOR_MULTI_MAX	10000
typedef struct{
	int nNumber;
	int nDim;
	R_VECTORD stVector[R_VECTOR_MULTI_MAX];
	R_VECTORD stBufferVector[R_VECTOR_MULTI_MAX];
	double *pY;
}R_PRLEARN_DATA;

typedef struct{
	int		nMode;					//サポートベクターマシンのモード（0:線形 1:非線形）
	int		nDim;					//SVMベクトル次元数
	int		nMaxRoop;				//最大繰返し回数
	int		nNumber;				//ベクトルデータの個数
	int		nCategoryANumber;		//カテゴリーA（1）の学習データ数
	int		nCategoryBNumber;		//カテゴリーB（1）の学習データ数
	double	dNormalizeScale;			//サンプルデータの正規化スケール
	double *pLmd;					//Lagrangue未定乗数
	double *pLmdDelta;				//Lagrangue未定乗数の逐次変化増分
	double *pGaussianKernel;		//カーネル
	double *pW;						//重み係数
	double *pB;						//バイアス係数
	double *pVectorAve;				//サンプルデータの平均値
	double *pY;						//データの学習タグ
	R_VECTORD	*pX;				//学習ベクトルデータ
	R_VECTORD	*pOrgX;				//学習ベクトルデータ
}R_PRSVM_PARA;
// --- variable ---
#ifdef _R_PR_FILE_
int nLearnErrorCode = -1;
#else
extern int nLearnErrorCode;
#endif


// --- prototype ---
int R_pr_svm_set_data( int dim, int number, int max_roop,R_VECTORD *str_org, R_VECTORD *str_buff,double *tag );			//SVM学習データのセット
int R_pr_svm_close_data( int nHandle );																					//SVM学習データのクローズ
int R_pr_svm_transfer_data( int nHandle );																				//特徴ベクトルの正規化
int R_pr_svm_count_category_number( int nHandle );																		//クラスに属しているデータ数を計算
int R_pr_svm_get_error( void );																							//R_pr_svm_set_dataでのエラーコード
int R_pr_svm_draw_parameter( int nHandle );																				//SVM用行列パラメータの表示
int R_pr_svm_count_category_number( int nHandle );																		//学習タグデータの計数
int R_pr_svm_draw_support_vector( int nHandle );																		//サポートベクターの表示（２次元のみ）
int R_pr_svm_draw_line( int nHandle );																					//境界直線の表示（２次元、線形SVMのみ）
int R_pr_svm_get_parameter( int nHandle );																				//重み係数、バイアス項の計算
int R_pr_svm_draw_area( int nHandle, unsigned char *sp );																//境界領域の表示（２次元、非線形SVMのみ）
int R_pr_svm_lmd_normalize( int nHandle );																				//Lagrangue未定乗数の正規化
int R_pr_svm_get_delta_lmd( int nHandle );																				//Lagrangue未定乗数の増分計算
int R_pr_svm_set_type( int nHandle, int mode );																			//SVMモードの選択（０：線形　１：非線形）
int R_pr_svm_lmd_initial( int nHandle );																				//Lagrangue未定乗数の初期正規化
int R_pr_svm_set_kernel( int nHandle );																					//非線形SVM用Mercelカーネルの作成
int R_pr_svm_get_support_vector_number( int nHandle );																	//サポートベクトルの個数を取得
int R_pr_svm_return_parameter( int nHandle );																			//パラメータの元空間への写像
int R_pr_svm_normalize_parameter( int nHandle );																		//パラメータの正規空間への写像
int R_pr_svm_save_data( int nHandle, int mode, char *filename, char *comment  );										//SVMデータのファイル書き込み


int R_pr_learn_data_open( int number, int dimension );																	//学習用データ領域確保
int R_pr_learn_data_close( int nHandle );																				//学習用データ領域解放
int R_pr_learn_svm_data_charge( int nLearnDataHandle, int nDataNumber, int nMaxRoop );									//学習用データをSVM構造体に割り当てる
int R_pr_learn_svm_data_discharge( int nHandle );																		//SVM構造体からの学習用データの解放
int R_pr_learn_svm_execute( int nHandle, int nMode );																	//SVMによる学習実行
int R_pr_learn_set_element( int nLearnDataHandle, int number, int dim, double element );								//学習用データ入力
int R_pr_learn_get_element( int nLearnDataHandle, int number, int dim, double *element );								//学習用データ出力
int R_pr_learn_svm_judgement( int nHandle, R_VECTORD *str_org, int mode );												//データのSVM識別器による判定
int R_pr_learn_svm_transfer_data( int nHandle, R_VECTORD *str_org, R_VECTORD *str_buff );								//データのSVM正規空間への写像


//int R_pr_learn_disp_data_contents( int cx, int cy, int nLearnDataHandle, int number );									
//int R_pr_learn_ability_of_classifier( int nHandle, int *percent );
//int R_pr_learn_consideration_from_file( unsigned char *filename );

#endif	// R_PR_H_
