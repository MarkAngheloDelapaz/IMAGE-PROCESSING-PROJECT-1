/******************************************************************************
	R_SVGA.H / R_SVGA.C
	
	�ύX����
	Ver 		1999/11/12	VESA�r�f�I���[�h�̒ǉ�
	Ver 3.46	2005/08/16	VESA�r�f�I���[�h�̒ǉ�
	Ver 3.46	2005/08/16	R_VESA_set_RAMDAC�ǉ�
	Ver 3.46	2005/08/23	VESAMODE�\���̂�PhysBasePtr�ǉ�
	Ver 3.47	2005/09/16	VESAMODE�\���̂ɒǉ��C��
	Ver 3.47	2005/09/20	SVGAINFO�\���̂Ƀ����o�ǉ�
	Ver 3.49	2006/10/13	LinearMode��`�ǉ�
******************************************************************************/

#ifndef	R_SVGA_H_
#define	R_SVGA_H_

#include "R_stdlib.h"
#include "R_vbe.h"

#ifdef __cplusplus
extern "C" {
#endif

/* WAOCOM�Œ�`����Ă��Ȃ�VESA�r�f�I���[�h�̒ǉ� 1999.11.12 ohshima */
#define _SXRES16COLOR		0x106	// 16�F		1280x1024
#define _SXRES256COLOR		0x107	// 256�F	1280x1024
#define _VRES16MCOLOR		0x112	// 16M�F	640x480
#define _SRES16MCOLOR		0x115	// 16M�F	800x600
#define _XRES16MCOLOR		0x118	// 16M�F	1024x768
#define _SXRES16MCOLOR		0x11B	// 16M�F	1280x1024 �g�p�ł��Ȃ��r�f�I�J�[�h����������

#define	VESA_640x480x8		0x101	// VGA		8bit/pixel 640x480		// 2005/08/16
#define	VESA_800x600x8		0x103	// SVGA		8bit/pixel 800x600		// 2005/08/16
#define	VESA_1024x768x8		0x105	// XGA		8bit/pixel 1024x768		// 2005/08/16
#define	VESA_1280x1024x8	0x107	// SXGA		8bit/pixel 1280x1024	// 2005/08/16

#define	VESA_640x480x32		0x112	// VGA		32bit/pixel 640x480		// 2005/08/16
#define	VESA_800x600x32		0x115	// SVGA		32bit/pixel 800x600		// 2005/08/16
#define	VESA_1024x768x32	0x118	// XGA		32bit/pixel 1024x768	// 2005/08/16
#define	VESA_1280x1024x32	0x11B	// SXGA		32bit/pixel 1280x1024	// 2005/08/16

#define	VESAFUNC	0x4F
#define PELADDR_W	0x03C8		/* PEL�A�h���X���W�X�^(Write���[�h)	*/
#define PELDATA		0x03C9		/* PEL�f�[�^���W�X�^				*/

#define NORMAL_DRAW		0
#define TWICE_DRAW		1

#define	VESA_RAMDAC_GRAYSCALE	0

#define	BytesParScanLine		BytesPerScanLine
#define	BitsParPixel			BitsPerPixel
#define	DirectColorModelInfo	DirectColorModeInfo

//#pragma pack(1);
#pragma pack(1)		// 2009/03/10

#ifndef	RGBQUADstruct
/*** �r�b�g�}�b�v�f�[�^�̃p���b�g�\���� */
typedef	struct	{
	unsigned char	rgbBlue;			/* ��								*/
	unsigned char	rgbGreen;			/* ��								*/
	unsigned char	rgbRed;				/* ��								*/
	unsigned char	rgbReserved;		/* ���U�[�u							*/
} RGBQUAD;
#define	RGBQUADstruct
#endif

//#pragma pack();
#pragma pack()		// 2009/03/10




#ifdef	SVGA
	unsigned char * VGAMemoryModel[8] = {	"text mode",
							"CGA graphics",
							"�n�[�L�����[ graphics",
							"4bit plane planer",
							"pack pixcel",
							"��`�F�[���@�S�A�Q�T�U�J���[",
							"direct color",
							"YUV",
						};
	int	WindowNo;					// �E�B���h�ENo.
	int	WindowPos;					// �E�B���h�E�̌��݈ʒu
	unsigned int	Granularity;	// �E�B���h�E�A�h���X�؂�ւ��P��
	unsigned int	SVGAWindowsSize;	// �E�B���h�E�̑傫��
	char	*VRAMaddr;				// VRAM�A�h���X
	VESAMODE VESAmodeinfo;
	int	LineBufferXs, LineBufferYs;	// �a�����\���̃������[�o�b�t�@�[�J�n���W
	int	LineBufferXe, LineBufferYe;	// �a�����\���̃������[�o�b�t�@�[�I�����W
	unsigned char *LineBuffer=0;	// �a�����\���̃������[�o�b�t�@�[
	int	CircleBufferX,CircleBufferY,CircleBufferR;	// �~�\���̃������o�b�t�@�[���S���W
	unsigned char *CircleBuffer;					// �~�\���̃������o�b�t�@�[
#else
	extern unsigned char *VGAMemoryModel[];
	extern int	WindowNo;
	extern int	WindowPos;
	extern unsigned int	Granularity;
	extern unsigned int	SVGAWindowsSize;
	extern char	*VRAMaddr;
	extern char	*VRAMBaseAdr;
	extern VESAMODE VESAmodeinfo;
	extern int	LineBufferXs, LineBufferYs;
	extern int	LineBufferXe, LineBufferYe;
	extern unsigned char *LineBuffer;
	extern int	CircleBufferX,CircleBufferY,CircleBufferR;
	extern unsigned char *CircleBuffer;
#endif



int R_VESAGetSVGAInfo(SVGAINFO *svgainfo);
int R_VESAGetModeInfo(short int vesamode, VESAMODE *modeinfo);

unsigned char R_VESAGetVideoMode(void);
void R_VESASetVideoMode(unsigned char mode);
short R_VESAGetVESAMode(void);
int R_VESASetSVGAMode(short int vesamode);
void R_VESAPSet(int x,int y,unsigned char data);
void R_VESASetWindowPos(int winpos);
void R_VESAHLine(int x,int y, int length, char *buffer);
void R_VESASetRAMDAC(char number,RGBQUAD *color);
unsigned char R_VESAGetPalette(void);
unsigned char R_VESASetPalette(unsigned char color);
int R_VESASetGrayRAMDAC(void) ;
int R_VESASetBinRAMDAC( int thd, int bdata, int mode );
int R_transmit_video2svga( int x,int y, int xs, int ys, int xsize,int ysize, int mode, int page ) ;


void R_VESAgetHLine( int x,int y, int length, char *buffer );
unsigned char R_VESAPGet( int x,int y );

int	R_VESA_draw_image(int x,int y, int xs, int ys,char *buffer,int mode);

int	R_VESA_set_RAMDAC(int nMode);

#ifdef __cplusplus
}
#endif

#endif	// R_SVGA_H_
