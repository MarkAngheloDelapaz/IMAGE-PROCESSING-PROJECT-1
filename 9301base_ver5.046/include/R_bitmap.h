/******************************************************************************
	R_bitmap.h / R_bitmap.c
	
	�ύX����
				1999.06.10	*IMAGE_ADDRESS��`�ύX(R_main.c��)
				1999.09.17	640x480�Ή�
				1999.11.09	R_load_bmp2(),R_load_bmp3(),R_read_bmp()�̃v���g�^�C�v�ǉ�
				2002.04.01	R_load_bmp(), R_bmpview()��y=0�̃��C�������[�h���Ȃ��̂��C��
				2002.10.29	GRAY_PLANE�ABIN_PLANE�폜(���d��`)R_stdlib.h��
	Ver 3.48	2006.02.21	R_load_bmp()�ŉ摜�x�T�C�Y��FySize���傫���ꍇFySize�𒴂��ăf�[�^�����[�h���Ă��܂��̂��C��
	Ver 3.48	2006.03.03	R_load_bmp()�ŉ摜�Ǎ���̃��������m�ۂ���Ă��Ȃ��ꍇ�ǂݍ��݂𒆎~����悤�ɏC��
	Ver 3.49	2006.10.18	OK��ERROR��`���폜�AR_stdlib.h���C���N���[�h
	Ver 3.50	2009.04.01	DOS & LINUX �\�[�X����
	Ver.4.01	2013.11.13	R_get_bmpinfo�ǉ�
	Ver.4.01	2016.08.26	�e�֐��̈����̃t�@�C������"char*"����"const char*"�ɕύX
******************************************************************************/

/*
	�r�b�g�}�b�v�t�@�C���֘A���C�u����

 * BITMAP FILE�֌W�̐ݒ�iR_BITMAP.H�j
 *
 *	���r�b�g�}�b�v�t�@�C���̒�`�ł�


				<<<<<<<<<<<<<<<		caution!!	>>>>>>>>>>>>>>>>>

	#pragma pack(1)�́A�\���̂̃p�b�L���O�T�C�Y�������I�ɂP�ɂ�����̂ł��B����ɂ��R���p�C������
	/zp4���Ă�dword�̃p�b�N�͂���܂���B�܂��A#pragma pack()�́A�f�t�H���g�ɖ߂����̂ł��B�ڍׂ̐����́A
	�v�`�s�b�n�l�@�b�^�R�W�U�@�t�������f���@�f���������̂o�D�Q�P�P�̃p�b�N�v���O�}�̍��ɐ���������܂��B

*/

#ifndef	R_BITMAP_H_
#define	R_BITMAP_H_

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)	//;


// bit map file header
typedef	struct {
	unsigned short int	bfType;			// file type (charactor BM)
	unsigned long	bfSize;				// file total size
	unsigned short int	bfReserved1;	// reserved
	unsigned short int	bfReserved2;	// reserved
	unsigned long	bfOffBits;			/* ���ް���̧�ِ擪����̵̾��		*/
} BMPFILEHEADER;

/*** �r�b�g�}�b�v�C���t�H���[�V�����w�b�_ */
typedef struct {
	unsigned long	biSize;				// size of this headder
	unsigned long	biWidth;			/* �r�b�g�}�b�v�̕�(�s�N�Z���P��)	*/
	unsigned long	biHeight;			/* �r�b�g�}�b�v�̍���(�s�N�Z���P��)	*/
	unsigned short int	biPlanes;			/* �P�ɂ���							*/
	unsigned short int	biBitCount;			/* �s�N�Z��������̃r�b�g��			*/
	unsigned long	biCompression;		/* ���k�����i�񈳏k�O�j				*/
	unsigned long	biSizeImage;		/* �ޯ�ϯ���ޯāi���ް��j�̃T�C�Y	*/
	unsigned long	biXpelsParMeter;	/* 1Ұ�ق�����̐����߸�ِ�(�O�ŉ�) */
	unsigned long	biYpelsParMeter;	/* 1Ұ�ق�����̐����߸�ِ�(�O�ŉ�) */
	unsigned long	biClrUsed;			/* �J���[���i�O�ŉj				*/
	unsigned long	biClrImportant;		/* �d�v�ȃJ���[���i�O�ŉj			*/
} BMPINFOHEADER;

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

/*** �������    ***********************/
#define	SCREEN_LINES		480			/* ���{��MODE�R�̃��C����			*/
#define SCREEN_WIDTH_BYTE	64			/* ���{��MODE�R�̂P�s�\���o�C�g��	*/
#define COLORS				16			/* ���{��MODE�R�̐F��				*/
#define PLANES				4			/* ���{��MODE�R�̃v���[����			*/
//#define GRAY_PLANE		0x01
//#define BIN_PLANE			0x02
#define	REVERSE				0x0004
#define RESO1_1				0x0010
#define RESO1_2				0x0020
#define RESO1_4				0x0040
#define COLOR16				0x0000
#define COLOR256			0x0100
#define COLOR2				0x0200
#define COLOR16M24			0x0400
#define COLOR16M32			0x0800
#define EX_GRAY_PLANE		0x1000
#define EX_BIN_PLANE		0x2000
#define COLORMASK			(COLOR2|COLOR16|COLOR256|COLOR16M24|COLOR16M32)


#pragma pack( )	//;


#ifdef	R_BITMAP

/*** ���C���o�b�t�@   **********************/

/*** BITMAP FILE HEADER ****************/
BMPFILEHEADER	bmpfh = {
			'MB',
			sizeof( BMPFILEHEADER ) 
			+ sizeof( BMPINFOHEADER )
			+ 64 			/* sizeof( RGBQUAD ) * COLORS 				*/
			+ 0,		/* SCREEN_WIDTH_BYTE * SCREEN_LINES *PLANES	*/
			0,
			0,
			sizeof( BMPFILEHEADER ) 
			+ sizeof( BMPINFOHEADER )
			+ 64			/* sizeof( RGBQUAD ) * COLORS				*/
};

/*** BITMAP INFO HEADER ***************/
BMPINFOHEADER	bmpih = {
			sizeof( BMPINFOHEADER ),
			0,			/* SCREEN_WIDTH_BYTE * sizeof( char )		*/
			0,
			1,
			PLANES,
			0,
			0,
			0,
			0,
			0,
			0
};

BMPFILEHEADER	bmpf;
BMPINFOHEADER	bmpi;
RGBQUAD			rgb;

#else
#endif

/*** �v���g�^�C�v�錾 ******************/
void getBINline( int mode,int xsize,unsigned char* pointdata1,unsigned char* pointdata2,unsigned char* data );
int	R_save_bmp(int x, int y, int xsize, int ysize, int mode, const char* filename);
int	R_save_bmp_m(int x, int y, int xsize, int ysize, int mode, const char* filename);
int	R_save_bmp_c(int x, int y, int xsize, int ysize, int mode, const char* filename);
int	R_save_bmp3(const char* filename, int page, int xs, int ys, int xw, int yw);
int	R_load_bmp(const char* filename, int x_start, int y_start);
int	R_bmpview(const char* filename, int x_start, int y_start);
int	R_load_bmp2(const char* filename, int page, int xs, int ys, int xw, int yw);
int	R_load_bmp3(const char* filename, int page, int xs, int ys, int xw, int yw);
int	R_read_bmp(const char* filename, char** address, int* x_size, int* y_size);
int	R_get_bmpinfo(const char* filename, BMPINFOHEADER* pInfo);

#ifdef __cplusplus
}
#endif

#endif	// R_BITMAP_H_
