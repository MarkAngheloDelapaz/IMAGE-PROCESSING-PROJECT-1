/******************************************************************************
	R_logo.h / R_logo.c
	
	�ύX����
				1999/09/02	BIN_BX->H_step�ɕύX
	Ver 3.50	2009/02/13	DOS & LINUX �\�[�X����
	Ver 4.01	2009/09/16	x86_64 �\�[�X����
******************************************************************************/

#ifndef	R_LOGO_H_
#define	R_LOGO_H_

#ifdef __x86_64__
#include "R_plane.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define		LOGO_NORMAL_1	1
#define		LOGO_EDGE_1		2
#define		LOGO_NORMAL_2	3
#define		LOGO_EDGE_2		4
#define		LOGO_CHAR_1		0x10
#define		LOGO_CHAR_2		0x20

#define		LOGO_NOMAL_1	1		// BugName
#define		LOGO_NOMAL_2	3		// BugName


#ifdef	R_LOGO 
		unsigned int	rohmlogo[3][22*4] = {
		  {
			0xe00fffff,0xffc0ffff,0xfff81fc0,0x0000ffff,
			0xf01fffff,0xffc1ffff,0xfffc1fc0,0x0001ffff,
			0xf83fffff,0xffc3ffff,0xfffe1fc0,0x0003ffff,
			0xf83fffff,0xffc3ffff,0xfffe1fc0,0x0003ffff,
			0xf83fffff,0xffc3fe1f,0xfffe1fc0,0x0003ffff,
			0xf83ff8ff,0xffc3fe1f,0xfffe1fc0,0x0003ffff,
			0xf83ff8ff,0xffc3fe1f,0xe3fe1fc0,0x0003fe3f,
			0xf83ff8ff,0xffc3fe1f,0xe3fe1fc0,0x0003fe3f,
			0xf81ff8ff,0xffc3fe1f,0xe3fe1fff,0x0003fe3f,
			0xf807f8ff,0xffc3fe1f,0xe3fe1fff,0x0003fe3f,
			0xf80ff8ff,0xffc3fe1f,0xe3fe1fff,0x0003fe3f,
			0xf80fe0ff,0xffc3fe03,0xe07e1fff,0x0003fe07,
			0xf80fe0ff,0xffc3fe03,0xe07e1fff,0x0003fe07,
			0xf81fe0ff,0x1fc3fe03,0xe07e1ff8,0x0003fe07,
			0xf81fc0ff,0x1fc3fe03,0xe07e1ff8,0x0003fe07,
			0xf81fc0ff,0x1fc3fe03,0xe07e1ff8,0x0003fe07,
			0xf83fc0ff,0x1fc3fe03,0xe07e1ff8,0x0003fe07,
			0xf83f80ff,0x1fc3fe03,0xe07e1ff8,0x0003fe07,
			0xf83f80ff,0x1fc3ffff,0xe07e1ff8,0x0003fe07,
			0xf87f80ff,0x1fc3ffff,0xe07e1ff8,0x0003fe07,
			0xf07f00ff,0x1fc1ffff,0xe07e1ff8,0x0003fe07,
			0xe0ff00ff,0x1fc0ffff,0xe07e1ff8,0x0003fe07
		  },
		  {
			0xe00fffff,0xffc0ffff,0xfff81fc0,0x0000ffff,
			0x10100001,0x80410000,0x00041040,0x00010000,
			0x08200001,0x80420000,0x00021040,0x00020000,
			0x08200001,0x804203f0,0x00021040,0x00020000,
			0x08200f81,0x80420210,0x00021040,0x00020000,
			0x08200881,0x80420210,0x3e021040,0x000203e0,
			0x08200881,0x80420210,0x22021040,0x00020220,
			0x08200881,0x80420210,0x22021040,0x00020220,
			0x08100881,0x80420210,0x2202107f,0x00020220,
			0x080c0881,0x00420210,0x22021000,0x00020220,
			0x08083881,0x0042021e,0x23c21000,0x0002023c,
			0x08082081,0x00420202,0x20421000,0x00020204,
			0x08082081,0xf0420202,0x2042100f,0x00020204,
			0x08102081,0x10420202,0x20421008,0x00020204,
			0x08104081,0x10420202,0x20421008,0x00020204,
			0x08104081,0x10420202,0x20421008,0x00020204,
			0x08204081,0x10420202,0x20421008,0x00020204,
			0x08208081,0x10420202,0x20421008,0x00020204,
			0x08208081,0x104203fe,0x20421008,0x00020204,
			0x08408081,0x10420000,0x20421008,0x00020204,
			0x10410081,0x10410000,0x20421008,0x00020204,
			0xe0ff00ff,0x1fc0ffff,0xe07e1ff8,0x0003fe07
		  }
		};

		unsigned int	rohmlogo_s[2][11*2] = {
		  {
			0xf8ff83ff,0x00fffc78,
			0xf9ffc7ff,0x01fffe78,
			0xf9e7c7ff,0x01fffe78,
			0xf9e7c7cf,0x01e79e78,
			0xf9e7c7cf,0x01e79e7f,
			0xf9e7c3cf,0x01e79e7f,
			0xf9e1c38f,0x01e38e7f,
			0x79e1c78f,0x01e38e7c,
			0x79e1c78f,0x01e38e7c,
			0x79ffc70f,0x01e38e7c,
			0x78ff8f0f,0x01e38e7c
		  }
		  ,
		  {
			0xf8ff87ff,0x00fffe3c,
			0x89004801,0x01000124,
			0x891c4871,0x011c7124,
			0x89144851,0x01145124,
			0x89144851,0x01145127,
			0x09164651,0x01165920,
			0xc9124491,0x01124923,
			0x49124491,0x01124922,
			0x491e4911,0x01124922,
			0x49004911,0x01124922,
			0x78ff9e1f,0x01f3cf3e
		  }
		};

		unsigned int	rohmlogo_f[16*4] = {
			0x8000fffe,0x03a1c07f,0x07f07700,0x00001c1c,
			0xc001ffff,0x03b1c07f,0x7fff7700,0x00001c1c,
			0xc001ffff,0x9ff7e07f,0xf80fbfff,0x00001c7e,
			0xc001f83f,0x9ff7e00f,0xc001bfff,0x0000ffbe,
			0xe001f83f,0x0381c007,0x00000700,0x0000ffb0,
			0xe001f83f,0x0381c007,0x7ffe0700,0x00001c38,
			0xe7fdf83f,0x0383e007,0x7ffe0700,0x00001c18,
			0xf7fdf83f,0x9ff3e003,0x000007ff,0x00001c1c,
			0xf7fdf83f,0x9ff7f003,0xffff07ff,0x00001c1c,
			0xf7fdf83f,0x0381c7e3,0xffff071c,0x00001c3e,
			0xf801f83f,0x0381c7e1,0x001c071c,0x00001c3e,
			0xf801f83f,0x07c1c7e1,0x001c0e1c,0x00001c7f,
			0xf801f83f,0x0fe1cfc1,0x700e0e1c,0x00001c1c,
			0xfc01ffff,0x1ff1cfff,0xe00e1c1c,0x00001c1c,
			0xfc01ffff,0xbbb9cfff,0xffff7bff,0x0001ffdc,
			0xf800fffe,0xb399c7ff,0xefff71ff,0x0001ffdc
		};


		unsigned int	rohmlogo_fs[9*2] = {
			0x6b0f00fe,0x00631eb0,
			0xff8f01ff,0x0067b37e,
			0x630381c7,0x00f661b0,
			0x6783bdc7,0x00631e30,
			0xffc3bdc7,0x0063003e,
			0x6331c1c7,0x0067bf6c,
			0xf371c1c7,0x00630c6c,
			0xfb7fc1ff,0x006366ed,
			0x6b3f80fe,0x01fb5fdf
		};


