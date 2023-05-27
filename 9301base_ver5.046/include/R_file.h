/******************************************************************************
	R_FILE.H / R_FILE.C
	
	変更履歴
	Ver 3.50	2008/07/24	r_file_appendix_関数追加
	Ver 3.50	2009/03/30	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_FILE_H_
#define	R_FILE_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//	file handling programs

#define	GRAY_PLANE0	0
#define	GRAY_PLANE1	1
#define	GRAY_PLANE2	2
#define	GRAY_PLANE3	3
#define	BIN_PLANE0	4
#define	BIN_PLANE1	5
#define	BIN_PLANE2	6
#define	BIN_PLANE3	7
#define	COLOR_PLANE0	0x00010
#define	COLOR_PLANE1	0x00011
#define	COLOR_PLANE2	0x00012
#define	COLOR_PLANE3	0x00013
#define	COLOR_PLANE0_24	0x00014
#define	COLOR_PLANE1_24	0x00015
#define	COLOR_PLANE2_24	0x00016
#define	COLOR_PLANE3_24	0x00017
#define	EX_GRAY_PLANE0	0x10000
#define	EX_GRAY_PLANE1	0x10001
#define	EX_GRAY_PLANE2	0x10002
#define	EX_GRAY_PLANE3	0x10003
#define	EX_BIN_PLANE0	0x10004
#define	EX_BIN_PLANE1	0x10005
#define	EX_BIN_PLANE2	0x10006
#define	EX_BIN_PLANE3	0x10007
#define	ORG_PLANE	8

#define	SIZE_512_480	0
#define	SIZE_256_240	1
#define	SIZE_512_240	2
#define	SIZE_AUTO		3

int R_save_picture_with_comment(int, int, const char*, const char*);
int R_load_picture_with_comment( char *, int, char *, int  );
int R_save_picture( int, int, char * );
int R_load_picture( char *, int );
#ifdef __DOS__
unsigned long R_get_diskfree(int drive);
#elif __GNUC__
unsigned long R_get_diskfree(const char* path);
#endif

int	r_file_appendix_add(int (*func)(FILE*, int), int param);
int	r_file_appendix_write_data(FILE* fp, char* string, int data);
int	r_file_appendix_write_string(FILE* fp, char* string);
int	r_file_appendix_save(FILE* fp);

#ifdef __cplusplus
}
#endif

#endif	// R_FILE_H_
