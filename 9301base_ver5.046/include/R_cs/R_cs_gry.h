/******************************************************************************

	R_CS_GRY.H

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		2006.05.09.000		�I���W�i��
	1.07b1.5	2009.02.06			#ifndef __R_CS_GRY_H__�`�ǉ�

******************************************************************************/
#ifndef __R_CS_GRY_H__
#define __R_CS_GRY_H__

#ifdef __cplusplus
extern "C" {
#endif


// --- define ---
// �G�b�W�T�[�`�̕���(dir)
#define	ES_UP		0x01		//��
#define	ES_DOWN		0x02		//��
#define	ES_RIGHT	0x04		//��
#define	ES_LEFT		0x08		//��

// �~�̈���G�b�W�T�[�`����Ƃ��̕���
#define	ES_CIR_EXT	0x01		//�~�O���ɃT�[�`
#define	ES_CIR_INT	0x02		//�~�����ɃT�[�`

#define ES_GRAY		0x0010
#define ES_BIN		0x0020

#define ES_ROUND	0x0100		// �O���[�G�b�W�T�[�`�̊ۂߍ���
#define ES_DEBUG	0x8000		// �G�b�W�T�[�`�_�\��

typedef struct {
	int page;			// �T�[�`�y�[�W
	int level;			// �G�b�W�ŏ��K����
	int average;		// ���ω���
	int noise;			// �m�C�Y��
	int slice;			// �G�b�W���o���x��
} R_CS_GRAY_ES_PARA;

typedef struct {
	R_POINT		tCenter;		//�T�[�`�G���A�̒��S���W[pixel]
	int			nRadius;		//�T�[�`�G���A�̒��S����T�[�`�J�n�_�܂ł̋���[pixel]
	int			nLength;		//�T�[�`���鋗��[pixel]
	int			nDirection;		//�T�[�`����(ES_CIR_EXT or ES_CIR_INT)
	int			nDegreeStart;	//�T�[�`�J�n�p�x[deg]
	int			nDegreeEnd;		//�T�[�`�I���p�x[deg]
	int			nDegreeStep;	//�T�[�`�p�x�X�e�b�v[deg]
} R_CS_GRAY_ES_CIR_PARA;

typedef struct {
	R_POINT		tCenter;		//�T�[�`�G���A�̒��S���W[pixel]
	R_POINT		*pStart;		//�T�[�`�J�n�_�@���W��[pixel]
	int			nNumber;		//�T�[�`�J�n�_��
	int			nLength;		//�T�[�`���鋗��[pixel]
	int			nDirection;		//�T�[�`����(ES_CIR_EXT or ES_CIR_INT)
} R_CS_GRAY_ES_CURVE_PARA;

// --- variable ---

// --- prototype ---
void R_CS_GrayEdgeSearchSetParameter(R_CS_GRAY_ES_PARA para);
int R_CS_GrayEdgeSearchArea(R_POINT *edge, R_RECT area, int step, int dir, int mode);
int R_CS_GrayEdgeSearchCircle( R_POINT *pEdge, R_CS_GRAY_ES_CIR_PARA *pPara, int nMode );
int R_CS_GrayEdgeSearchCurve( R_POINT *pEdge, R_CS_GRAY_ES_CURVE_PARA *pPara, int nMode );

#ifdef __cplusplus
}
#endif
#endif
