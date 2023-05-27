/******************************************************************************
	R_gray.c / R_projec.c / R_gray.h

	�X�V����
	Ver.3.48	2006.05.08	R_projection_x/y()�̃v���g�^�C�v�錾�Ŗ߂�l�̎w�肪�Ȃ������̂Œǉ�  by matsu
	Ver.3.49	2006.12.20	hav4_mmx�֐��ǉ� by tanaka
	Ver.3.50	2009.02.12	DOS & LINUX �\�[�X����
	Ver.4.01	2017.04.24	MAX_TEMPLATE_TIMES��200����999�ɕύX
******************************************************************************/

#ifndef	R_GRAY_H_
#define	R_GRAY_H_

#include <stdio.h>	// 2009/07/21�ǉ�

#include "R_rotate.h"

#ifdef __x86_64__
#include <stdint.h>
#include "R_plane.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//#define		NULL	2009/07/21 �R�����g�A�E�g

#define		OK		0
#define		ERROR	(-1)
#define		YES		1
#define		NO		0


#define		INITIAL_PTN_AREA	0
#define		CONTINUE_PTN_AREA	1


#define		LAYER1		0x01
#define		OUT_OF_FOCUS 0x80
#define		NON_LAYER4	0x10
#define		NON_LAYER8	0x08
#define		NON_LAYER16	0x00

// #define		LAYER1		1
#define		LAYER2		2
#define		LAYER4		4
#define		LAYER8		8
#define		LAYER16		16

//#define		MAX_TEMPLATE_TIMES	200			// 
#define		MAX_TEMPLATE_TIMES	999			// 2017.04.24

#define		IS_8BIT_PROCESS	0x48

typedef struct {
	int			name;		/*	�T�[�`�p�^�[���̖��O	*/
	int			org_x;		/*	�؂�o�������̍���̂w���W	*/
	int			org_y;		/*	�؂�o�������̍���̂x���W	*/
	int			x_size;		/*	�T�[�`�p�^�[���̂w�����̕�	*/
	int			y_size;		/*	�T�[�`�p�^�[���̂x�����̕�	*/
	int			x_offset;	/*	�}�[�N�����鍶�ォ��̃I�t�Z�b�g�w���W�̂P�O�{�l	*/
	int			y_offset;	/*	�}�[�N�����鍶�ォ��̃I�t�Z�b�g�x���W�̂P�O�{�l	*/
	int			area_size;	/*	�G���A�T�C�Y���̍\���́{�摜�������[�T�C�Y	*/
	int			search_mode; /*	�T�[�`���[�h	*/
	unsigned char	*a1;	/*	���摜�̐擪�A�h���X�@�摜�f�[�^�[�́A�UBIT�Ɉ��k����Ċi�[����܂��B	*/
	unsigned char	*a2;	/*	�P�^�Q�Ɉ��k�����摜�̐擪�A�h���X	*/
	unsigned char	*a4;	/*	�P�^�S�Ɉ��k�����摜�̐擪�A�h���X	*/
	unsigned char	*a8;	/*	�P�^�W�Ɉ��k�����摜�̐擪�A�h���X	*/
	unsigned char	*a16;	/*	�P�^�P�U�Ɉ��k�����摜�̐擪�A�h���X	*/
	int			search_xs;	/*	�T�[�`�E�B���h�[�@�w�n�[�ʒu�Q�̔{��	*/
	int			search_ys;	/*	�T�[�`�E�B���h�[�@�x�n�[�ʒu�Q�̔{��	*/
	int			search_xw;	/*	�T�[�`�E�B���h�[�@�w�T�C�Y�P�U�̔{��	*/
	int			search_yw;	/*	�T�[�`�E�B���h�[�@�x�T�C�Y�P�U�̔{��	*/
	int			kouho_ly;	/*	���_�T�[�`�̃��C���[	*/
	int			saisyu_ly;	/*	�ŏI�_�T�[�`�̃��C���[	*/
	int			kouho_low;	/*	���_�T�[�`�̑��֌W�������l	*/
	int			saisyu_low;	/*	�ŏI�_�T�[�`�̑��֌W�������l	*/
	int			kouho_limit;/*	���_�T�[�`�̍�����Βl	*/
	union {
		int			reserve;	
		unsigned char	mode[4];	// mode[0] �f�[�^�����r�b�g��
	};
	int			org_sabun[5];// �e���C���[�̍�����Βl�̃��~�b�g�̊���
	int			org_soukan[5];// �e���C���[�̑��֒l�̍ň��l
	int			avg;		/*	���邳�̕��ϒl           �Q 2	*/
	unsigned int	pmt[5];		/*	�p�����[�^�[�P�@���i�hi�|�h�j	*/
	} GS_SEARCH_PATTERN	;
