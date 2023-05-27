/***************************************************************************
	R_chough.h
	
����
Ver 4.01	2009.08.07 R_chough_max_point_detection_x256�ǉ�	by nakahara

***************************************************************************/

#ifndef R_CHOUGH_H_
#define R_CHOUGH_H_

#include "R_stdlib.h"

#ifdef __x86_64__
#include "R_plane.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define R_CHOUGH_DRAW_NORMAL	0
#define R_CHOUGH_DRAW_MAX		1
#define R_CHOUGH_DRAW_XY	0
#define R_CHOUGH_DRAW_XR	1
#define R_CHOUGH_DRAW_YR	2
// --- variable ---
#ifndef __x86_64__
extern int nCHoughErrorCode;
#endif

typedef struct{
	int nRMin;								// �ő匟�o���a�ő�l
	int nRMax;								// �ő匟�o���a�ŏ��l
	int nXMin;								// �ő匟�o�̈捶��X���W
	int nYMin;								// �ő匟�o�̈捶��Y���W
	int nXMax;								// �ő匟�o�̈�E��X���W
	int nYMax;								// �ő匟�o�̈�E��Y���W
	int nRSearchMin;						// �T�����锼�a�̍ŏ��l
	int nRSearchMax;						// �T�����锼�a�̍ő�l
	int nXSearchMin;						// �T������w���W�̍ŏ��l
	int nXSearchMax;						// �T������w���W�̍ő�l
	int nYSearchMin;						// �T������x���W�̍ŏ��l
	int nYSearchMax;						// �T������x���W�̍ő�l
	int nMemorySize;						// ���C�u�����Ŏg�p���Ă��郁�����T�C�Y[byte]
	int nAreaSize;							// ���[�̈�(XYR�S��)�̃T�C�Y[byte]
	int nPlaneSize;							// ���[�̈�(XY�̂݁AR�Œ�)�̃T�C�Y[byte]
	unsigned short *pChoughVotingArea;		// ���[�̈�ւ̃|�C���^
	int nVotingNumber;						// ���[��(R_chough_voting�œ��[������)	2008.07.29 �ǉ�
}R_CHOUGH_DATA;
// --- prototype ---
#ifdef __x86_64__
typedef	R_CHOUGH_DATA*	R_CHOUGH_HANDLE;
R_CHOUGH_HANDLE	R_chough_open(R_RECT* tWindow, int nRMin, int nRMax, int width, int height);	// �~���o�n�t�ϊ����C�u�����̃I�[�v��
int	R_chough_disp_voting_area(R_CHOUGH_HANDLE nHandle, int d, int cut_mode, int mode, R_PLANE* plane);
#else
typedef	int				R_CHOUGH_HANDLE;
R_CHOUGH_HANDLE	R_chough_open(R_RECT* tWindow, int nRMin, int nRMax);	// �~���o�n�t�ϊ����C�u�����̃I�[�v��
int	R_chough_disp_voting_area(R_CHOUGH_HANDLE nHandle, int d, int cut_mode, int mode);
#endif
int	R_chough_close(R_CHOUGH_HANDLE nHandle);							// �~���o�n�t�ϊ����C�u�����̃N���[�Y
int	R_chough_clear(R_CHOUGH_HANDLE nHandle);							// �~���o�n�t��Ԃ̃N���A(���[�̃N���A)
int	R_chough_get_error(void);
int	R_chough_voting(R_CHOUGH_HANDLE nHandle, int number, R_POINT* point);
int	R_chough_set_search_radius(R_CHOUGH_HANDLE nHandle, int rmin, int rmax);
int	R_chough_set_search_area(R_CHOUGH_HANDLE nHandle, int xmin, int ymin, int xmax, int ymax);
int	R_chough_max_point_detection(R_CHOUGH_HANDLE nHandle, R_POINT *res_point, int *res_r , int *res_val);
int	R_chough_max_point_detection_x256(R_CHOUGH_HANDLE nHandle, R_POINT* res_point, int* res_r, int* res_val);		//2009.08.07 by nakahara added
int	R_chough_get_voted_value(R_CHOUGH_HANDLE nHandle, int x, int y, int r);
int	R_chough_get_memsize(R_CHOUGH_HANDLE nHandle);
int	R_chough_point_voting(R_CHOUGH_HANDLE nHandle, int r, R_POINT* point);

int R_chough_detect_consentric_circle(R_CHOUGH_HANDLE nHandle,int *width,int linenum, R_POINT* res_point, int* res_r, int* res_val);
int R_chough_detect_consentric_circle_x256(R_CHOUGH_HANDLE nHandle,int *width,int linenum, R_POINT* res_point, int* res_r, int* res_val);

R_POINT	R_chough_set_vote_area(R_CHOUGH_HANDLE nHandle, int xmin, int ymin);										//2016.11.21	komatsu
int		R_chough_set_vote_radius(R_CHOUGH_HANDLE nHandle, int r);												//2016.11.21	komatsu

/*
int R_chough_open( R_RECT *tWindow , int nRMin, int nRMax);							// �~���o�n�t�ϊ����C�u�����̃I�[�v��
int R_chough_close( int nHandle );													// �~���o�n�t�ϊ����C�u�����̃N���[�Y
int R_chough_clear( int nHandle );													// �~���o�n�t��Ԃ̃N���A(���[�̃N���A)
int R_chough_get_error( void );
int R_chough_voting( int nHandle, int number, R_POINT *point );
int R_chough_set_search_radius( int nHandle, int rmin, int rmax );
int R_chough_set_search_area( int nHandle, int xmin, int ymin, int xmax, int ymax );
int R_chough_max_point_detection( int nHandle, R_POINT *res_point, int *res_r , int *res_val);
int R_chough_max_point_detection_x256( int nHandle, R_POINT *res_point,int *res_r , int *res_val);		//2009.08.07 by nakahara added
int R_chough_get_voted_value( int nHandle, int x, int y, int r );
int R_chough_disp_voting_area( int nHandle, int d, int cut_mode, int mode );
int R_chough_get_memsize( int nHandle );
int R_chough_point_voting( int nHandle, int r, R_POINT *point );	// 2009/07/21
*/
//int R_chough_voting( int nHandle, R_POINT tPoint );								// �~���o�n�t��Ԃւ̓��[
//int R_chough_detection( int nHandle, int nNumber, R_CIRCLE pCoeff[] );			// �n�t�ϊ��ɂ��~�̌��o
//int R_chough_set_threshold( int nHandle, int nThreshold );						// �~���o�n�t�@���[���������l�̐ݒ�
//int R_chough_set_region( int nHandle, int nRegionX, int nRegionY, int nRegionR );	// �~���o�n�t�@�Ǐ��ɑ�_�����߂�̈�̐ݒ�
//int R_chough_set_start_point( int nHandle, R_POINT tStart );						// �~���o�n�t�@�����E�B���h�E�n�_�̐ݒ�
//int R_chough_set_vote_range_x( int nHandle, int nRangeMin, int nRangeMax );		// �~���o�n�t�@���SX���[�͈͂̐ݒ�
//int R_chough_set_vote_range_y( int nHandle, int nRangeMin, int nRangeMax );		// �~���o�n�t�@���SY���[�͈͂̐ݒ�
//int R_chough_set_vote_range_r( int nHandle, int nRangeMin, int nRangeMax );		// �~���o�n�t�@���a���[�͈͂̐ݒ�

#ifdef __cplusplus
}
#endif

#endif	// R_CHOUGH_H_
