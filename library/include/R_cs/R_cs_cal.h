/******************************************************************************

	R_CS_CAL.H

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		2006.05.09.000		�I���W�i��
	1.07b1.5	2009.02.02			#ifndef __R_CS_CAL_H__�`�ǉ�
				2009.02.06			�\����R_CS_CALIB_FOCUS_PARA�Ɉȉ��̃����o�ǉ�
										R_POINT	tDispPosition;		�\���ʒu
										int		nColor;				�\���F
******************************************************************************/
#ifndef __R_CS_CAL_H__
#define __R_CS_CAL_H__

// --- define ---
#define CALIB_DIR_INT			0x1			//�����փT�[�`
#define CALIB_DIR_EXT			0x2			//�O���փT�[�`
#define CALIB_DIR_HORIZONTAL	0x10		//���s�������o
#define CALIB_DIR_VERTICAL		0x20		//�����������o

#define CALIB_MODE_SINGLE			0x00000001		//1�{�̐������o
#define CALIB_MODE_DOUBLE			0x00000002		//2�{�̐������o

#include "R_cs_gry.h"

typedef struct{
	int					*pRateX;							//X��������\(um/pixel * 1000)�ւ̃|�C���^
	int					*pRateY;							//Y��������\(um/pixel * 1000)�ւ̃|�C���^
	int					nTargetRateMin;						//�ڕW�{���ŏ��l(um/pixel * 1000)
	int					nTargetRateMax;						//�ڕW�{���ő�l(um/pixel * 1000)
	int					nPitchUm;							//���肷��s�b�`�̒l[um]
	int					nXw;								//����G���A���@[pixel]
	int					nYw;								//����G���A����[pixel]
	int					nDirection;							//�T�[�`�������(CALIB_DIR_INT or CALIB_DIR_EXT)
	int					nColor;								//�T�[�`����F(ES_BLACK or ES_WHITE)
	R_CS_GRAY_ES_PARA	tEsPara;							//�G�b�W�T�[�`�p�����[�^�\����
	R_POINT				*pPoint;							//���W��p�o�b�t�@
	int					nMode;								//���샂�[�h(CALIB_MODE_SINGLE or CALIB_MODE_DOUBLE)
} R_CS_CALIB_MAGNIFICATION_PARA;

typedef struct{
	void		(*pFunc)(R_POINT *pCenter, int *pDegree);	//�摜��荞��&�ʒu���ߊ֐��ւ̃|�C���^

	int			nLimitDegree;								//���[�N�X�����~�b�g[deg](10�{�l)
	int			nLimitGapX;									//���[�N�ʒu����X���~�b�g[pixel]
	int			nLimitGapY;									//���[�N�ʒu����Y���~�b�g[pixel]
	R_POINT		tTargetPosi;								//���[�N���S�@�ڕW�ʒu[pixel]

	R_POINT		tFocusAreaVector;							//�s���g����@����G���A���S[pixel](���[�N���S����̑��΍��W)
	int			nFocusAreaXw;								//�s���g����@����G���A���@[pixel]
	int			nFocusAreaYw;								//�s���g����@����G���A����[pixel]
	int			nFocusLimitLevel;							//�s���g����@�ڕW�G�b�W�Z�x(0�`255)
	int			nFocusEdgeDirection;						//�s���g����@�G�b�W����(CALIB_DIR_HORIZONTAL or CALIB_DIR_VERTICAL)
	int			nFocusPageExe;								//�s���g����@�ΏۃO���[�y�[�W�ԍ�
	int			nFocusPageTemp;								//�s���g����@�e���|�����p�O���[�y�[�W�ԍ�
	int			*pFocusBuffer;								//�s���g����@�f�[�^��p�o�b�t�@
} R_CS_CALIB_POSI_PARA;

typedef struct{
	R_RECT	tArea;										//����G���A
	int		nLimitLevel;								//�ڕW�G�b�W�Z�x(0�`255)
	int		nPageExe;									//����ΏۃO���[�y�[�W�ԍ�
	int		nPageTemp;									//�e���|�����p�O���[�y�[�W�ԍ�
	int		*pBuffer;									//�f�[�^��p�o�b�t�@
	void	(*Capture)(void);							//�摜��荞�݊֐��ւ̃|�C���^
	R_POINT	tDispPosition;								//<1.07b1.5> �\���ʒu
	int		nColor;										//<1.07b1.5> �\���F
} R_CS_CALIB_FOCUS_PARA;

// --- variable ---

// --- prototype ---
int R_CS_CalibrateMagnification(R_CS_CALIB_MAGNIFICATION_PARA *pPara);
void R_CS_CalibratePosition(R_CS_CALIB_POSI_PARA *pPara);
int R_CS_CalibrateFocus(R_CS_CALIB_FOCUS_PARA *pPara);

#endif
