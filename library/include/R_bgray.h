/*	�q�Q�a�f�q�`�x.�g
	�摜�v�����C�u�����[�̃w�b�_�[�t�@�C��
	�q�X�g�O�����v���A�Q�l���A�Q�l���c�[���Ȃ�

�X�V����
98. 3.12				R_gray2bin	MMX�Ή��̂��ߕ��򕔕��Ȃǂb�ɏ���������.
99.09.13				OLDC_Ver�ȊO�̊֐��̑Ή�  by matsu
00.09.14				R_gray2bin16_mmx_c() <OLDC_Ver��> �� emms �����C�� by matsu
03.07.16				R_get_binlevel_by_DC()	�ǉ�	by shimizu
03.08.12				R_get_binlevel_by_mode()	�ǉ�	by shimizu
04.01.07				R_bgray_thresholding�֐��Q�ǉ� by sassa
05.02.15				R_bgray_thresholding_demo�C�� by shimizu
06.01.23				R_gray2bin16 SSE2���߂ŋL�ڂ����֐���ǉ� by tanaka
06.01.25	Ver.3.47	R_gray2bin/_r/16_r ��SSE2���߂ŋL�ڂ����֐���ǉ� by tanaka
06.03.06	Ver.3.48	R_bgray_thresholding_open()�łQ�d���������֎~����悤�ɏC��  by mastu
06.09.20	Ver.3.49	R_count_gray_area  ��SSE2���߂ŋL�ڂ����֐���ǉ� by tanaka
09.04.02	Ver.3.50	DOS & LINUX �\�[�X����
16.06.13	Ver.4.01	R_bgray_thresholding_level_value�֐��ǉ�
*/

#ifndef	R_BGRAY_H_
#define	R_BGRAY_H_

#ifdef __cplusplus
extern "C" {
#endif

//for R_bgray_thresholding
#define	THRESH_OTSU			0
#define	THRESH_SAITO		1
#define THRESH_KITTLER		2
#define THRESH_DEPTH_8BIT	256

//R_get_binlevel_by_DC�p
#define	OTSU	0
#define	WMMTM	1
#define	SE_CALC	0x10000000



#ifdef	R_BGRAY
	int	Bright[257];						/* �Z�x�O�`�Q�T�T�ɂ������f�� */
	int Separation[256];					/* DC�@�ł̕����x */
#else
	extern int	Bright[257];				/* �Z�x�O�`�Q�T�T�ɂ������f�� */
	extern int Separation[256];
#endif





int R_get_histgram( int xs, int ys, int xe, int ye, unsigned char *address) ;
int R_get_histgram1( int xs, int ys, int xe, int ye, unsigned char *address);
int R_get_step_histgram( int xs, int ys, int xe, int ye, unsigned char *address) ;
int R_pset_histgram( int off_set, int max_level, int table[]) ;
void R_avg_histgram( int count,int table[]) ;
int R_calc_gray2bin_level( int table[]) ;
int R_calc_gray2bin_level_r( int table[]) ;
int R_calc_gray2bin_level_2( int sh,int table[]) ;

void R_gray2bin(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin16(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);

void R_gray2bin_r(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin_r_mmx(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin16_r(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
int R_count_gray_area( int sho, int page, int x1, int y1, int x2, int y2 );
int R_get_binlevel_by_DC(int method, int *hist_tbl);
int R_get_binlevel_by_ptile(int area, int *hist_tbl);
int R_bgray_thresholding( int *data_tbl, int start, int end, int method );
int R_bgray_thresholding_open( int size );
int R_bgray_thresholding_close( void );
int R_bgray_thresholding_demo( void );
int R_bgray_thresholding_disp_graph( int offset, int length );
int R_bgray_thresholding_level_value(int* level_value_table);

// R_g2b.asm
void	R_gray2binwm_sse(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr,int mask);
void	R_gray2bin16_sse(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void	R_gray2binwm_mmx(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr,int mask);
void	R_gray2bin16_mmx(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void	R_gray2bin_ia(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void	R_gray2bin16_ia(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void	R_gray2binwm_r_sse(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr,int mask);
void	R_gray2bin16_r_sse(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void	R_gray2binwm_r_mmx(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr,int mask);
void	R_gray2bin16_r_mmx(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void	R_gray2bin_r_ia(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
void	R_gray2bin16_r_ia(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr, unsigned char * b_adr);
int		R_count_graywm_sse(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr,int mask);
int		R_count_gray16_sse(unsigned char sho, int x,int y,int xsize,int ys,unsigned char *g_adr);
#ifdef __GNUC__
	void	R_gray2binwm_sse_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr, int mask);
	void	R_gray2bin16_sse_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr);
	void	R_gray2binwm_mmx_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr, int mask);
	void	R_gray2bin16_mmx_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr);
	void	R_gray2bin_ia_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr);
	void	R_gray2bin16_ia_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr);
	void	R_gray2binwm_r_sse_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr, int mask);
	void	R_gray2bin16_r_sse_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr);
	void	R_gray2binwm_r_mmx_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr, int mask);
	void	R_gray2bin16_r_mmx_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr);
	void	R_gray2bin_r_ia_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr);
	void	R_gray2bin16_r_ia_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, unsigned char* b_adr);
	int		R_count_graywm_sse_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr, int mask);
	int		R_count_gray16_sse_(unsigned char sho, int x, int y, int xsize, int ys, unsigned char* g_adr);
#endif

//	R_ascope.c
void R_ascope(int memory_no);

#ifdef __cplusplus
}
#endif

#endif	// R_BGRAY_H_