#define	FILE_SIZE_OF_GS_SEARCH_PATTERN	160

typedef struct {
	int			x;		/*	���������̂w���W�̂P�O�{�l	*/
	int			y;		/*	���������̂x���W�̂P�O�{�l	*/
	int			val;	/*	���֌W���l(�`10000)	*/
	} GS_SEARCH_RESULT	;


#ifdef	GRAY_FILE
	int			gs_search_pat_count=0;		/*	�o�^�ς݃T�[�`�p�^�[����	*/
	int			gs_search_pat_enable=MAX_TEMPLATE_TIMES;	/*	�o�^�\��	*/
	GS_SEARCH_PATTERN	*gs_pat[MAX_TEMPLATE_TIMES];		/*	�T�[�`�p�^�[���̍\���̂̃|�C���^�[	*/
	int			gs_pat_size[MAX_TEMPLATE_TIMES];			/*	�T�[�`�p�^�[���̍\���̂̃T�C�Y	*/
#else
	extern int			gs_search_pat_count;	/*	�o�^�ς݃T�[�`�p�^�[����	*/
	extern int			gs_search_pat_enable;	/*	�o�^�\��	*/
	extern GS_SEARCH_PATTERN	*gs_pat[MAX_TEMPLATE_TIMES];	/*	�T�[�`�p�^�[���̍\���̂̃|�C���^�[	*/
	extern int			gs_pat_size[MAX_TEMPLATE_TIMES];		/*	�T�[�`�p�^�[���̍\���̂̃T�C�Y	*/
#endif

typedef struct {
	int		x;
	int		y;
} SAXIL;

// �ő�T�[�`�T�C�Y		2001.04.18  by matsu
typedef struct {
	int	x;
	int	y;
} SEARCH_MAX_SIZE;

#ifdef __x86_64__
	typedef struct {
		int		py_x;				// �f�[�^�[�̂w
		int		py_y;				// �f�[�^�[�̂x
		int		py_xw;				// �f�[�^�[�̂w��
		int		py_yw;				// �f�[�^�[�̂x��
		int		py_axw;				// ���σf�[�^�[�̂w��
		int		py_ayw;				// ���σf�[�^�[�̂x��
		unsigned char*	py_avg;		// �Q�f�[�^�[�ł̕��ϒl�̐擪�Ԓn
		union {
			unsigned char*	py_a[5];
			struct {
				unsigned char*	py_a1;		// �P�w�ڂ̐��f�[�^�[�̐擪�Ԓn
				unsigned char*	py_a2;		// �P�^�@�Q�f�[�^�[�̐擪�Ԓn
				unsigned char*	py_a4;		// �P�^�@�S�f�[�^�[�̐擪�Ԓn
				unsigned char*	py_a8;		// �P�^�@�W�f�[�^�[�̐擪�Ԓn
				unsigned char*	py_a16;		// �P�^�P�U�f�[�^�[�̐擪�Ԓn
			};
		};
		int		Valsub;				// �T�[�`�������̌��_�����l
		int		avg_calc_mode;		//	0:nomal 1:h_mode 2:sh_mode
		SAXIL*	Axil;
		void*	h_avg_work;
		SEARCH_MAX_SIZE	SearchMaxSize;
		int		bit_depth;			// 6(6bit���Z) or 8(8bit���Z)
	} R_GS_OBJ;
#else

#ifdef __DOS__
	extern short int	py_x	;	/*	�f�[�^�[�̂w	*/
	extern short int	py_y	;	/*	�f�[�^�[�̂x	*/
	extern short int	py_xw	;	/*	�f�[�^�[�̂w��	*/
	extern short int	py_yw	;	/*	�f�[�^�[�̂x��	*/
	extern short int	py_axw	;	/*	���σf�[�^�[�̂w��	*/
	extern short int	py_ayy	;	/*	���σf�[�^�[�̂x��	*/
#elif __GNUC__
	extern int			py_x;		/*	�f�[�^�[�̂w	*/
	extern int			py_y;		/*	�f�[�^�[�̂x	*/
	extern int			py_xw;		/*	�f�[�^�[�̂w��	*/
	extern int			py_yw;		/*	�f�[�^�[�̂x��	*/
	extern int			py_axw;		/*	���σf�[�^�[�̂w��	*/
	extern int			py_ayy;		/*	���σf�[�^�[�̂x��	*/
