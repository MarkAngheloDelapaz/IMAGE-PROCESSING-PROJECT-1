/******************************************************************************
	R_calip.h
	
	�ύX����
	Ver 3.50	2009.03.24	R_calip_line_l2d,R_calip_degree_l2l�֐��ǉ�
	Ver 3.50	2009.04.02	DOS & LINUX �\�[�X����
	Ver 3.50	2009.06.30	R_calip_find_lineV()��������
	Ver 4.01	2009.08.07	R_calip_change_line_with_org�ǉ�
	Ver 4.01 	2010.02.24	R_calip_rotate_point_integer�ǉ�				by nakahara
				2011.01.05	R_calip_mpoint_open()�ŁAR_POINT�̃A�h���X�ݒ肪�Ԉ���Ă����̂��C��	by sassa
	Ver4.01		2013.05.16	R_calip_intersection x���ɐ����ȏꍇ�̃o�O�C��
							R_stdlib��R_DLINE��ǉ�
							R_calip_find_line�̓����̏�����R_calip_find_line_fine�ֈڍs				by komatsu
******************************************************************************/

#ifndef	R_CALIP_H_
#define	R_CALIP_H_

#include "R_robust.h"
#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define R_MINIMUM_SQUARE_MODE		0
#define R_ROBUST_STATICS_MODE		0x01
#define R_MAX_AREA_DETECT			0
#define R_MAX_VALUE_DETECT			0x02

// --- define ---

//LegacyFunction	->	R_DLINE���g�p���Ă�������
typedef struct {
	int c;
	float a;
	float b;
} R_LINEF;

typedef double (*DFUNC)();

typedef struct{
	int amp;			//�u���[�u����
	int start_pos;		//�u���[�u�J�n�ʒu
	int end_pos;		//�u���[�u�I���ʒu
	int peak_pos;		//�u���[�u�s�[�N�ʒu
	int area;			//�u���[�u�ʐ�
}R_CALIP_PROTRUDE_RESULT;
typedef struct{
	R_LINE lineeq;		//�����
	R_CALIP_PROTRUDE_RESULT	convex;
	R_CALIP_PROTRUDE_RESULT	concave;
}R_CALIP_PROTRUDE;


typedef struct{
	int nSrcPointNumber;			//���p�`�̒��_��
	R_POINT *pointSrcPnt;			//���p�`�̒��_���W
	int *npAntiPodalStartNumber;	//�e���_�ɑ΂���Anti-podal point�̌��_�̊J�n���_�ԍ�
	int *npAntiPodalEndNumber;		//�e���_�ɑ΂���Anti-podal point�̌��_�̏I���ԍ�
	int	nMemorySize;				//�n���h���̃������T�C�Y
}R_CALIP_RC;

// --- variable ---
#ifdef _R_CALIP_FILE_
	ROBUST_CONDITION RCalipRobustCondition;
	int *RCalipData = NULL;
	R_POINT RCalipPoint[1024];
	double *RCalipSplineTable_P;
	double *RCalipSplineTable_A;
	double *RCalipSplineTable_B;
	double *RCalipSplineTempD;
	double *RCalipSplineTempH;
	double *RCalipCoX;
	double *RCalipCoY;
#else
	extern ROBUST_CONDITION RCalipRobustCondition;
	extern int *RCalipData;
	extern R_POINT RCalipPoint[];
	extern double  *RCalipSplineTable_P;
	extern double  *RCalipSplineTable_A;
	extern double  *RCalipSplineTable_B;
	extern double  *RCalipSplineTempD;
	extern double  *RCalipSplineTempH;
	extern double *RCalipCoX;
	extern double *RCalipCoY;
#endif

