/******************************************************************************
	R_hough.h
		ƒnƒt•ÏŠ·ƒ‰ƒCƒuƒ‰ƒŠ[‚Ìƒwƒbƒ_[ƒtƒ@ƒCƒ‹
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
		int	x_mi, x_ma, y_mi, y_ma;		//detection—p‚Ì•Ï”
		int	off, org_x, org_y;
		unsigned short *lhough_tbl;
	} R_LHOUGH_DATA;
#else
	extern unsigned short *lhough_tbl;
	extern int	lhough_min_r;
	extern int	lhough_max_r;
	extern int	lhough_rsize;
	extern int	lhough_size;
	extern int x_mi, x_ma, y_mi, y_ma;		//detection—p‚Ì•Ï”
//	proto type declaration
//	r_hough.c
#endif
	typedef struct
				{
					int	a;	/*ŒW”a‚ğ‚R‚Q‚V‚U‚W”{‚µ‚½‚R‚Qƒrƒbƒg®”’l*/
					int b;	/*ŒW”b‚ğ‚R‚Q‚V‚U‚W”{‚µ‚½‚R‚Qƒrƒbƒg®”’l*/
					int c;	/*ŒW”c‚ğ      ‚P‚U”{‚µ‚½‚R‚Qƒrƒbƒg®”’l*/
				} HLINE_T;

#ifdef __x86_64__	// 2012.04.09 by araki
R_LHOUGH_DATA*	R_lhough_open(int xs, int ys, int xe, int ye);
int	R_lhough_close(R_LHOUGH_DATA*);
int	R_lhough_clear(R_LHOUGH_DATA*);
int	R_lhough_voting(R_LHOUGH_DATA*, int x, int y);		//ƒÆ‚Ì’PˆÊ‚Í‚P“x–ˆ
int	R_lhough_voting_fine(R_LHOUGH_DATA*, int x, int y);	//ƒÆ‚Ì’PˆÊ‚Í‚OD‚T“x–ˆ
int	R_lhough_detection(R_LHOUGH_DATA*, int n, int region_r, int region_q, HLINE_T coeff[], int appl);
int	R_lhough_detection_th(R_LHOUGH_DATA*, int n, int region_r, int region_q, HLINE_T coeff[], int appl,int th);
int	R_lhough_detection_fine(R_LHOUGH_DATA*, int n, int region_r, int region_q, HLINE_T coeff[], int appl);
#else
int R_lhough_open( int xs, int ys, int xe, int ye ) ;
int R_lhough_close( void ) ;
int R_lhough_clear( void ) ;
int R_lhough_voting( int x, int y ) ; //ƒÆ‚Ì’PˆÊ‚Í‚P“x–ˆ
int R_lhough_voting_fine( int x, int y ) ; //ƒÆ‚Ì’PˆÊ‚Í‚OD‚T“x–ˆ
int R_lhough_detection( int n, int region_r, int region_q, HLINE_T coeff[], int appl);
int R_lhough_detection_th( int n, int region_r, int region_q, HLINE_T coeff[], int appl,int th);
int R_lhough_detection_fine( int n, int region_r, int region_q, HLINE_T coeff[], int appl);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_HOUGH_H_
