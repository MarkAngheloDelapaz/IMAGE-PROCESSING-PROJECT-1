#ifndef	R_MATRIX_H_
#define	R_MATRIX_H_

//#ifndef __R_STDLIB_H__
//#include "R_STDLIB.h"
//#endif
#include "R_stdlib.h"

#define R_FTINY 1.0e-20;
typedef struct{
	int **matr;		//�s��̃|�C���^�̃|�C���^
	int nrl;		//�s���^�O�ŏ��l
	int nrh;		//�s���^�O�ő�l
	int row_size;	//�s��
	int ncl;		//�񐔃^�O�ŏ��l
	int nch;		//�񐔃^�O�ŏ��l
	int col_size;	//��
}R_MATRIXN;
typedef struct{
	float **matr;		//�s��̃|�C���^�̃|�C���^
	int nrl;		//�s���^�O�ŏ��l
	int nrh;		//�s���^�O�ő�l
	int row_size;	//�s��
	int ncl;		//�񐔃^�O�ŏ��l
	int nch;		//�񐔃^�O�ŏ��l
	int col_size;	//��
}R_MATRIXF;
typedef struct{
	double **matr;		//�s��̃|�C���^�̃|�C���^
	int nrl;		//�s���^�O�ŏ��l
	int nrh;		//�s���^�O�ő�l
	int row_size;	//�s��
	int ncl;		//�񐔃^�O�ŏ��l
	int nch;		//�񐔃^�O�ŏ��l
	int col_size;	//��
}R_MATRIXD;
typedef struct{
	int *vector;	//�x�N�g���̃|�C���^
	int nl;			//�^�O�ŏ��l
	int nh;			//�^�O�ő�l
	int size;		//������
}R_VECTORN;
typedef struct{
	float *vector;	//�x�N�g���̃|�C���^
	int nl;			//�^�O�ŏ��l
	int nh;			//�^�O�ő�l
	int size;		//������
}R_VECTORF;
typedef struct{
	double *vector;	//�x�N�g���̃|�C���^
	int nl;			//�^�O�ŏ��l
	int nh;			//�^�O�ő�l
	int size;		//������
}R_VECTORD;
// --- variable ---
#ifdef _R_MATRIX_FILE_

#else

#endif


// --- prototype ---
int R_matrix_open_n( R_MATRIXN *str, int nrl, int nrh, int ncl, int nch);				//�s��̈�(int�^)�̊m��
void R_matrix_close_n(R_MATRIXN *str);													//�s��̈�(int�^)�̊J��
void R_matrix_draw_n( int color, int mode, int cx, int cy, R_MATRIXN *str );				//�s��̕\��
int R_matrix_and_n( R_MATRIXN *strA, R_MATRIXN *strB, R_MATRIXN *strC );						//�s��̐�

int R_matrix_open_f( R_MATRIXF *str, int nrl, int nrh, int ncl, int nch);				//�s��̈�(float�^)�̊m��
void R_matrix_close_f(R_MATRIXF *str);													//�s��̈�(float�^)�̊J��
void R_matrix_draw_f( int color, int mode, int cx, int cy, R_MATRIXF *str );				//�s��̕\��
int R_matrix_and_f( R_MATRIXF *strA, R_MATRIXF *strB, R_MATRIXF *strC );						//�s��̐�

int R_matrix_open_d( R_MATRIXD *str, int nrl, int nrh, int ncl, int nch);				//�s��̈�(double�^)�̊m��
void R_matrix_close_d(R_MATRIXD *str);													//�s��̈�(double�^)�̊J��
void R_matrix_draw_d( int color, int mode, int cx, int cy, R_MATRIXD *str );				//�s��̕\��
int R_matrix_and_d( R_MATRIXD *strA, R_MATRIXD *strB, R_MATRIXD *strC );						//�s��̐�

int R_matrix_LUdecomp_f( R_MATRIXF *mstr, int *indx, int *info);
int R_matrix_LUdecomp_d( R_MATRIXD *mstr, int *indx, int *info);
int R_matrix_LUbacksub_f( R_MATRIXF *str, int *indx, float b[],float x[]);
int R_matrix_LUbacksub_d( R_MATRIXD *str, int *indx, double b[],double x[]);

int R_vector_and_vector_f( R_VECTORF *str1, R_VECTORF *str2, float *inner  );				//�x�N�g���~�x�N�g��
int R_vector_and_vector_d( R_VECTORD *str1, R_VECTORD *str2, double *inner  );				//�x�N�g���~�x�N�g��
int R_vector_and_vector_n( R_VECTORN *str1, R_VECTORN *str2, int *inner  );					//�x�N�g���~�x�N�g��
void R_vector_and_scalar_f( R_VECTORF *str1, float scalar, R_VECTORF *res_str );			//�X�J���[�~�x�N�g��
void R_vector_and_scalar_d( R_VECTORD *str1, double scalar, R_VECTORD *res_str );			//�X�J���[�~�x�N�g��
void R_vector_and_scalar_n( R_VECTORN *str1, int scalar, R_VECTORN *res_str );				//�X�J���[�~�x�N�g��
void R_vector_close_n( R_VECTORN *str );													//�x�N�g���̈�̃N���[�Y
void R_vector_close_d( R_VECTORD *str );													//�x�N�g���̈�̃N���[�Y
void R_vector_close_f( R_VECTORF *str );													//�x�N�g���̈�̃N���[�Y
int R_vector_open_d(R_VECTORD *str, long nl, long nh);										//�x�N�g���̈�̊m��
int R_vector_open_f(R_VECTORF *str, long nl, long nh);										//�x�N�g���̈�̊m��
int R_vector_open_n(R_VECTORN *str, long nl, long nh);										//�x�N�g���̈�̊m��
void R_vector_draw_f( int color, int mode, int cx, int cy, R_VECTORF *str );				//�x�N�g�������\��
void R_vector_draw_d( int color, int mode, int cx, int cy, R_VECTORD *str );				//�x�N�g�������\��
void R_vector_draw_n( int color, int mode, int cx, int cy, R_VECTORN *str );				//�x�N�g�������\��
void R_vector_two_norm_d( R_VECTORD *str1, R_VECTORD *str2, double *result );				//�x�N�g���̃m�������v�Z
void R_vector_copy_d( R_VECTORD *str1, R_VECTORD *str2 );									//�x�N�g���P���x�N�g���Q�ɃR�s�[

#endif	// R_MATRIX_H_
