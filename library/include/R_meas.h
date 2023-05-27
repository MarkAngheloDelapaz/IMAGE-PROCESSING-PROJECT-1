/******************************************************************************

	�s�r�b�X�R�O�P�i�摜�������u�j

		�Q�l�摜�������C�u�����p�w�b�_

	[history]
	version		date		comment
	Ver 1.02	1993.11. 1	�I���W�i��
	Ver 1.04	1994. 1.24	���x�����O�A���S���Y���C��
	Ver 1.04	1994. 1.25	�o�O�C���i�F�e�[�u���쐬�~�X�j
	Ver 1.04	1994. 2. 2	R_measure�������ύX�i�����ʌv�Z�̎w��ǉ��j
	Ver 1.04	1994. 2. 2	�ʐϏ��Ƀ\�[�g����֐��쐬
	Ver 1.05	1994. 3. 7	 �v�����ʊi�[�̈�̃T�C�Y���ςƂ���
				1994. 3.10	���ʐσu���[�u�����֐���ǉ�
				1994. 4.29	struct MEAS_DATA �� page �ǉ��i���͈ȑO����ύX����Ă����I�I�j
				1994. 4. 8	�u���[�u�����֐��C��
				1994. 5.23	�~��̌v���G���A���w��\�ɂ���
				1994. 6. 9	�A���S���Y���̐�����ǉ�
				1994. 6.15	
				1996. 7. 1	�w��ʐψȉ��u���[�u�폜�֐��F�w��ǉ�
				1996. 7.11	�������w��ȉ������Ή��ǉ�
				1996. 7.19	�w��ʐψȉ������Ή��ǉ�
				1999.11.12	640x480�Ή��̂��߁A���Q�摍�a�̃e�[�u���A���͒��v�Z�e�[�u����
							R_measure_open()�ŏ���������悤�ɂ���
				1999.09.10	640x480�Ή��iFX_SIZE,FY_SIZE->FxSize,FySize�̕ύX�AMxSize�̓����j by matsu
				1999.11.12	640x480�Ή��̂��߁A���Q�摍�a�̃e�[�u���A���͒��v�Z�e�[�u����
							R_measure_open()�ŏ���������悤�ɂ���  by matsu
				1999.12.08	�v�Z�e�[�u���̊J����R_measure_close()����ĂԂ悤�ɂ���
				2000.07.17	cal_character3�֐�����R_read_clk_count���R�����g�A�E�g
				2000.12.15	R_measure_close_table()��free��|�C���^��NULL��ݒ肷��
				2001.06.04	R_measure(),R_measure_area(),R_measure_fast()�ŃG���A�`�F�b�N���s���悤�ɂ���
	Ver 3.32b	2002.10.25	�G���A�O�̏ꍇ�̓G���[�I��
	Ver 3.48	2006.03.28	labeling_1st �i�Ō�́j���������̏����̊ԈႢ���C��
	Ver 3.48	2006.06.05	labeling_1st �Ŏ��͒������l�̐ݒ�̊ԈႢ���C��
	Ver 3.49	2006.09.05	����f�Ή�
	Ver.3.50	2007.06.26	R_measure���s���̌v���͈̓`�F�b�N�ŃT�C�Y���������ꍇ�̃`�F�b�N�������Ă����̂Œǉ�
				2008.01.24	R_measure_open()���s����FxSize,FySize�ŏ���������̂ł͂Ȃ���摜�T�C�Y�ő�l�ŏ���������悤�ɏC��
				2008.09.04	�I�[�o�t���[�΍�E64bit���Z��
				2008.09.29	R_measure_disp_blob��ǉ��@����
	Ver 3.50	2009.01.30	DOS & LINUX �\�[�X����
	Ver 4.01	2010.01.13	mpx->mode�`�F�b�N�p�~
				2010.05.22	��������90���̂Ƃ�sin��cos�̒l���������Ȃ�悤�ɂ����B_cal_character1�֐��C��
				2010.06.07	wsize��0�ɂȂ�Ȃ��悤�ɂ����B_cal_character3�֐��C���i�؎̂Ă��l�̌ܓ��ɕύX�j
				2010.10.12	DLONG�\���̂̐錾��R_stdlib.h�ֈړ�
				2011.01.05	R_meas_calc_contour()�ŁA�֊s�̓_����malloc�Ŋm�ۂ����_���𒴂��Ȃ����`�F�b�N���镔�����C��
				2012.07.20	R_get_run�֐��d�l�ύX�A�ő僉�����ǉ�
				2012.07.23	�������Ǘ�(runsize��rp)��unsigned short����int�ɕύX
				2012.08.06	R_meas_calc_contour�C���B�Ώۃu���u���P��f�̏ꍇ�A�t���[�Y����̂��C���B
				2016.05.27	R_meas_get_run_number�֐��ǉ�
				2016.06.21	�w�i�F�͂W�A����������悤�ɕύX�B�i�e�Ȃ��u���u�΍�j
				2016.06.22	�����ߗ̈�̂w���a�Ƃx���a��ǉ�
				2016.06.24	R_sort_area2�֐��ǉ�
******************************************************************************/