#endif

extern unsigned char	*py_avg	;	/*	�P�^�@�Q�f�[�^�[�ł̕��ϒl�̐擪�Ԓn	*/
extern unsigned char	*py_a1	;	/*	�P�w�ڂ̐��f�[�^�[�̐擪�Ԓn	*/
extern unsigned char	*py_a2	;	/*	�P�^�@�Q�f�[�^�[�̐擪�Ԓn	*/
extern unsigned char	*py_a4	;	/*	�P�^�@�S�f�[�^�[�̐擪�Ԓn	*/
extern unsigned char	*py_a8	;	/*	�P�^�@�W�f�[�^�[�̐擪�Ԓn	*/
extern unsigned char	*py_a16	;	/*	�P�^�P�U�f�[�^�[�̐擪�Ԓn	*/

extern unsigned char	*GS_SEARCH_ADDRESS;			// �O���[�T�[�`�����p�A�h���X
#endif



int R_make_py_av_mem(short x, short y,short xw, short yw, short bx, short by, int mode) ;

#ifdef __x86_64__
	int	R_gray_make_py_av(R_GS_OBJ*, R_PLANE*, int x, int y, int xw, int yw, int bx, int by, int mode);
	int	R_gray_make_py_av_h(R_GS_OBJ*, R_PLANE*, int x, int y, int xw, int yw, int bx, int by, int mode);
#elif defined(__DOS__)
	int	R_gray_make_py_av(int page, short x, short y,short xw, short yw, short bx, short by, int mode) ;		//ver3.36
	int	R_gray_make_py_av_h(int page, short x, short y,short xw, short yw, short bx, short by, int mode) ;		//ver3.36
	int	R_gray_make_py_av_mmx(int page, short x, short y,short xw, short yw, short bx, short by, int mode) ;	//ver3.36
#elif __GNUC__
	int	R_gray_make_py_av(int page, int x, int y, int xw, int yw, int bx, int by, int mode);
	int	R_gray_make_py_av_h(int page, int x, int y, int xw, int yw, int bx, int by, int mode);
	int	R_gray_make_py_av_mmx(int page, int x, int y, int xw, int yw, int bx, int by, int mode);
#endif

int R_gray_make_py_av_h_mmx(int page, short x, short y,short xw, short yw, short bx, short by, int mode) ;	//ver3.36
int R_gray_make_py_av_sh_mmx(int page, short x, short y,short xw, short yw, short bx, short by, int mode);	//ver3.36
int R_gray_make_py_av_sh(int page, short x, short y,short xw, short yw, short bx, short by, int mode);		//ver3.36
int R_gray_make_py_only(int page, short x, short y,short xw, short yw, short bx, short by, int mode);		//ver3.36

int R_dsp_tp(unsigned char *s_po, short int xw, short int yw, short int x, short int y);
#ifdef __DOS__
	int R_dsp_tp_v(unsigned char *s_po, short int xw, short int yw, short int x, short int y,int bright, int expand);
#elif __GNUC__
	int R_dsp_tp_v(unsigned char*s_po, int xw, int yw, int x, int y, int bright, int expand);
#endif
int R_dspsvga_tp_v(unsigned char *s_po, int xw, int yw, int x, int y,int bright, int expand);
int R_gs_defadrs( int mode );
int R_gray_make_template( int page, short x, short y,short xw, short yw ) ;									//ver3.36

#define R_make_py_av( x, y, xw, yw, bx, by, mode) R_gray_make_py_av(SearchGrayPage, x, y, xw, yw, bx, by, mode)				//ver3.36
#define R_make_py_av_mmx( x, y, xw, yw, bx, by, mode) R_gray_make_py_av_mmx(SearchGrayPage, x, y, xw, yw, bx, by, mode)		//ver3.36
#define R_make_py_av_h(x, y, xw, yw, bx, by, mode) R_gray_make_py_av_h(SearchGrayPage, x, y, xw, yw, bx, by, mode)			//ver3.36
#define R_make_py_av_h_mmx(x, y, xw, yw, bx, by, mode) R_gray_make_py_av_h_mmx(SearchGrayPage, x, y, xw, yw, bx, by, mode)	//ver3.36
#define R_make_py_av_sh_mmx(x, y, xw, yw, bx, by, mode) R_gray_make_py_av_sh_mmx(SearchGrayPage, x, y, xw, yw, bx, by, mode)	//ver3.36
#define R_make_py_av_sh(x, y, xw, yw, bx, by, mode) R_gray_make_py_av_sh(SearchGrayPage, x, y, xw, yw, bx, by, mode)			//ver3.36
#define R_make_py_only(x, y, xw, yw, bx, by, mode) R_gray_make_py_only(SearchGrayPage, x, y, xw, yw, bx, by, mode)			//ver3.36
#define R_make_template( x, y, xw, yw ) R_gray_make_template( SearchGrayPage, x, y, xw, yw )									//ver3.36