#endif



/*	���S�̍���

	�P�D���l�߂Ńr�b�g�𗧂Ă����������}�[�N�B�r�b�g�𗧂ĂȂ������X�y�[�X�Ńp�^�[�������܂��B
	�Q�Dlogoconv.exe�Ńr�b�g�p�^�[���ɕϊ����܂��B
	�R�D���̃w�b�_�[�t�@�C���ɓo�^���܂��B

	�g�p�����p�^�[��

@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@@      @@@@@@@@@@      @@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@@@@@@@  
@@@@@@@@@@@@@@@@@@@@@       @@@@@@@@@@@@@@@@@@@@@     @@@@@@@@@@      @@@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@     @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@@@@@      @@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@     @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@@@@@      @@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@     @@@@@@@@@@    @@@@@@@@@    @@@@@@@@@@      @@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@   @@@@@@@@@@@     @@@@@@@@@@    @@@@@@@@@    @@@@@@@@@@      @@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@   @@@@@@@@@@@     @@@@@@@@@@    @@@@@@@@@    @@@@@@@@@@      @@@@@@@    @@@@@@@@@   @@@@@@@@@   @@@@@@@@@
@@@@@@@@   @@@@@@@@@@@     @@@@@@@@@@    @@@@@@@@@    @@@@@@@@@@      @@@@@@@    @@@@@@@@@   @@@@@@@@@   @@@@@@@@@
@@@@@@@@   @@@@@@@@@@      @@@@@@@@@@    @@@@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@@@@   @@@@@@@@@   @@@@@@@@@
@@@@@@@@   @@@@@@@@        @@@@@@@@@@    @@@@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@@@@   @@@@@@@@@   @@@@@@@@@
@@@@@@@@   @@@@@@@@@       @@@@@@@@@@    @@@@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@@@@   @@@@@@@@@   @@@@@@@@@
@@@@@@@@     @@@@@@@       @@@@@@@       @@@@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@     @@@@@@@       @@@@@@@       @@@@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@     @@@@@@@@      @@@@@@@       @@@@@@@@@    @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@      @@@@@@@      @@@@@@@       @@@@@@@@@    @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@      @@@@@@@      @@@@@@@       @@@@@@@@@    @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@      @@@@@@@@     @@@@@@@       @@@@@@@@@    @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@       @@@@@@@     @@@@@@@       @@@@@@@@@    @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@       @@@@@@@     @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@       @@@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@    @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@        @@@@@@@     @@@@@@@@@@@@@@@@@@@@@     @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@
@@@@@@@@        @@@@@@@@     @@@@@@@@@@@@@@@@@@@      @@@@@@@      @@@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@



@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@@      @@@@@@@@@      @@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@@@@@@@  
@                   @       @                   @     @       @      @      @     @                             @ 
@                    @     @                     @    @       @      @      @    @                               @
@                    @     @        @@@@@@       @    @       @      @      @    @                               @
@      @@@@@         @     @        @    @       @    @       @      @      @    @                               @
@      @   @         @     @        @    @       @    @       @      @      @    @       @@@@@       @@@@@       @
@      @   @         @     @        @    @       @    @       @      @      @    @       @   @       @   @       @
@      @   @         @     @        @    @       @    @       @      @      @    @       @   @       @   @       @
@      @   @        @      @        @    @       @    @       @@@@@@@@      @    @       @   @       @   @       @
@      @   @      @@       @        @    @       @    @                     @    @       @   @       @   @       @
@      @   @@@     @       @     @@@@    @       @    @                     @    @    @@@@   @    @@@@   @       @
@      @     @     @       @     @       @       @    @                     @    @    @      @    @      @       @
@      @     @     @       @     @       @       @    @      @@@@@@@@       @    @    @      @    @      @       @
@      @     @      @      @     @       @       @    @      @      @       @    @    @      @    @      @       @
@      @      @     @      @     @       @       @    @      @      @       @    @    @      @    @      @       @
@      @      @     @      @     @       @       @    @      @      @       @    @    @      @    @      @       @
@      @      @      @     @     @       @       @    @      @      @       @    @    @      @    @      @       @
@      @       @     @     @     @       @       @    @      @      @       @    @    @      @    @      @       @
@      @       @     @     @     @@@@@@@@@       @    @      @      @       @    @    @      @    @      @       @
@      @       @      @    @                     @    @      @      @       @    @    @      @    @      @       @
@      @        @     @     @                   @     @      @      @       @    @    @      @    @      @       @
@@@@@@@@        @@@@@@@@     @@@@@@@@@@@@@@@@@@@      @@@@@@@@      @@@@@@@@@    @@@@@@      @@@@@@      @@@@@@@@@


 @@@@@@@@@@@@@@@               @@@@@@@@       @@@    @ @@@              @@@ @@@     @@@@@@@       @@@     @@@    
@@@@@@@@@@@@@@@@@             @@@@@@@@@       @@@   @@ @@@              @@@ @@@ @@@@@@@@@@@@@@@   @@@     @@@    
@@@@@@@@@@@@@@@@@             @@@@@@@@@      @@@@@@ @@@@@@@@@  @@@@@@@@@@@@@@@ @@@@@       @@@@@ @@@@@@   @@@    
@@@@@@     @@@@@@             @@@@@@         @@@@@@ @@@@@@@@@  @@@@@@@@@@@@@@@ @@             @@ @@@@@ @@@@@@@@@ 
@@@@@@     @@@@@@            @@@@@@           @@@      @@@              @@@                         @@ @@@@@@@@@ 
@@@@@@     @@@@@@            @@@@@@           @@@      @@@              @@@      @@@@@@@@@@@@@@    @@@    @@@    
@@@@@@     @@@@@@ @@@@@@@@@  @@@@@@          @@@@@     @@@              @@@      @@@@@@@@@@@@@@    @@     @@@    
@@@@@@     @@@@@@ @@@@@@@@@ @@@@@@           @@@@@  @@@@@@@@@  @@@@@@@@@@@@                       @@@     @@@    
@@@@@@     @@@@@@ @@@@@@@@@ @@@@@@          @@@@@@@ @@@@@@@@@  @@@@@@@@@@@@     @@@@@@@@@@@@@@@@  @@@     @@@    
@@@@@@     @@@@@@ @@@@@@@@@ @@@@@@   @@@@@@   @@@      @@@        @@@   @@@     @@@@@@@@@@@@@@@@ @@@@@    @@@    
@@@@@@     @@@@@@          @@@@@@    @@@@@@   @@@      @@@        @@@   @@@       @@@            @@@@@    @@@    
@@@@@@     @@@@@@          @@@@@@    @@@@@@   @@@     @@@@@       @@@    @@@      @@@           @@@@@@@   @@@    
@@@@@@     @@@@@@          @@@@@@     @@@@@@  @@@    @@@@@@@      @@@    @@@     @@@        @@@   @@@     @@@    
@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@  @@@   @@@@@@@@@     @@@     @@@    @@@         @@@  @@@     @@@    
@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@  @@@  @@@ @@@ @@@ @@@@@@@@@@@ @@@@ @@@@@@@@@@@@@@@@  @@@ @@@@@@@@@@@
 @@@@@@@@@@@@@@@           @@@@@@@@@@@@@@@@   @@@  @@  @@@  @@ @@@@@@@@@@   @@@ @@@@@@@@@@@@ @@@  @@@ @@@@@@@@@@@


@@@@@@@@@@     @@@@@@@@@   @@@@@   @@@@   @@@@@@@@@@@@@@ 
@@@@@@@@@@@   @@@@@@@@@@@  @@@@@   @@@@  @@@@@@@@@@@@@@@@
@@@@@@@@@@@   @@@@@  @@@@  @@@@@   @@@@  @@@@@@@@@@@@@@@@
@@@@  @@@@@   @@@@@  @@@@  @@@@@   @@@@  @@@@  @@@@  @@@@
@@@@  @@@@@   @@@@@  @@@@  @@@@@@@@@@@@  @@@@  @@@@  @@@@
@@@@  @@@@    @@@@@  @@@@  @@@@@@@@@@@@  @@@@  @@@@  @@@@
@@@@   @@@    @@@    @@@@  @@@@@@@@@@@@  @@@   @@@   @@@@
@@@@   @@@@   @@@    @@@@  @@@@   @@@@@  @@@   @@@   @@@@
@@@@   @@@@   @@@    @@@@  @@@@   @@@@@  @@@   @@@   @@@@
@@@@    @@@   @@@@@@@@@@@  @@@@   @@@@@  @@@   @@@   @@@@
@@@@    @@@@   @@@@@@@@@   @@@@   @@@@@  @@@   @@@   @@@@


@@@@@@@@@@@    @@@@@@@@@   @@@@@  @@@@   @@@@@@@@@@@@@@@ 
@          @  @         @  @   @  @  @  @               @
@   @@@    @  @   @@@   @  @   @  @  @  @   @@@   @@@   @
@   @ @    @  @   @ @   @  @   @  @  @  @   @ @   @ @   @
@   @ @    @  @   @ @   @  @   @@@@  @  @   @ @   @ @   @
@   @ @  @@   @  @@ @   @  @         @  @  @@ @  @@ @   @
@   @  @  @   @  @  @   @  @  @@@@   @  @  @  @  @  @   @
@   @  @  @   @  @  @   @  @  @  @   @  @  @  @  @  @   @
@   @   @  @  @  @@@@   @  @  @  @   @  @  @  @  @  @   @
@   @   @  @  @         @  @  @  @   @  @  @  @  @  @   @
@@@@@    @@@@  @@@@@@@@@   @@@@  @@@@@  @@@@  @@@@  @@@@@



 @@@@@@@        @@@@    @@ @ @@     @@ @ @@@@   @@   @@  
@@@@@@@@@       @@@@   @@@@@@@@@ @@@@@@ @@  @@ @@@@  @@  
@@@   @@@      @@@      @@   @@     @@ @@    @@  @@ @@@@ 
@@@   @@@ @@@@ @@@     @@@@  @@     @@   @@@@   @@   @@  
@@@   @@@ @@@@ @@@    @@@@@@@@@@ @@@@@          @@   @@  
@@@   @@@     @@@   @@  @@   @@   @@ @@ @@@@@@ @@@@  @@  
@@@   @@@     @@@   @@@ @@  @@@@  @@ @@   @@    @@   @@  
@@@@@@@@@     @@@@@@@@@ @@ @@@@@@ @@ @@@ @@  @@ @@   @@  
 @@@@@@@       @@@@@@@  @@ @ @@ @@@@@ @@@@@@@ @ @@ @@@@@@

*/













#ifdef __x86_64__
int R_draw_logo(R_PLANE* plane, int xx, int yy, int mode_);
#else
int R_draw_logo( int xx, int yy, int mode );
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_LOGO_H_