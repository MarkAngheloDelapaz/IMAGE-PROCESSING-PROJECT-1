/******************************************************************************
	R_hex.h / R_hex.c
	
	�ύX����
	Ver 1.04	1994/02/16	���������[�X
	Ver 3.50	2009/06/09	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_HEX_H_
#define	R_HEX_H_

// �w�L�T�t�H�[�}�b�g�̕ϊ�
unsigned int r_sethex(unsigned char *buff,unsigned char bin,unsigned int sum);
int R_bin2intelhex(unsigned char *buff,unsigned int addres,unsigned char *src, int byte);

#endif	// R_HEX_H_
