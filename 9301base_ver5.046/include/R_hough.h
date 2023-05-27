/******************************************************************************
	R_hough.h
		�n�t�ϊ����C�u�����[�̃w�b�_�[�t�@�C��
******************************************************************************/

#ifndef R_HOUGH_H_
#define R_HOUGH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define	NORMAL		0
#define FINE    	1
#define APPL_ON    	1
#define APPL_OFF    0

#ifdef __x86_64__
	typedef struct {
		int	lhough_min_r;
		int	lhough_max_r;
		int	lhough_rsize;
		int	lhough_size;
		int	x_mi, x_ma, y_mi, y_ma;		//detection�p�̕ϐ�
		int	off, org_x, org_y;
		unsigned short *lhough_tbl;
	} R_LHOUGH_DATA;
#else
	extern unsigned short *lhough_tbl;
	extern int	lhough_min_r;
	extern int	lhough_max_r;
	extern int	lhough_rsize;
	extern int	lhough_size;
	extern int x_mi, x_ma, y_mi, y_ma;		//detection�p�̕ϐ�
//	proto type declaration
//	r_hough.c
#endif
	typedef struct
				{
					int	a;	/*�W��a���R�Q�V�U�W�{�����R�Q�r�b�g�����l*/
					int b;	/*�W��b���R�Q�V�U�W�{�����R�Q�r�b�g�����l*/
					int c;	/*�W��c��      �P�U�{�����R�Q�r�b�g�����l*/
				} HLINE_T;

#ifdef __x86_64__	// 2012.04.09 by araki
R_LHOUGH_DATA*	R_lhough_open(int xs, int ys, int xe, int ye);
int	R_lhough_close(R_LHOUGH_DATA*);
int	R_lhough_clear(R_LHOUGH_DATA*);
int	R_lhough_voting(R_LHOUGH_DATA*, int x, int y);		//�Ƃ̒P�ʂ͂P�x��
int	R_lhough_voting_fine(R_LHOUGH_DATA*, int x, int y);	//�Ƃ̒P�ʂ͂O�D�T�x��
int	R_lhough_detection(R_LHOUGH_DATA*, int n, int region_r, int region_q, HLINE_T coeff[], int appl);
int	R_lhough_detection_th(R_LHOUGH_DATA*, int n, int region_r, int region_q, HLINE_T coeff[], int appl,int th);
int	R_lhough_detection_fine(R_LHOUGH_DATA*, int n, int region_r, int region_q, HLINE_T coeff[], int appl);
#else
int R_lhough_open( int xs, int ys, int xe, int ye ) ;
int R_lhough_close( void ) ;
int R_lhough_clear( void ) ;
int R_lhough_voting( int x, int y ) ; //�Ƃ̒P�ʂ͂P�x��
int R_lhough_voting_fine( int x, int y ) ; //�Ƃ̒P�ʂ͂O�D�T�x��
int R_lhough_detection( int n, int region_r, int region_q, HLINE_T coeff[], int appl);
int R_lhough_detection_th( int n, int region_r, int region_q, HLINE_T coeff[], int appl,int th);
int R_lhough_detection_fine( int n, int region_r, int region_q, HLINE_T coeff[], int appl);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_HOUGH_H_