int R_calip_find_line( R_POINT *pt, int pt_num, R_LINEF *res, int mode );							//�������oA
int R_calip_find_intersection( R_LINEF sline, R_LINEF dline, R_POINT *pt );							//�Q�����̌�_A
int R_calip_find_intersection_B(R_LINE line1, R_LINE line2, int *x, int *y);						//�Q�����̌�_(�����a�j
int R_calip_check_clockwise( R_POINT a, R_POINT b, R_POINT c );										//�_�̃x�N�g���ɑ΂��鎞�v��葶�ݔ���
int R_calip_tri_area( R_POINT a, R_POINT b, R_POINT c );											//�O�p�`�̖ʐ�
int R_calip_thinning( unsigned char *sp, unsigned char *dp, R_BOX area );							//�א���
int r_cconc( int inb[] );																			//�א��������֐�(�Ώۓ_�̎��͊m�F�����̒l�������邩)
int R_calip_convex( int num, R_POINT *p, R_POINT *res);												//�ʕ�
int R_calip_inner_product( R_POINT p1, R_POINT p2, R_POINT q1, R_POINT q2 );						//�x�N�g������
int R_calip_outer_product( R_POINT p1, R_POINT p2, R_POINT q1, R_POINT q2 );						//�x�N�g���O��
int R_calip_distance_l2p( R_POINT p1,R_POINT p2, R_POINT org,int *distance);						//�_�ƒ����Ƃ̋���(�����F�Q�_����)
int R_calip_distance_l2p_A( R_LINEF line, R_POINT org, int *distance );								//�_�ƒ����Ƃ̋���(�����FA)
int R_calip_distance_l2p_B(R_LINE line, R_POINT p);													//�_�ƒ����Ƃ̋���(�����FB)
int R_calip_distance_l2p_sum(R_LINE line, R_POINT *p, int n);										//�����̓_�ƒ����̋��������l�̕��ς����߂�
int R_calip_distance_p2p(R_POINT p0, R_POINT p1);													//�_�Ɠ_�̋���
int R_calip_distance_l2l(R_LINE line0, R_LINE line1);												//���s�Q�����̋���
int R_calip_get_coX_fromline(R_LINE tLine, int nY, int *pX);										//������̓_��X���W�����߂�(Y���W��^�����Ƃ�)
int R_calip_get_coY_fromline(R_LINE tLine, int nX, int *pY);										//������̓_��Y���W�����߂�(X���W��^�����Ƃ�)
int R_calip_stat_data_mode( int *data, int start, int end );										//�f�[�^��ŕp�l
int R_calip_stat_data_max( int *data, int start, int end );											//�f�[�^��ő�l
int R_calip_stat_data_min( int *data, int start, int end );											//�f�[�^��ŏ��l
double R_calip_stat_data_stdev( int *data, int start, int end );									//�f�[�^��W���΍�
double R_calip_stat_data_average( int *data, int start, int end );									//�f�[�^�񕽋�
int R_calip_stat_data_movingave( int *data_src, int *data_dst, int start, int end, int step );		//�f�[�^��ړ����ς��s��
int R_calip_find_conic(R_DPOINT p[], int num, double a[]);											//�ŏ����@�ɂ��~�������ߎ�
int R_calip_calc_conicinfo(double a[], double p[]);													//�~���Ȑ�����̏��v�Z
void R_calip_shift_line(R_LINE line, int length, int *result);										//��������苗�����s�ړ������Ƃ��̐ؕ�
int R_calip_point_in_square( R_POINT org, R_POINT *pt );												//�C�ӂ̓_�̂S�p�`�����ݔ���
int R_calip_rectangle_area( R_POINT *pt );															//�C�ӎl�p�`�̖ʐ�
void R_calip_data_swap_n( int* x0, int* x1 );														//�Q�̃f�[�^�̓���ւ�(int)
void R_calip_data_swap_d( double* x0, double* x1 );													//�Q�̃f�[�^�̓���ւ�(double)
int R_calip_rotate_point( R_DPOINT *p_target, R_DPOINT p_org, double radian );						//���W�̉�](���W�A��)
int R_calip_gauss_elimination(double a[], double b[], double x[], int n);							//�A���P��������������
int R_calip_find_lineH(R_POINT *p, int n, R_LINE *line);											//�ŏ����@�ɂ�钼���ߎ�(�O�`�S�T�j
int R_calip_find_lineV(R_POINT *p, int n, R_LINE *line);											//�ŏ����@�ɂ�钼���ߎ�(�S�T�`�X�O�j
int R_calip_find_line_fineH(R_POINT *p, int n, R_DLINE *line);										//�ŏ����@�ɂ�钼���ߎ�(�O�`�S�T�j�ڍה�
int R_calip_find_line_fineV(R_POINT *p, int n, R_DLINE *line);										//�ŏ����@�ɂ�钼���ߎ�(�S�T�`�X�O�j�ڍה�
int R_calip_protrude_open( int max_number );														//�P�����f�[�^�_�񉚓ʌ��o�֐��̃I�[�v��
int R_calip_protrude_line( int *data, int number, int mode, int thr_offset_convex, int thr_offset_concave, R_CALIP_PROTRUDE *res );		//�P�����f�[�^�z��̋ߎ���������̓ˋN�����v�Z���܂�
int R_calip_protrude_disp_result( int *data, int number, int y_offset, R_CALIP_PROTRUDE *res );			//�ʉ����ʂ̕`��
int R_calip_calc_intercept(R_POINT *point, int number, int a0, int b0, int64_t *c0 );					//�X�������m�̒����̐ؕЂ��ŏ����@�ɂ�苁�߂�	2017.04.08	komatsu	c��longlong�Ɋg��
void R_calip_calc_intercept_bylength(R_POINT p, R_LINE line, int length, int *result);				//�X���A�w�肵���_����̋��������m�̒����̐ؕЂ����߂�
int R_calip_find_circle(R_DPOINT p[], int number, double *result_coeff );							//�ŏ����@�ɂ��~�ߎ�
int R_calip_spline_open( int max_number );															//3��-B�X�v���C���֐��̏�����
void R_calip_spline_close( void );																	//3��-B�X�v���C���֐��̏I��
int R_calip_spline_2d( R_POINT *s_pnt, int number, int div, R_DPOINT *d_pnt );						//3��-B�X�v���C���֐��ɂ����
int R_calip_intersection_line_on_rect( R_RECT *rect, R_LINE *l, R_POINT *point1, R_POINT *point2 );	//�����ƒ����`�̌�_
int R_calip_point_in_rect( R_POINT *point, R_RECT *rect );											//�C�ӂ̓_�̒����`�����ݔ���
int R_calip_remove_far_point(int nNumSrc, R_POINT *pPointSrc, int *pNumDst, R_POINT *pPointDst, int nDistance, int nLength);//�A�����Ă���_�񂩂�O��_����������

int R_calip_rc_ready( int hConvexHandle, int *nRcHandle );											//Rotate Caliper�g�p����
int R_calip_rc_release( int nRcHandle );															//Rotate Caliper�I��
int R_calip_rc_get_width( int hRcHandle, R_POINT *pEdgePoint, R_POINT *pVertex, int *pWidth );		//�ʑ��p�`��"���h�v�Z
void R_calip_calc_parallel_line_through_point( R_POINT *pSrcPoint, R_LINE *pSrcLine, R_LINE *pDstLine);	//���钼���ɕ��s�ȁA����_��ʂ钼���̎����v�Z
int R_calip_calc_line_with_2point( R_POINT *pSrcPoint1, R_POINT *pSrcPoint2, R_LINE *pLine );		//2�_��ʂ钼���̎�
int R_calip_mpoint_open( int number );																//R_MPOINT������(�n���h����Ԃ�)
void R_calip_mpoint_close( int hMpointHandle );														//R_MPOINT���
int R_calip_mpoint_show_padd( int hMpointHandle );													//R_MPOINT�n���h������_�Q(R_POINT *)�̐擪�A�h���X�𓾂�
int R_calip_mpoint_show_curnumber( int hMpointHandle );												//R_MPOINT�n���h������_�Q(R_POINT *)�̌��ݗL���ȓ_�̐��𓾂�
int R_calip_mpoint_show_maxnumber( int hMpointHandle );												//R_MPOINT�n���h������_�Q(R_POINT *)�̍ő�i�[�\�ȓ_�̐��𓾂�
int R_calip_convex_mpoint( int hMultiPoint, int hConvexPoint );										//R_MPOINT�n���h����R_MPOINT�n���h���̓ʕ�
int R_calip_draw_mpoint( int color, int hMultiHandle, int mode );									//R_MPOINT�n���h���̓_�Q�`��
int R_calip_draw_connect_mpoint( int color, int hMultiHandle, int mode, ULONG pattern );			//R_MPOINT�n���h���̓_�Ԃ����Ԑ����̕\��

int R_calip_line_l2d(R_LINE *pSrcLine,double radian,R_LINE *pDstLine);								//���m�̒����Ɏw��p�x�̂Ȃ����������߂�
double R_calip_degree_l2l(R_LINE *pLine1,R_LINE *pLine2);											//�Q�����̂Ȃ��p�����߂�

int R_calip_intersection( R_LINE l1, R_LINE l2, R_POINT *pt );										//�Q�����̌�_(256�{�l)
int R_calip_intersection_fine( R_DLINE *l1, R_DLINE *l2, R_DPOINT *pt );							//�Q�����̌�_(���������_)
void R_calip_multi_point_compress( int nNumber, R_POINT *pPoint, int nScale );						//�_�Q�̃X�P�[���_�E��
void R_calip_change_line_with_org( R_LINE *pSrcLine, R_LINE *pDstLine, R_POINT *pSrcOrg, R_POINT *pDstOrg );		//���_�̈ړ��ɏ]���Ē����̕��������Čv�Z	//2009.08.07 by nakahara

void R_calip_point_swap( R_POINT *point1, R_POINT *point2 );
int R_calip_rotate_point_integer( R_POINT tSrcPoint, R_POINT tCenter, int nAngle, R_POINT *pDstPoint );		//�_�̉�]

#ifdef __cplusplus
}
#endif

#endif	// R_CALIP_H_