//	�ȉ������g�p��p�i���[�U�[����͎g�p���܂���I�I�j

/*	�v���g�^�C�v�錾�@�\�[�X�t�@�C���F�q�Q��������.�b	*/


char *g2p4(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);
void r_gs_close(void);


/*	�v���g�^�C�v�錾�@�\�[�X�t�@�C���F���Q�����Q����.������	*/


// R_gs_py.asm
//char *R_g2p4_filter(char *sp1, char *d_p, int y_k, int k);
char*	R_g2p4_filter(unsigned char* sp1, unsigned char* d_p, int y_k, int k);	// 2009/07/27
//char *p12p2av(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char*	p12p2av(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);	// 2009/07/27
//char *R_avh2av(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_y);
char*	R_avh2av(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_y);	// 2009/07/27
//char *p12p2av_div(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	p12p2av_div(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *R_avh2av_div(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	R_avh2av_div(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *p12p2av_96(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	p12p2av_96(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *g2p2av(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char*	g2p2av(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);		// 2009/07/27
//char *g2p2av_96(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	g2p2av_96(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);		// 2009/07/27
//char *g2p2av_div(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	g2p2av_div(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *R_avh2av_96(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	R_avh2av_96(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *R_g2p4(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);
char*	R_g2p2(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);	// 2009/07/27
char*	R_g2p4(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);	// 2009/07/27
#ifdef __GNUC__
	char*	R_g2p4_filter_(unsigned char* sp1, unsigned char* d_p, int y_k, int k);
	char*	R_avh2av_div_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	g2p2av_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);
	char*	p12p2av_96_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	g2p2av_96_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	R_avh2av_96_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	R_g2p2_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);
	char*	R_g2p4_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);
#endif

char *p12p2av_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);

//R_gspymx.asm
//char *R_g2p4_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);
char*	R_g2p4_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);	// 2009/07/27
//char *hav4_mmx(         char *sp2,char *d_p,int x_k, int y_k,        int m, int dev_x,short *w);
char*	hav4_mmx(unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int m, int dev_x, short* w);		// 2009/07/27
//char *hav4_div_mmx(         char *sp2,char *d_p,int x_k, int y_k,        int m, int dev,short *w);
char*	hav4_div_mmx(unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int m, int dev, short* w);	// 2009/07/27
//char *R_g2p2_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);
char*	R_g2p2_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);	// 2009/07/27
//char *R_avh2av_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_y);
char*	R_avh2av_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_y);	// 2009/07/27
//char *R_avh2av_div_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	R_avh2av_div_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *hav_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char*	hav_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);	// 2009/07/27
//char *hav_div_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	hav_div_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//void g2p1_c8_mmx(char *sp1,char *sp2,int xw, int yw);
void	g2p1_c8_mmx(unsigned char* sp1, unsigned char* sp2, int xw, int yw);
//void g2p1_c16_mmx(char *sp1,char *sp2,int xw, int yw);
void	g2p1_c16_mmx(unsigned char* sp1, unsigned char* sp2, int xw, int yw);
#ifdef __GNUC__
	char*	hav4_mmx_(unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int m, int dev, short* w);
	char*	hav_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);
	char*	hav4_div_mmx_(unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int m, int dev, short* w);
	char*	R_avh2av_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_y);
	char*	R_avh2av_div_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	R_g2p4_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);
	char*	R_g2p2_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);
	char*	hav_div_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	void	g2p1_c8_mmx_(unsigned char* sp1, unsigned char* sp2, int xw, int yw);
	void	g2p1_c16_mmx_(unsigned char* sp1, unsigned char* sp2, int xw, int yw);
#endif


char *g2p4_filter(char *sp1,char *d_p, int y_k, int k);
char *R_g2p22_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);

