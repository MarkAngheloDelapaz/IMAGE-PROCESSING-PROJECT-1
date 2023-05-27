/******************************************************************************
	R_nHough.H / R_nHough.C
	
	�ύX����
	Ver 3.39	2003/06/10	R_nhough_clear_tbl(),R_nhough_voting_point()�ǉ�  by matsu
	Ver 4.01	2009/08/07	R_nhough_hp****	�ǉ�	by nakahara
	Ver 4.01	2012/04/09	64bit�}���`�X���b�h�Ή�
******************************************************************************/

#ifndef R_NHOUGH_H_
#define R_NHOUGH_H_

#include <R_stdlib.h>

#ifdef __x86_64__
#include "R_plane.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __x86_64__
	typedef struct {
		unsigned long	ulNhough_max_r;
		unsigned long	ulNhough_org_x;
		unsigned long	ulNhough_org_y;
		unsigned long	ulNhough_xo;
		unsigned long	ulNhough_yo;
		unsigned long	ulNhough_del_rw;
		unsigned long	ulNhough_del_aw;
		unsigned short 	*pNhough_tbl;
	} R_NHOUGH_DATA;
#else
	extern unsigned short	*pNhough_tbl;
	extern unsigned long	ulNhough_max_r;
	extern unsigned long	ulNhough_org_x;
	extern unsigned long	ulNhough_org_y;
	extern unsigned long	ulNhough_xo;
	extern unsigned long	ulNhough_yo;
	extern unsigned long	ulNhough_del_rw;
	extern unsigned long	ulNhough_del_aw;
#endif

#define R_NHOUGH_DIMENSION_MAX	10		//2009.08.07 by nakahara added

//2009.08.07 by nakahara added
typedef struct{			//�ėp�n�t�ϊ��e�[�u���\����
	int				nDimension;							//����
	int				nSize[R_NHOUGH_DIMENSION_MAX];			//�e�����̓��[�T�C�Y
	int				nMemSize;							//�\���̑S�̂̃������T�C�Y
	int				nDepth;								//�e�[�u���̓��[��ԃT�C�Y(2�����̏ꍇ nSize[0]*nSize[1]
	unsigned long	*pTable;							//�e�[�u���̐擪�A�h���X
}R_NHOUGH_HPSTRUCT;

//2009.08.07 by nakahara added
typedef struct{			//�����n�t�ϊ��p�e�[�u���\����
	int				nResoAngle;								//�p�x����
	int				nResoPixel;								//��f����
	int				nOrgAngle;								//�p�x���e�[�u�����_�̊p�x
	int				nOrgPixel;								//��f���e�[�u�����_�̋���
#ifdef __x86_64__
	int				nWidth;
	int				nHeight;
#endif
	R_NHOUGH_HPSTRUCT	*pVoteArea;
}R_NHOUGH_HPLINE;

//2009.08.07 by nakahara added
typedef struct{			//�����`�n�t�ϊ��p�e�[�u��
	R_NHOUGH_HPLINE	tLineHough[4];
}R_NHOUGH_HPRECT;

typedef struct{
		int start;
		int end;
}R_NHOUGH_HPPOLY_RANGE;


#define R_NHOUGH_HPPOLY_LINE_MAXNUM	16

typedef struct{
		int nLineNum;									//	�}�`�Ɏg�p����Ă��钼����
		int nBaseAngleMin;								//	�}�`�̋��e�p�x(�ŏ�)
		int nBaseAngleMax;								//	�}�`�̋��e�p�x(�ő�)
		int nDist[R_NHOUGH_HPPOLY_LINE_MAXNUM];			//	�e�����̌��_����̋���
		int nAngle[R_NHOUGH_HPPOLY_LINE_MAXNUM];		//	�e�����̖@���̊p�x(�E��萳)
}R_NHOUGH_HPPOLY_SHAPE;


#define R_NHOUGH_HPPOLY_RANGE_MAXNUM	64
typedef struct{
		R_NHOUGH_HPPOLY_RANGE tRange[R_NHOUGH_HPPOLY_RANGE_MAXNUM];
		int nRangeNum;
		R_NHOUGH_HPLINE tHpStruct;
}R_NHOUGH_HPPOLY;

#ifdef __x86_64__
R_NHOUGH_DATA*	R_nhough_open(int widht, int height);
int		R_nhough_voting(R_NHOUGH_DATA*, R_PLANE* plane, R_RECT* rect, int th);
int		R_nhough_disp_tbl(R_NHOUGH_DATA*, R_PLANE* plane);
void	R_nhough_clear_tbl(R_NHOUGH_DATA*);
void	R_nhough_voting_point(R_NHOUGH_DATA*, R_POINT* point, int num);
int		R_nhough_detection(R_NHOUGH_DATA*, int* a, int* b, int* c, int line_num);
int		R_nhough_set_mask(R_NHOUGH_DATA*, int width, int angle);
int		R_nhough_get_mask(R_NHOUGH_DATA*, int* width, int* angle);
#else
int R_nhough_open( void );
int R_nhough_voting( int xs,int ys,int xw,int yw,unsigned char *sp, int th);
int R_nhough_disp_tbl( void );
void R_nhough_clear_tbl();
void R_nhough_voting_point(R_POINT *point, int num);
int R_nhough_detection( int *a, int *b , int *c, int line_num );
int R_nhough_set_mask( int width, int angle );
int R_nhough_get_mask(int *width, int *angle);
#endif
int R_nhough_disp_line( int a[],int b[],int c[], int line_num ,int mode,int xs,int ys,int xw,int yw);

