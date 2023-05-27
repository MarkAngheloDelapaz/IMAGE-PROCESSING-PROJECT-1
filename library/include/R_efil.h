/******************************************************************************
	R_efil.h / R_efil.c
	
	�ύX����
	Ver 3.50	2008.03.10	���������[�X
				2008.07.29	R_efil_get_result_memsize�ǉ�
	Ver 3.50	2009.02.12	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_EFIL_H_
#define	R_EFIL_H_

#include "R_stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

//���ʍ\���̂̌^�錾(���[�U�[�Ő錾���ꂽ�ϐ���R_efil_open�Ɉ��n���ď�����)
typedef struct{
	int	nMemSize;				//�S�������T�C�Y
	int nMaxNumber;				//�ő�i�[���W�_��
	int nFoundNumber;			//���o���W�_��
	R_POINT *pPoint;			//���o���W
	int *pValue;				//���o�G�b�W���x
	int *pAngle;				//���o�G�b�W���z����
}R_EFIL_RESULT;
//�p�����[�^�\���̂̌^�錾(��{�I�ɂ͊O���ɂ͌���Ȃ�)
typedef struct{
	int nMemSize;				//�S�������T�C�Y
	int nGaussSigma;			//Gaussian�J�[�l���̃Вl
	int nGaussSize;				//Gaussian�J�[�l���̃T�C�Y
	R_RECT sRect;				//�����̈�					�i���[�U�[�ύX�\�j
	int nThresh;				//���o�_�������l			�i���[�U�[�ύX�\�j
	int *pMaskX;				//X�Δ����^Gaussian�J�[�l���̐擪�A�h���X
	int *pMaskY;				//Y�Δ����^Gaussian�J�[�l���̐擪�A�h���X
	int *pImageX;				//�����o�b�t�@�i�P�K�w�Δ����p�j
	int *pImageY;				//�����o�b�t�@�i�P�K�x�Δ����p�j
}R_EFIL_PARAMETER;
typedef struct{
	R_POINT sSrcPoint;			//�g�咆�S�ʒu
	R_RECT	sDstRect;			//�g��\���̈�
	int		nMag;				//�g��{��
	R_POINT *pDotPoint;			//�`��_�Q�̍��W�擪�A�h���X
	int		nDotNumber;			//�`��_�Q��
	int		nDotMag;			//�`��_���W�̔{��
	int		nColor;				//�`��_�̐F
	int		nMode;				//�`�惂�[�h
}R_EFIL_DISP_EXPANSION;
//�v���g�^�C�v�錾
int R_efil_parameter_open( int gauss_size, int gauss_sigma );				//�����p�����[�^�̏�����
int R_efil_parameter_close( int nParaHandle );								//�����p�����[�^�̏I��
int R_efil_result_open( int max_number );									//���ʊi�[��̏�����
int R_efil_result_close( int nResHandle );									//���ʊi�[��̏I��
int R_efil_set_area( int nParaHandle, R_RECT sRect );						//�����̈�̐ݒ�
int R_efil_set_thresh( int nParaHandle, int nThresh );						//�������l�̐ݒ�
int R_efil_canny( int nParaHandle, int nResHandle, unsigned char *pImage );	//Canny�t�B���^�̎��s
int R_efil_canny_with_nosense( int nParaHandle, int nResHandle, unsigned char *pImage, R_RECT *pNoSenseArea );	//2009.08.10 by nakahara added
int R_efil_get_result_number( int nResHandle );								//���o�G�b�W���̎擾
R_POINT *R_efil_get_result_point( int nResHandle );							//���o�G�b�W���W�̃|�C���^�擾
int R_efil_point_compress( R_POINT *pPoint, int nNumber, int nComp  );		//�G�b�W���W�̓������k
int R_efil_get_para_memsize( int nParaHandle );								//�����p�����[�^�̃������T�C�Y�̎擾
int R_efil_get_para_gsize( int nParaHandle );								//�K�E�V�A���}�X�N�̃T�C�Y�擾
int R_efil_get_para_gsigma( int nParaHandle );								//�K�E�V�A���}�X�N�̃Вl�擾
R_RECT *R_efil_get_para_rect( int nParaHandle );							//�����̈�̃|�C���^�擾
int R_efil_get_result_memsize( int nResHandle );							//�������ʂ̃������T�C�Y�̎擾			2008.07.29�ǉ�
int   R_efil_find_edge_on_line( int nParaHandle, unsigned char *pImage, R_POINT *p0, R_POINT *p1, R_POINT *rp, int mode );	//������̃G�b�W�G�b�W���o
void R_efil_draw_set_expimage_srcpoint( R_EFIL_DISP_EXPANSION *pExDisp, R_POINT pPoint );	//�摜�g��ʒu���W�̐ݒ�
void R_efil_draw_set_expimage_dstrect( R_EFIL_DISP_EXPANSION *pExDisp, R_RECT pRect );		//�摜�\����̗̈�ݒ�
void R_efil_draw_set_expimage_mag( R_EFIL_DISP_EXPANSION *pExDisp, int nMag );				//�摜�g�嗦�̐ݒ�
void R_efil_draw_set_expimage_color( R_EFIL_DISP_EXPANSION *pExDisp, int nColor );			//�_�Q�\���F�̐ݒ�
void R_efil_draw_set_expimage_mode( R_EFIL_DISP_EXPANSION *pExDisp, int nMode );			//�g��\�����[�h�̐ݒ�
void R_efil_draw_set_expimage_dot( R_EFIL_DISP_EXPANSION *pExDisp, R_POINT *pPnt, int nPntNumber, int nPntMag );	//�_�Q�\���p�f�[�^�̐ݒ�
int R_efil_draw_disp_expimage( R_EFIL_DISP_EXPANSION *pExDisp, int nSrcNo );				//�g��\�����s
int R_efil_draw_disp_expimage_with_cursor( R_EFIL_DISP_EXPANSION *pExDisp, R_RECT sRect, int nSrcNo);	//�g��\�����s(�J�[�\������t)

int		R_efil_do_c( unsigned char *pImage, int *pImageX, int *pImageY, int nGaussSize, int nGaussSigma, int xsize,int ysize ,short *maskX,short *maskY,int div);
int*	R_efil_get_result_angle( int nResHandle );
int*	R_efil_get_result_value( int nResHandle );

int	R_efil_dogfilter(int nParaHandle, unsigned char* pImage);	// 2009/07/30
int	R_line_correction(int v1, int v2, int v3, int v4, int p, int q, int max);	// 2009/07/30

// ���C�u���������֐� R_ef_do.asm
int	R_efil_do(   unsigned char *pImage, int *pImageX, int *pImageY, int nGaussSize, int nGaussSigma, int xsize,int ysize ,short *maskX,short *maskY,int div);
int	R_efil_do11( unsigned char *pImage, int *pImageX, int *pImageY, int nGaussSize, int nGaussSigma, int xsize,int ysize ,short *maskX,short *maskY,int div);
#ifdef __GNUC__
	int	R_efil_do_(unsigned char* pImage, int* pImageX, int* pImageY, int nGaussSize, int nGaussSigma, int xsize, int ysize, short* maskX, short* maskY, int div);
	int	R_efil_do11_(unsigned char* pImage, int* pImageX, int* pImageY, int nGaussSize, int nGaussSigma, int xsize, int ysize, short* maskX, short* maskY, int div);
#endif

#ifdef __cplusplus
}
#endif
	
#endif	// R_EFIL_H_
