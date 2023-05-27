#ifndef	R_MATRIX_H_
#define	R_MATRIX_H_

//#ifndef __R_STDLIB_H__
//#include "R_STDLIB.h"
//#endif
#include "R_stdlib.h"

#define R_FTINY 1.0e-20;
typedef struct{
	int **matr;		//行列のポインタのポインタ
	int nrl;		//行数タグ最小値
	int nrh;		//行数タグ最大値
	int row_size;	//行数
	int ncl;		//列数タグ最小値
	int nch;		//列数タグ最小値
	int col_size;	//列数
}R_MATRIXN;
typedef struct{
	float **matr;		//行列のポインタのポインタ
	int nrl;		//行数タグ最小値
	int nrh;		//行数タグ最大値
	int row_size;	//行数
	int ncl;		//列数タグ最小値
	int nch;		//列数タグ最小値
	int col_size;	//列数
}R_MATRIXF;
typedef struct{
	double **matr;		//行列のポインタのポインタ
	int nrl;		//行数タグ最小値
	int nrh;		//行数タグ最大値
	int row_size;	//行数
	int ncl;		//列数タグ最小値
	int nch;		//列数タグ最小値
	int col_size;	//列数
}R_MATRIXD;
typedef struct{
	int *vector;	//ベクトルのポインタ
	int nl;			//タグ最小値
	int nh;			//タグ最大値
	int size;		//次元数
}R_VECTORN;
typedef struct{
	float *vector;	//ベクトルのポインタ
	int nl;			//タグ最小値
	int nh;			//タグ最大値
	int size;		//次元数
}R_VECTORF;
typedef struct{
	double *vector;	//ベクトルのポインタ
	int nl;			//タグ最小値
	int nh;			//タグ最大値
	int size;		//次元数
}R_VECTORD;
// --- variable ---
#ifdef _R_MATRIX_FILE_

#else

#endif


// --- prototype ---
int R_matrix_open_n( R_MATRIXN *str, int nrl, int nrh, int ncl, int nch);				//行列領域(int型)の確保
void R_matrix_close_n(R_MATRIXN *str);													//行列領域(int型)の開放
void R_matrix_draw_n( int color, int mode, int cx, int cy, R_MATRIXN *str );				//行列の表示
int R_matrix_and_n( R_MATRIXN *strA, R_MATRIXN *strB, R_MATRIXN *strC );						//行列の積

int R_matrix_open_f( R_MATRIXF *str, int nrl, int nrh, int ncl, int nch);				//行列領域(float型)の確保
void R_matrix_close_f(R_MATRIXF *str);													//行列領域(float型)の開放
void R_matrix_draw_f( int color, int mode, int cx, int cy, R_MATRIXF *str );				//行列の表示
int R_matrix_and_f( R_MATRIXF *strA, R_MATRIXF *strB, R_MATRIXF *strC );						//行列の積

int R_matrix_open_d( R_MATRIXD *str, int nrl, int nrh, int ncl, int nch);				//行列領域(double型)の確保
void R_matrix_close_d(R_MATRIXD *str);													//行列領域(double型)の開放
void R_matrix_draw_d( int color, int mode, int cx, int cy, R_MATRIXD *str );				//行列の表示
int R_matrix_and_d( R_MATRIXD *strA, R_MATRIXD *strB, R_MATRIXD *strC );						//行列の積

int R_matrix_LUdecomp_f( R_MATRIXF *mstr, int *indx, int *info);
int R_matrix_LUdecomp_d( R_MATRIXD *mstr, int *indx, int *info);
int R_matrix_LUbacksub_f( R_MATRIXF *str, int *indx, float b[],float x[]);
int R_matrix_LUbacksub_d( R_MATRIXD *str, int *indx, double b[],double x[]);

int R_vector_and_vector_f( R_VECTORF *str1, R_VECTORF *str2, float *inner  );				//ベクトル×ベクトル
int R_vector_and_vector_d( R_VECTORD *str1, R_VECTORD *str2, double *inner  );				//ベクトル×ベクトル
int R_vector_and_vector_n( R_VECTORN *str1, R_VECTORN *str2, int *inner  );					//ベクトル×ベクトル
void R_vector_and_scalar_f( R_VECTORF *str1, float scalar, R_VECTORF *res_str );			//スカラー×ベクトル
void R_vector_and_scalar_d( R_VECTORD *str1, double scalar, R_VECTORD *res_str );			//スカラー×ベクトル
void R_vector_and_scalar_n( R_VECTORN *str1, int scalar, R_VECTORN *res_str );				//スカラー×ベクトル
void R_vector_close_n( R_VECTORN *str );													//ベクトル領域のクローズ
void R_vector_close_d( R_VECTORD *str );													//ベクトル領域のクローズ
void R_vector_close_f( R_VECTORF *str );													//ベクトル領域のクローズ
int R_vector_open_d(R_VECTORD *str, long nl, long nh);										//ベクトル領域の確保
int R_vector_open_f(R_VECTORF *str, long nl, long nh);										//ベクトル領域の確保
int R_vector_open_n(R_VECTORN *str, long nl, long nh);										//ベクトル領域の確保
void R_vector_draw_f( int color, int mode, int cx, int cy, R_VECTORF *str );				//ベクトル成分表示
void R_vector_draw_d( int color, int mode, int cx, int cy, R_VECTORD *str );				//ベクトル成分表示
void R_vector_draw_n( int color, int mode, int cx, int cy, R_VECTORN *str );				//ベクトル成分表示
void R_vector_two_norm_d( R_VECTORD *str1, R_VECTORD *str2, double *result );				//ベクトルのノルムを計算
void R_vector_copy_d( R_VECTORD *str1, R_VECTORD *str2 );									//ベクトル１をベクトル２にコピー

#endif	// R_MATRIX_H_