//���M�������n�t�ϊ��֐��Q //2009.08.07 by nakahara added
#ifdef __x86_64__
int	R_nhough_hpline_create(R_NHOUGH_HPLINE* pLineHough, int nResoAngle, int nResoPixel, int width, int height);
int	R_nhough_hprect_create(R_NHOUGH_HPRECT* pRectHough, int widht, int height);
#else
int R_nhough_hpline_create( R_NHOUGH_HPLINE *pLineHough, int nResoAngle, int nResoPixel );
int R_nhough_hprect_create( R_NHOUGH_HPRECT *pRectHough );
#endif
int R_nhough_hpline_destroy( R_NHOUGH_HPLINE *pLineHough );
int R_nhough_hpline_table_all_clear( R_NHOUGH_HPLINE *pLineHough );
int R_nhough_hpline_table_angle_clear( R_NHOUGH_HPLINE *pLineHough, int nMinAngle, int nMaxAngle );
int R_nhough_hpline_vote_with_mode( int nSrcNumber, const R_POINT *pSrcPoint, R_NHOUGH_HPLINE *pLineHough, int nAngleMin, int nAngleMax , int nMode);
int R_nhough_hpline_search( R_NHOUGH_HPLINE *pLineHough, int nNumber, int nAngleMask, int nPixelMask, int nMinAngle, int nMaxAngle, R_LINE *pLineCoeff, int *pLineAngle, unsigned long *pLineVoteValue);
int R_nhough_hprect_search( int nSrcNumber, const R_POINT* pSrcPoint, R_NHOUGH_HPRECT *pRectHough, int nAngleMin, int nAngleMax, int nLengthMin_a, int nLengthMax_a, int nLengthMin_b, int nLengthMax_b, R_ARBRECT *pResult );

// �����֐�(�����C�u�����֐��Ŏg���\������)
#ifdef __x86_64__
R_NHOUGH_HPSTRUCT* r_nhough_hptrans_table_create(int nDimension, int* pSize);
#else
int		r_nhough_hptrans_table_create( int nDimension, int *pSize );
#endif
void	r_nhough_hptrans_table_destroy( R_NHOUGH_HPSTRUCT *pHough );
int		r_nhough_hpline_table_clear( unsigned long *pTable, int xs, int ys, int xe, int ye, int xsize );
int		r_nhough_hpline_get_maxvote( unsigned long *pTable, int xs, int ys, int xsize, int ysize, int nMaxXSize, int *pResX, int *pResY );
int		r_nhough_hpcalc_line_equation( R_LINE *pLineCoeff, int xs, int ys, int nOrgAngle, int nResoAngle, int nOrgPixel, int nResoPixel );
int		r_nhough_hpline_projection_angle( int nFilterSize, R_NHOUGH_HPLINE *pLineHough, int xs, int ys, int xe,  int ye, unsigned long *bright );
int		r_nhough_hprect_get_maxangle( R_NHOUGH_HPLINE *pLineHough, unsigned long *pProjection, int nAngleMin, int nAngleMax, int *pResultAngle );
int		r_nhough_hprect_get_maxlength( unsigned long *pTable, int xs, int ys, int xe, int ye, int xsize, int lmin, int lmax, R_POINT *pResPos, int *pResLength );
int		r_nhough_hptrans_table_all_clear( R_NHOUGH_HPSTRUCT *pHough );
int		R_nhough_hpline_table_disp(R_NHOUGH_HPLINE *pLineHough, int rate);


//----------------------------------------------------------------------------------------------------
//	���p�`�n�t���o���C�u�����֐��Q
//	R_nhough_hppoly_***
//----------------------------------------------------------------------------------------------------

///
/// \brief R_nhough_hppoly_create
/// \param pHough		���p�`�n�t�ϊ��\����
/// \param nResoAngle	�p�x��������\	hpline�\���̂ɏ�����
/// \param nResoPixel	������������\	hpline�\���̂ɏ�����
/// \return
///
///	�n�t���[��Ԃ��쐬����
///	�������o�Ɠ�����2�����̋�Ԃ𗘗p����
int		R_nhough_hppoly_create(R_NHOUGH_HPPOLY *pHough, int nResoAngle, int nResoPixel );


