// �b��\�[�X�ł��B�d�l�ύX����ꍇ������܂��B
// �\�[�X��ύX���Ȃ��ł��������B�Ȃɂ�����΍r�؂܂ŁB

/******************************************************************************
	�t�H���g���C�u�����[
	
	RFont.h / RFont.c
	�ύX����
	Ver 3.49	2006/11/16	�V�K�쐬�iRGraph���番���Ɨ��j
	Ver 3.50	2009/02/16	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_FONT_H_
#define	R_FONT_H_

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	FONT_ID_LENGTH		6
#define	FONT_NAME_LENGTH	8

#define	FONT_TYPE_HALF		0
#define	FONT_TYPE_FULL		1
#define	FONT_TYPE_SYSTEM	2

//---------------------------------------------------------
//	�t�H���g�e�[�u���\����
//---------------------------------------------------------
typedef struct {
	unsigned short nStart;
	unsigned short nEnd;
	unsigned char *pAddress;
} RFontTable;

//---------------------------------------------------------
//	�t�H���g�\����
//---------------------------------------------------------
typedef struct {									//̫�ď��\����
	int			nValid;								//0: ����, 1:�L��
	char		cIdentifer[FONT_ID_LENGTH+1];		//FONT���� 6char "FONTX2"�������Ă���
	char		cName[FONT_NAME_LENGTH+1];			//FONT��   8char "POP"(etc) �������Ă���
	int			nType;								//0: ���p, 1:�S�p
	int			nXsize;								//FONT�T�C�YX
	int			nYsize;								//FONT�T�C�YY
	UCHAR		*pFontDataBase;						//FONT�ް��ް���ر��ڽ
	UCHAR		*pFontData;							//FONT�ް���ر��ڽ(16byte aligned)
	int			nTableNum;							//̫��ð��ِ�	����̫��=1, ���p=1, �S�p=̫�Ĉˑ�
	RFontTable	*pFontTable;						//̫��ð���
} RBaseFont;

typedef struct {
	RBaseFont hfont;
	RBaseFont zfont;
	RBaseFont hx2font;
	RBaseFont zx2font;
} RFont;

//---------------------------------------------------------
//	���������w��
//---------------------------------------------------------
#define DISP_NORMAL			0			// R_FFT
#define DISP_H2				0x0001
#define DISP_V2				0x0010
#define DISP_4				0x0011
#define DISP_H4				0x0012
#define DISP_V4				0x0021
#define DISP_16				0x0022

//---------------------------------------------------------
//	�e�L�X�g�`��C���[�W�\����
//---------------------------------------------------------
#define	TEXT_IMAGE_MAX_WIDTH		1024		//	�ő剡�s�N�Z����
#define	TEXT_IMAGE_SHIFT_MAX		32			//	�ő�V�t�g��+1
#define	TEXT_IMAGE_FONT_HEIGHT_MAX	32			//	�t�H���g�ő卂��
#define	TEXT_IMAGE_WIDTH_ALIGN		32			//	�������̔{���ɂ���
#define	TEXT_IMAGE_SIZE		(((TEXT_IMAGE_MAX_WIDTH+TEXT_IMAGE_SHIFT_MAX)+TEXT_IMAGE_WIDTH_ALIGN-1)/TEXT_IMAGE_WIDTH_ALIGN*TEXT_IMAGE_FONT_HEIGHT_MAX*4)*TEXT_IMAGE_WIDTH_ALIGN

typedef struct {
	int nImageXsize;
	int nImageYsize;
	int nImageXbytes;
	int nMemXbytes;
	int nShift;
	unsigned char Image[TEXT_IMAGE_SIZE];
} RFontTextImage;

//---------------------------------------------------------
//	public �֐�
//---------------------------------------------------------
int RFontCreateStandardFont(void);
void RFontFreeStandardFont(void);
int RFontCreate(RFont *font, char *hfont_name, char *zfont_name, char *hx2font_name, char *zx2font_name);
void RFontFree(RFont *font);
RFontTextImage *RFontCreateTextImage(RFont *font, const char* text, int mode);
void RFontShiftTextImage(RFontTextImage *image, int shift);

int	RFontGetFontSizeX(RFont* pFont, int nMode);	// 2006/12/07 by araki
int	RFontGetFontSizeY(RFont* pFont, int nMode);	// 2006/12/07 by araki

extern RFont*	pfontCurrent;		// 2006/12/07 by araki
extern RFont	fontSys;
extern RFont	fontStd;

#ifdef __cplusplus
}
#endif

#endif	//R_FONT_H_
