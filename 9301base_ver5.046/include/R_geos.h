/******************************************************************************
	R_geos.h / R_geos.c
	
	�ύX����
	Ver 3.28	2001/02/05	�V�K�쐬
	Ver 3.50	2009/02/10	DOS & LINUX �\�[�X����
******************************************************************************/

// �􉽊w�}�b�`���O���C�u����

#ifndef	R_GEOS_H_
#define	R_GEOS_H_

#define GEOS_MAX_TEMPLATE	100
#define GEOS_COARSE_MODE	0
#define GEOS_DETAIL_MODE	1
#define WITH_ROTATE			0
#define WITHOUT_ROTATE		1

typedef struct {
	int				name;				//	�T�[�`�p�^�[���̖��O
	int				org_x;				//	�T�[�`�p�^�[���̌��_�����W(�T�[�`�̈撆�S)
	int				org_y;				//	�T�[�`�p�^�[���̌��_�����W(�T�[�`�̈撆�S)
	int				search_xs;			//	�T�[�`�p�^�[���̈捶�゘���W
	int				search_ys;			//	�T�[�`�p�^�[���̈捶�゙���W
	int				search_xw;			//	�T�[�`�p�^�[���̈悘�����T�C�Y
	int				search_yw;			//	�T�[�`�p�^�[���̈悙�����T�C�Y
	int				pat_xs;				//	�p�^�[�����S�����W
	int				pat_ys;				//�@�p�^�[�����S�����W
	int				pat_xw;				//	�p�^�[���������T�C�Y
	int				pat_yw;				//	�p�^�[���������T�C�Y
	int				edge_th;			//	�G�b�W�摜�������l
	int				edge_num;			//	�G�b�W�_��
	int				max_r;				//	�n�t�ϊ��e�[�u���ő傒
	int				angle_ratio;		//	�p�x�q�X�g�O�������o�~���l��
	unsigned short* hough_tbl;			//	�n�t�ϊ��e�[�u��
	unsigned long*	hough_angle_hist;	//	�n�t�ϊ��e�[�u���̊p�x�q�X�g�O����
	unsigned char*	image;				//	�o�^�摜
	signed short*	heisin_data[5];		//	���i��ʉ��n�t�o�^�f�[�^
	int				heisin_data_num[5];	//	���i��ʉ��n�t�o�^�f�[�^��
	int				saisyu_ly;
	int				kouho_ly;
	int				mode;				//�������[�h�i�O�F��]�t���P�F��]�Ȃ��j
}GEOS_SEARCH_PATTERN;

typedef struct {
	int		x;
	int		y;
	int		angle;
	int		val;
}GEOS_SEARCH_RESULT;

#ifdef GEOS_FILE
	unsigned char 		*pGeosBufferGray[3];
	unsigned short 		*pGeosHoughTbl;
	unsigned long 		*pGeosHoughAngleHist;
	unsigned short 		*pGeosHeisinTbl;
	
	int					geos_window_xw;
	int					geos_window_yw;
	int					geos_window_xs;
	int					geos_window_ys;
	int					geos_search_pat_count;
	int					geos_pat_size[ GEOS_MAX_TEMPLATE ];
	GEOS_SEARCH_PATTERN *geos_pat[ GEOS_MAX_TEMPLATE ];
#else
	extern unsigned char 		*pGeosBufferGray[3];
	extern unsigned short 		*pGeosHoughTbl;
	extern unsigned short 		*pGeosHeisinTbl;
	extern unsigned long 		*pGeosHoughAngleHist;
	extern int					geos_window_xw;
	extern int					geos_window_yw;
	extern int					geos_window_xs;
	extern int					geos_window_ys;
	extern int					geos_search_pat_count;
	extern int					geos_pat_size[ GEOS_MAX_TEMPLATE ];
	extern GEOS_SEARCH_PATTERN *geos_pat[ GEOS_MAX_TEMPLATE ];
#endif

int R_geos_open( void );
int R_geos_usepat( int name, int xs, int ys, int xsize, int ysize ,int th,unsigned char *mask);
int R_geos_search( int pn , GEOS_SEARCH_RESULT *result,unsigned char *mask);
void R_geos_draw_box(int org_x,int org_y,int xsize,int ysize,int thita);
int R_geos_change_search_mode( int pn, int mode );
int R_geos_save_template( int pn, char *file_name );
int R_geos_load_template(char* file_name);
int R_geos_change_kouho_ly( int pn,int ly );
int R_geos_change_saisyu_ly( int pn,int ly );

// �v���g�^�C�v�錾�ǉ� 2009/07/21
int		R_geos_draw_pattern(int pn);
int		R_geos_draw_houghtbl(int pn);
int		R_geos_draw_anglehist(int pn);
int		R_geos_get_table_data(char* file_name);

// �����֐��v���g�^�C�v�錾�ǉ� 2009/07/15
int		r_geos_hough_tbl_clear(unsigned short* tbl, int xsize, int ysize);
int		r_geos_hough_hist_clear(unsigned long* hist, int size);
int		r_geos_hough_voting(int xs, int ys, int xw, int yw, int org_x, int org_y, int th, int max_r, unsigned char* sp,unsigned short* tbl);
int		r_geos_get_hough_angle_hist(unsigned short* tbl, unsigned long* hist, int rsize, int p_th);
int		r_geos_get_heisin_data(int xs, int ys, int xw, int yw, unsigned char* sp, int pn, int layer);
int		r_geos_resize_image(unsigned char* sp, unsigned char* dp, int bai);
int		r_geos_get_angle(unsigned long* tmp, unsigned long* trg, int* angle, int* val, int mode);
int		r_geos_generalized_voting(int xs, int ys, int xw, int yw, signed short* data, int num, unsigned char* sp, int thita, unsigned short* tbl, int ly);
int		r_geos_get_max_position(unsigned short* tbl, int* ox, int* oy,int xsize, int ysize);
// �����֐��v���g�^�C�v�錾�ǉ� 2009/07/21
int		r_geos_cut_image( unsigned char *sp, unsigned char *dp, int xs, int ys, int xw, int yw);
int		r_geos_draw_any_houghtbl( unsigned short *houghtbl ,int rsize );
int		r_geos_draw_any_anglehist( unsigned long *hist_org ,int angle);
void	r_geos_draw_heisin_data( signed short *data, int num ,int org_x,int org_y);
int		r_geos_draw_any_heisintbl( unsigned short *houghtbl,int xsize,int ysize);

#endif	// R_GEOS_H_
