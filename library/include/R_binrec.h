/*
�@�Q�l�F���p�w�b�_�t�@�C��
	
	Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
*/

#ifndef	R_BINREC_H_
#define	R_BINREC_H_

#ifdef __cplusplus
extern "C" {
#endif

//	���F�����[�h(LINERCG_PARA.mode)
#define	DIR_SINCOS	0
#define	DIR_X		1
#define	DIR_135		2
#define	DIR_Y		3
#define	DIR_45		4

//	���F���p�����[�^�\����
typedef struct {
	int		page;
	int		x;
	int		y;
	int		level;
	int		noise;
	int		mode;
	long	sind;
	long	cosd;
} LINERCG_PARA;

//�@���F���\����
typedef struct {
	int		runstrt[ 512 ];
	int		runleng[ 512 ];
	int		runcol[ 512 ];

	int		dir;
	int 	rn;
	int		black;
	int		white;

	int		x1;
	int		y1;
	int		x2;
	int		y2;
	int		xc;
	int		yc;
	int		w;
	int		color;
} LINERCG;

#if	defined(R_BINREC)
	LINERCG_PARA	Lrcg_prm;
#else
	extern LINERCG_PARA	Lrcg_prm;
#endif

//	�y�[�W�ݒ�
#define	R_linercg_set_page(p)		Lrcg_prm.page = p;
//	�F���ʒu�ݒ�
#define	R_linercg_set_point(a,b)	Lrcg_prm.x = a;	Lrcg_prm.y = b;
//	�Q�l�����x���ݒ�
#define	R_linercg_set_level(n)		Lrcg_prm.level = n;
//	�ő�m�C�Y���ݒ�
#define	R_linercg_set_noise(n)		Lrcg_prm.noise = n;

//	���F�������ݒ�i���P�ʁj
void R_linercg_set_dir1( int );
//	���F�������ݒ�i�������ƁA�������Ɓj
void R_linercg_set_dir2( double, double );
//	���F�������ݒ�i�Q�_�w��j
void R_linercg_set_dir3( int, int, int, int );

//	���F��
int R_linercg( LINERCG * );
//�@�C�ӊp�x�����e�[�u���쐬
int R_linerun( LINERCG * );
//�@�Œ�p�x�����e�[�u���쐬
int R_linerun_sp( LINERCG * );
//�@�������̎擾
int R_get_runinfo( LINERCG *, int );
//	�~��̔��^���𐔂���
void R_get_color_circle( int, int, int, int, int *, int * );
//	������̔��^���𐔂���
void R_get_color_line( int, int [], int *, int * );
//	�w���f�i�����j�̔��^���𐔂���
void R_get_color_point( int, int [], int *, int *t );
//	�w��G���A�̔��𐔂���
int R_count_bin_area( int page, int x1, int y1, int x2, int y2 );

#if	defined(R_BINREC)
int	pixel_table[ 256 ] = {
		0,	1,	1,	2,	1,	2,	2,	3,	1,	2,	2,	3,	2,	3,	3,	4,
		1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
		1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
		1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
		3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
		4,	5,	5,	6,	5,	6,	6,	7,	5,	6,	6,	7,	6,	7,	7,	8,
	};
#else
	extern int pixel_table[];	// 2009/07/28
#endif

// R_SFTBIN.asm
void	R_ShiftBinMemory(void* p, int dx, int dy);
#ifdef __GNUC__
	void	R_ShiftBinMemory_(void* adr, int d_x, int d_y);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_BINREC_H_