///
/// \brief R_nhough_hppoly_setrange
/// \param pHough		���p�`�n�t�ϊ��\����
/// \param nAngleMin	�ŏ��p�x�z��
/// \param nAngleMax	�ő�p�x�z��
/// \param num			�z��
/// \return
///
///	�n�t�ϊ��ɂē��[�Ώۂ̊p�x���w�肷��
///	�����Ŋp�x�͈͂̏d���`�F�b�N���s�����ߏd�������G���A���w���
int		R_nhough_hppoly_setrange(R_NHOUGH_HPPOLY *pHough, int *nAngleMin, int *nAngleMax, int num);

///
/// \brief R_nhough_hppoly_setshape
/// \param pHough		���p�`�n�t�ϊ��\����
/// \param pShape		���p�`�`��f�[�^�\����
/// \return
///
///	�n�t�ϊ��ɂē��[�Ώۂ̊p�x�𑽊p�`�`��f�[�^����w�肷��
/// �����ŏd���`�F�b�N�����{���邽�ߕ����`��̓o�^����
///	R_nhough_hppoly_search�ɂČ�������\��������`��͗\�ߓ��[�O�Ɍ`��o�^�����{���邱��
int		R_nhough_hppoly_setshape(R_NHOUGH_HPPOLY *pHough, R_NHOUGH_HPPOLY_SHAPE *pShape);

///
/// \brief R_nhough_hppoly_vote
/// \param pHough		���p�`�n�t�ϊ��\����
/// \param pPoint		���[����_�Q
/// \param num			�_�Q�̑���
/// \return
///
///	�n�t��ԏ�ɓ_�Q�̓��[���s��
int		R_nhough_hppoly_vote(R_NHOUGH_HPPOLY *pHough, R_POINT *pPoint, int num);

///
/// \brief R_nhough_hppoly_search
/// \param pHough		���p�`�n�t�ϊ��\����
/// \param pShape		���o���鑽�p�`�`��f�[�^
/// \param tArea		�`�󌟏o���s���G���A(�}�`�̌��_���܂܂��͈�)
/// \param tPos			���o�ʒu(result)
/// \param nAngle		���o�p�x(result)
/// \return
///
///	�����̏d�ˍ��킹�}�b�`���O���s�����o�����ʒu���ʂ�Ԃ�
///	���ӂ��ׂ��_�́A�{���C�u�����[�͒���Hough��Ԃ��g�p�����h���������o�h�ł��邽�߁A��ʓI�}�`�ɓK�p����h��ʉ�Hough�ϊ��h�Ƃ͈ȉ��̓_�ňقȂ�Ƃ������Ƃł���B
///		1.	���o����̂͒����݂̂ŋȐ��͈���Ȃ�
///		2.	���o����̂́h�����h�ł���h�����h�ł͂Ȃ�����
int		R_nhough_hppoly_search(R_NHOUGH_HPPOLY *pHough, R_NHOUGH_HPPOLY_SHAPE *pShape, R_RECT tArea, R_POINT *tPos, int *nAngle);

///
/// \brief R_nhough_hppoly_getline
/// \param pShape		���p�`�`��f�[�^�\����
/// \param tPos			�摜��ԏ�ł̑��p�`�̌��_�ʒu
/// \param nAngle		�摜��ԏ�ł̑��p�`�̊p�x
/// \param result		���p�`�e�ӂ�R_LINE�\����
///
/// \return	int			���o���ꂽ�ő哊�[��
///
///	���p�`�`��f�[�^�y�шʒu�A�p�x���瑽�p�`�e�ӂ�R_LINE�\���̂ɕϊ�����
int		R_nhough_hppoly_getline(R_NHOUGH_HPPOLY_SHAPE *pShape, R_POINT tPos, int nAngle, R_LINE *result);

///
/// \brief R_nhough_hppoly_disp
/// \param pShape		���p�`�`��f�[�^�\����
/// \param tArea		�\���͈�
/// \param x			���p�`���_��x���W
/// \param y			���p�`���_��y���W
/// \param nAngle		���p�`�̊p�x
///
///	���p�`��C�ӂ̈ʒu�ɕ`�悷��
void	R_nhough_hppoly_disp(R_NHOUGH_HPPOLY_SHAPE *pShape, R_RECT tArea, int x, int y, int nAngle);

//!
//! \brief R_nhough_hppoly_clearrange
//! \return
//!
//! �o�^���p�`���N���A����B
//! setrange�Asetshape�͕����o�^�\�Ȃ��ߓo�^�������ꍇ�̓N���A�����s���邱��
int		R_nhough_hppoly_clear_range(R_NHOUGH_HPPOLY *pHough);

//!
//! \brief R_nhough_hppoly_clear_tbl
//! \param pHough
//! \return
//!
//!	���[�e�[�u�����N���A����
int		R_nhough_hppoly_clear_tbl(R_NHOUGH_HPPOLY *pHough);

#ifdef __cplusplus
}
#endif

#endif	// R_NHOUGH_H_