// R_gray.c
#ifndef __x86_64__
//void g2p1_c(char *sp1,char *sp2,int xw, int yw);
void	g2p1_c(unsigned char* sp1, unsigned char* sp2, int xw, int yw);	// 2009/07/27
//char *g2pav_filter(char *sp1, char *sp2, char *d_p, int y_k, int k, int m, int dev);
unsigned char*	g2pav_filter(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int y_k, int k, int m, int dev);	// 2009/07/27
//char *g2pav_div_filter(char *sp1, char *sp2, char *d_p, int y_k, int k, int m, int dev);
unsigned char*	g2pav_div_filter(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int y_k, int k, int m, int dev);	// 2009/07/27
#endif




//	R_make_pa_,R_make_pa_new_		�����Ă��Ȃ�

/*	�v���g�^�C�v�錾�@�\�[�X�t�@�C���F���Q�����Q������.������	*/
int gs_get_val_k(  unsigned char *img_p, long img_x, unsigned char img_avg,
			      unsigned char *tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
#ifdef __DOS__
	int	gs_get_val(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
	int	gs_get_val_mmx(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
	int	gs_get_valsub(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long limmit);
	int	gs_get_valsub_b(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long limmit);
	int	gs_get_val_b(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
	int	gs_get_val_b_mmx(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
#elif defined(__GNUC__) && defined(__i386__)
	int	gs_get_val(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
	int	gs_get_val_mmx(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
	int	gs_get_valsub(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long limmit);
	int	gs_get_valsub_b(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long limmit);
	int	gs_get_val_b(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
	int	gs_get_val_b_mmx(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
#endif

#if defined(__GNUC__) && defined(__i386__)
	int gs_get_val_mmx_(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
	int gs_get_val_b_mmx_(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
#endif




char *g2p2av_c(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char *g2p2av_c8(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char *g2p1p2av(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x,char *dp1);
void R_avh2av_c(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_y);

// r_gsb.asm
void R_gray2bin(unsigned char sho, int x, int y, int xs, int ys, unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin16(unsigned char sho, int x, int y, int xs, int ys, unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin_r(unsigned char sho, int x, int y, int xs, int ys, unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin16_r(unsigned char sho, int x, int y, int xs, int ys, unsigned char *g_adr, unsigned char * b_adr);


// R_projec.c
int R_projection_x( int page, int x1, int y1, int x2, int y2, int *x_project);		// �߂�lint�ǉ� 2006.05.08  by matsu
int R_projection_y( int page, int x1, int y1, int x2, int y2, int *y_project);		// "

// R_projea.asm
int	r_projc_x_mmx1(unsigned long *pntr,int *x_project,int H_step,int y2,unsigned long *masks);
int	r_projc_x_mmx( unsigned long *pntr,int *x_project,int H_step,int y2,unsigned long *masks,unsigned long *maske,int xe);
int	r_projc_x_sse1(unsigned long *pntr,int *x_project,int H_step,int y2,unsigned long *masks);
int	r_projc_x_sse( unsigned long *pntr,int *x_project,int H_step,int y2,unsigned long *masks,unsigned long *maske,int xe);
int r_projc_ysum_sse2(unsigned long *p,int y1,int H_step,int *sum);
int r_projc_ysum_mmx(unsigned long *p,int y1,int H_step,int *sum);
#ifdef __GNUC__
	int	r_projc_x_mmx1_(unsigned long* pntr, int* x_project, int H_step, int y2, unsigned long* masks);
	int	r_projc_x_mmx_(unsigned long* pntr, int* x_project, int H_step, int y2, unsigned long* masks, unsigned long* maske, int xe);
	int	r_projc_x_sse1_(unsigned long* pntr, int* x_project, int H_step, int y2, unsigned long* masks);
	int	r_projc_x_sse_(unsigned long* pntr, int* x_project, int H_step, int y2, unsigned long* masks, unsigned long* maske, int xe);
	int	r_projc_ysum_sse2_(unsigned long* p, int y1, int H_step, int* sum);
	int	r_projc_ysum_mmx_(unsigned long* p, int y1, int H_step, int* sum);
#endif

// r_rotate.c
// void R_RotateGrayMemory(int sp, int dp, int xc, int yc, int bs, int bb);

//R_projec.c
int R_projection_bin_x( int page, int x1, int y1, int x2, int y2, int *x_project);
int R_projection_bin_y( int page, int x1, int y1, int x2, int y2, int *y_project);

#ifdef __x86_64__
uint64_t	RProjectionX(R_PLANE* plane, R_RECT* rect, int* data);
uint64_t	RProjectionY(R_PLANE* plane, R_RECT* rect, int* data);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_GRAY_H_