#ifndef	R_MEAS_H_
#define	R_MEAS_H_

#ifdef __x86_64__
#include <stdint.h>
#endif

#include "R_stdlib.h"	// 2010/10/12 by araki
#include "R_plane.h"			// ����f�Ή�  2006.09.05  by matsu

#ifdef __cplusplus	// 2010/10/12 by araki
extern "C" {
#endif

#define	PAI			3.141592654

//#define	MAXBLOB		2000		// �����O�̍ő�u���[�u��
//#define	MAXPOINT	480			// �����W�ő�l
#define	_MAX_POINT		480			//	�݊����ێ��̂���

#define	CONV_MAX_DEFAULT		10
#define	OYAKO_MAX_DEFAULT		4000
#define	MAXBLOB_DEFAULT			1000
#define	MEASURE_WIDTH_DEFAULT	1024
#define	MEASURE_HEIGHT_DEFAULT	1024

#define	GROUP1		0			//	�����ʌv�Z�O���[�v
#define	GROUP2		1
#define	GROUP3		2
#define	GROUP4		3

#define	SMALL_BLACK		0
#define	SMALL_WHITE		1

#define	RM_BLACK		1
#define	RM_WHITE		2

#define RM_NORMAL	0
#define RM_RUN		1
#define RM_AREA		2

//�@�U�S�r�b�g���l�p�\���� -> R_stdlib.h�Ɉړ�	2010/10/12
//#ifndef	DLONG_DEF
//#define	DLONG_DEF
//typedef struct {
//	unsigned long	lo;
//	unsigned long	hi;
//} DLONG ;
//#endif

/*
//	�C���[�W�\����
#define	IMAGE_TYPE_BINARY	0
#define	IMAGE_TYPE_GRAY8	1

typedef struct {
	int type;
	int width;
	int height;
	int bytes_per_line;
	void *pData;
} rsImage;
*/

//�@���Ԍv���p
#ifdef	R_MEAS
	unsigned long		meas_time;
	unsigned long		measure_time;
	unsigned long		calc_time;
	unsigned long		calc2_time1;
	unsigned long		calc2_time2;
	unsigned long		calc2_time3;
	unsigned long		cbtime[ 500 ];
#else
	extern	unsigned long		measure_time;
	extern	unsigned long		conb_time;
	extern	unsigned long		calc_time;
	extern	unsigned long		calc2_time1;
	extern	unsigned long		calc2_time2;
	extern	unsigned long		calc2_time3;
#endif


#define	MEAS_TM_NUM					32
//---------------------------------------------------------
#define	MEAS_TM_MEASURE_MAKE_AREA_TABLE		0
#define	MEAS_TM_MEASURE_GET_RUN				1
#define	MEAS_TM_MEASURE_MODIFY_RUN			2
#define	MEAS_TM_MEASURE_LABELING_FAST		3
#define	MEAS_TM_MEASURE_LABELING_1ST		4
#define	MEAS_TM_MEASURE_LABELING_2ND		5
#define	MEAS_TM_MEASURE_GET_BLOB_SIZE		6
#define	MEAS_TM_MEASURE_CALC1				7
#define	MEAS_TM_MEASURE_CALC2				8
#define	MEAS_TM_MEASURE_CALC3				9

#define	MEAS_TM_MEASURE_TOTAL				10
//---------------------------------------------------------

#define	MEAS_ID			0x03112121
#define	MEAS_STANDARD	1000

#ifdef __x86_64__
typedef struct {
	int i_oyako_n;
	
	void*	top;
	int*		pi_conv_n;				//	�����e�[�u���p�G���A�P
	int*		pi_conv_table;			//	�����e�[�u���p�G���A�Q
	int**		ppi_conv_table;			//	�����e�[�u���p�G���A�R
	uint32_t*	pul_sum_n2;		//	���Q�摍�a�̃e�[�u��
	uint32_t*	pul_perim;		//	���͒��v�Z�e�[�u��������
	int*		pi_oyako;				//	�e�q�e�[�u��������
	uint32_t*	pul_perim_work1;	//	���Ӓ���ƃG���A�P
	uint32_t*	pul_perim_work2;	//	���Ӓ���ƃG���A�Q
	int*		pi_perim_work3;			//	���Ӓ���ƃG���A�R
	int*		pi_child_table;			//	�q���e�[�u���p�G���A
	int*		pi_sort_work;				//	�\�[�g�p��ƃG���A
	unsigned short*	pus_area_work;	//	���������p
} MEASWORK;
#endif

#define	MEAS_MODE_COMPATIBLE	0			//	�I���W�i���݊����[�h
#define	MEAS_MODE_EXT			1			//	�g�����[�h
/*
enum {
	E_BMEAS_CANNOT_OPEN1 = 10000,
	E_BMEAS_CANNOT_OPEN2,
	E_BMEAS_NOT_OPENED,
	E_BMEAS_CANNOT_CREATE_AREA,
	E_BMEAS_ILLEGAL_IMAGE_TYPE,
	E_BMEAS_ILLEGAL_AREA,
	E_BMEAS_RUN_OVERFLOW,
	E_BMEAS_CONVERT_ERROR,
};
*/
typedef struct {
	int				id;

	int				mode;					// �������[�h
	int 			max_width;				// �����\�ȍő啝
	int 			max_height;				// �����\�ȍő卂
	int 			conv_max;				// �����֌W�ő�l
	int				oyako_max;				// �e�q�֌W�ő�l
	int				group;					// �v�Z�O���[�v

	int				modify;					// �����e�[�u���ҏW�t���O
	int				areasize;				// �w��ʐψȉ��������̃T�C�Y
	int				modicolor;				// �w��ʐψȉ��������̐F�w��
	int				errflag;				// �G���[���ɂO�ȊO���i�[�����

	R_PLANE			*pPlane;				// �����Ώۉ摜   2006.09.05  by matsu
	int				x;						// �v���G���A���゘���W
	int				y;						// �@�@�V�@�@�@�@�����W
	int				xl;						// �v���G���A����
	int				yl;						// �@�@�V�@�@����
	int				page;
	int				maxrun;
	int				maxblob;
	
	int				background_color;		// �w�i�F�A0:�� / 1:���B�w�i�F�͂W�A����������B�f�t�H���g��0�B	2016.06.21 by araki
	
	int				tm[MEAS_TM_NUM];		// ���Ԍv��
	void			*top;					// �m�ۂ����擪�|�C���^

//�@�ꎟ������
//	unsigned short	bn;						// �u���[�u�ԍ��ŏI�l
//	unsigned short	runsize;				// �����̑���					�i�ǉ��j
//	unsigned short	*rp;					// �e����ԍ��̃����ԍ�
	int				bn;						// �u���[�u�ԍ��ŏI�l	2012.07.20
	int				runsize;				// �����̑���			2012.07.20
	int*			rp;						// �e����ԍ��̃����ԍ�	2012.07.23
	unsigned short	*rn;					// �e����̃����̐�				�i�ǉ��j
	unsigned short	*yaxis;					// �e�����̂����W				�i�ǉ��j
	unsigned short	*rcol;					// �e�����̐F					�i�ǉ��j
	unsigned short	*runstrt;				// �����X�^�[�g�ʒu
	unsigned short	*runleng;				// ���������O�X
	unsigned short	*runnumb;				// �e�����̑�����u���[�u�ԍ�

	unsigned short	*parent;				// �e�̃u���[�u�ԍ�
	unsigned short	*child;					// �q�̃u���[�u�ԍ�
	unsigned short	*sibling;				// �Z��̃u���[�u�ԍ�
	unsigned short	*color;					// �F�i�O�F���A�P�F���j
	unsigned short	*perim;					// ���͒�
	unsigned short	*nholes;				// ���̐�
	unsigned short	*imin;					// �������ŏ��l
	unsigned short	*imax;					// �������ő�l
	unsigned short	*jmin;					// �������ŏ��l
	unsigned short	*jmax;					// �������ő�l

#ifdef __x86_64__
	uint32_t*		area;					// �ʐ�
	uint32_t*		sumi;					// ���������a
	uint32_t*		sumj;					// ���������a

	uint64_t*		sumi2;					// ���������a
	uint64_t*		sumij;					// �����Ϙa
	uint64_t*		sumj2;					// ���������a
#else
	unsigned long	*area;					// �ʐ�
	unsigned long	*sumi;					// ���������a
	unsigned long	*sumj;					// ���������a

	DLONG			*sumi2;					// ���������a
	DLONG			*sumij;					// �����Ϙa
	DLONG			*sumj2;					// ���������a
#endif

	unsigned short	*sortnumb;				// �ʐϏ��\�[�g����

//�@�񎟓�����
	unsigned short	*center_x;				// �d�S�����W
	unsigned short	*center_y;				// �d�S�����W

	double			*cos;					// �������i�������Ɓj
	double			*sin;					// �������i�������Ɓj

	unsigned short	*ellipse_a;				// ������
	unsigned short	*ellipse_b;				// �Z����

#ifdef __x86_64__
	uint32_t*		sholes;					// ���ʐ�
	uint32_t*		area2;					// �����܂߂��ʐ�
	uint32_t*		sumi_of_area2;			// �����܂߂����������a		// 2016.06.22
	uint32_t*		sumj_of_area2;			// �����܂߂����������a		// 2016.06.22
#else
	unsigned long	*sholes;				// ���ʐ�
	unsigned long	*area2;					// �����܂߂��ʐ�
	unsigned long*	sumi_of_area2;			// �����܂߂����������a		// 2016.06.22
	unsigned long*	sumj_of_area2;			// �����܂߂����������a		// 2016.06.22
#endif

	unsigned short	*hratio;				// ���ʐϔ�
	unsigned short	*dhmax;					// �������ő�l
	unsigned short	*dhmin;					// �������ŏ��l
	unsigned short	*dhave;					// ���������ϒl
	unsigned short	*nareas;				// �אڃG���A��
	unsigned short	*dsmax;					// �אڃG���A�����ő�l
	unsigned short	*dsmin;					// �אڃG���A�����ŏ��l
	unsigned short	*dsave;					// �אڃG���A�������ϒl
	unsigned short	*dpmax;					// �O�������ő�l
	unsigned short	*dpmin;					// �O�������ŏ��l
	unsigned short	*dpave;					// �O���������ϒl
	unsigned short	*lsize;					// �O�ڒ����`�厲������
	unsigned short	*wsize;					// �O�ڒ����`����������
	unsigned short	*lsize_1;				// �O�ڒ����`�`��p
	unsigned short	*lsize_2;				// �O�ڒ����`�`��p
	unsigned short	*wsize_1;				// �O�ڒ����`�`��p
	unsigned short	*wsize_2;				// �O�ڒ����`�`��p
	unsigned short	*xdiff;					// ���͈�
	unsigned short	*ydiff;					// ���͈�
	unsigned short	*peround;				// �ʐώ��͒���i�����܂܂Ȃ��j
	unsigned short	*pps;					// �ʐώ��͒���i�����܂ށj
#ifdef __x86_64__
	uint32_t*		xyarea;					// �O�ڒ����`�ʐρ��P
	uint32_t*		lwarea;					// �O�ڒ����`�ʐρ��Q
#else
	unsigned long	*xyarea;				// �O�ڒ����`�ʐρ��P
	unsigned long	*lwarea;				// �O�ڒ����`�ʐρ��Q
#endif
	unsigned short	*xyratio;				// �O�ڒ����`�c���䁔�P
	unsigned short	*wlratio;				// �O�ڒ����`�c���䁔�Q
	unsigned short	*sratioxy;				// �O�ڒ����`�ʐϔ䁔�P
	unsigned short	*sratiolw;				// �O�ڒ����`�ʐϔ䁔�Q
	unsigned short	*axisratio;				// �����ȉ~�厲�E������
	
#ifdef __x86_64__
	//	����̈�w��
	short*			ps_marea_y;
	short*			ps_marea_x;
	short*			ps_marea_xl;
	// ��Ɨ̈�
	MEASWORK*		work_area;
#endif
} MEASDATA;

typedef struct {
	unsigned short	xmin;
	unsigned short	xmax;
	unsigned short	y;
	unsigned short	yl;
	unsigned short	x[_MAX_POINT];
	unsigned short	xl[_MAX_POINT];
} MEASFORM;

typedef struct {
// �O���[�v�P������
	unsigned short	imin;					// �������ŏ��l
	unsigned short	imax;					// �������ő�l
	unsigned short	jmin;					// �������ŏ��l
	unsigned short	jmax;					// �������ő�l
	
	unsigned long	area;					// �ʐ�
	unsigned long	sumi;					// ���������a
	unsigned long	sumj;					// ���������a
	
	DLONG			sumi2;					// ���������a
	DLONG			sumij;					// �����Ϙa
	DLONG			sumj2;					// ���������a
	
	double			center_x;				// �d�S�����W
	double			center_y;				// �d�S�����W
	
// �O���[�v�Q������
	double			cos;					// �������i�������Ɓj
	double			sin;					// �������i�������Ɓj
	unsigned short	ellipse_a;				// ������
	unsigned short	ellipse_b;				// �Z����
	unsigned short	axisratio;				// �����ȉ~�厲�E������
	
} MEASCHAR;

// �����ʊi�[�̈�m��
int R_measure_open(MEASDATA *mpx, int maxblob);
int R_measure_open_ex(MEASDATA *mpx, int maxblob, int nSizeX, int nSizeY, int nConvMax, int nOyakoMax);

// �����ʊi�[�̈�J��
void R_measure_close( MEASDATA * );

// �����ʉ��Z�@���Z�����w��
int R_set_meas_configration( MEASDATA *mpx, int modsize, int areasize, int areacolor, int mode );

// �ꎟ�����ʂ����߂�
int R_measure( MEASDATA *, int, int, int, int, int, int );
// �ꎟ�����ʂ����߂�[����f]
int R_measure_ex(MEASDATA *mpx, R_PLANE *pPlane, int x, int y, int xl, int yl, int mode);
int R_measure_circle_ex(MEASDATA *mpx, R_PLANE *pPlane, int x, int y, int r, int mode);

// �ꎟ�����ʂ����߂�i�d�S�C�ʐς̂݁j
int R_measure_area( MEASDATA *, int, int, int, int, int );
// �ꎟ�����ʂ����߂�i�d�S�C�ʐς̂݁j[����f]
int R_measure_area_ex(MEASDATA *mpx, R_PLANE *pPlane, int x, int y, int xl, int yl);

// �ꎟ�����ʂ����߂�i�w��ʐψȉ������j
//int R_measure_with_area( MEASDATA *, int, int, int, int, int, int, int );

// �ʐϏ��u���[�u�ԍ��e�[�u���쐬
void R_sort_area( MEASDATA * );

// �S�ʐϏ��u���[�u�ԍ��e�[�u���쐬
void R_sort_area2(MEASDATA* mpx);

/*
	�֐����F	R_meas_get_run_number
	�@�@�\�F	�w���f�̑����郉���ԍ��擾
	�`�@���F	#include "R_meas.h"
				int R_meas_get_run_number(const MEASDATA* mpx, int x, int y);
	�������F	mpx		�����ʍ\���̃|�C���^
				x		x���W
				y		y���W
	�߂�l�F	�u���[�u�ԍ��i���̒l�̏ꍇ�́A�͈͊O�j
	���@���F	���W�ix, y�j�̉�f�������郉���ԍ���Ԃ��܂��B
	���ӎ����F	���ɂ���܂���B
*/
int	R_meas_get_run_number(const MEASDATA* mpx, int x, int y);

// �w���f�̑�����u���[�u�ԍ��擾
int R_get_blob_number(const MEASDATA* mpx, int x, int y);

// �w��ʐψȉ��̃u���[�u�폜
void R_del_small_blob( MEASDATA *, int );

// �w��ʐψȉ��̃u���[�u�폜
void R_del_small_blob_with_color( MEASDATA *, int, int );

// �v���G���A�w��i�~�j�֐�
int R_set_meas_circle( MEASFORM *, int, int, int );

int R_measure_form( MEASDATA *, int, MEASFORM *, int );

int R_measure_fast( MEASDATA *mpx, int mem_no, int x, int y, int xl, int yl, int color, int noise );
int R_measure_fast_ex(MEASDATA *mpx, R_PLANE *pPlane, int x, int y, int xl, int yl, int color, int noise);

int R_measure_test( MEASDATA *mpx, int mem_no, int x, int y, int xl, int yl );

// �����ʌv�Z�e�[�u���J��
void R_measure_close_table( void );

//�u���[�u�֊s���o
int R_meas_calc_contour(const MEASDATA* mpx, int nBlobNumber, int hMultiPointHandle);

//�u���[�u���w���BIN�v���[���ɕ\������
int R_measure_disp_blob(const MEASDATA* mpx, int page, int blobNo);

// �����u���u�̓����ʂ̎Z�o�֐�
int	R_measure_integrated_blob(const MEASDATA* mpx, const int* blob_no, int num_of_blobs, int group, MEASCHAR* result);

// �u���u�S�̂̓����ʂ̎Z�o�֐�
int	R_measure_whole_blob(const MEASDATA* mpx, int blob_no, int group, MEASCHAR* result);

// �u���u�̃T�u�s�N�Z���d�S���W
R_DPOINT	R_measure_center_of_blob(const MEASDATA* mpx, int num);

// �����u���u���܂߂��S�ʐς̃T�u�s�N�Z���d�S���W
R_DPOINT	R_measure_center_of_whole_blob(const MEASDATA* mpx, int num);

//�@�A�Z���u���֐�
//int		R_get_run( long *, int, int, int, unsigned short *, unsigned short *, unsigned short * );
int		R_get_run(long*, int, int, int, unsigned short*, unsigned short*, unsigned short*, int);
int		R_get_run_ext( long *, int, int, int, unsigned short *, unsigned short *, unsigned short * );
long	R_div6432a( long, long, long, DLONG * );
void	R_mul3232( long, long, DLONG * );
void	dword_shr( long, long, long, DLONG * );
void	dword_div6( long, long, DLONG * );
#ifdef __GNUC__
	int		R_get_run_(long*, int, int, int, unsigned short*, unsigned short*, unsigned short*, int);
	long	R_div6432a_(long, long, long, DLONG*);
	void	R_mul3232_(long, long, DLONG*);
	void	dword_shr_(long, long, long, DLONG*);
#endif

#ifdef __cplusplus	// 2010/10/12 by araki
}
#endif

#endif	// R_MEAS_H_
