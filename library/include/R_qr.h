/******************************************************************************

	QR�R�[�h�p���C�u�����w�b�_�t�@�C��

	2000.xx.xx	Ver.1.0����				by matsu
	2001.10.12	QR_INFO��status��ǉ�	by matsu
	2002. 2.28	�t�@�C���_�p�^�[���T�����@�ύX�A�G���[�R�[�h���ꉻ	by matsu
	2002. 6.27	�t�@�C���_�p�^�[�����W���o��			by mastu
	2002.11. 5	QR-READER�p�֐��ǉ�		by saito
	2009.02.12	DOS & LINUX �\�[�X����

******************************************************************************/

#ifndef	R_QR_H_
#define	R_QR_H_

#ifdef __cplusplus
extern "C" {
#endif

// QR�R�[�h�G���[�������x��
#define ERROR_L		1		// ���x���k�i�`7%�j
#define ERROR_M		0		// ���x���l�i�`15%�j
#define ERROR_Q		3		// ���x���p�i�`25%�j
#define ERROR_H		2		// ���x���g�i�`30%�j

// QR�f�R�[�h�X�e�[�^�X�t���O
/*
#define FINDER_ERR	0x01	// �t�@�C���_�p�^�[�����o�G���[
#define VER_ERR		0x03	// �o�[�W�����Z�o�G���[
*/
#define DECODE_OK	0x00	// �f�R�[�h����
#define ERR_READ_MEM		-9		// R_QR_read������malloc�G���[
#define ERR_FINDER			-10		// �t�@�C���_�p�^�[�����o�G���[
#define ERR_FINDER_UNDER	-11		// �t�@�C���_�p�^�[�����o�s��
#define ERR_FINDER_OVER		-12		// �t�@�C���_�p�^�[�����o�ߑ�
#define ERR_FINDER_GROUP	-13		// �t�@�C���_�p�^�[���O���[�v���G���[
#define ERR_FINDER_MEM		-19		// �t�@�C���_�p�^�[��malloc�G���[
#define ERR_VERSION			-20		// �o�[�W��������G���[
#define ERR_CLOCK			-30		// �N���b�N�ǂݎ��G���[
#define ERR_PATERN			-40		// �N���b�N�ǂݎ��G���[
#define ERR_PATERN_MEM		-49		// �N���b�N�ǂݎ��malloc�G���[
#define ERR_DECODE			-50		// �f�R�[�h�G���[
#define ERR_DECODE_FORMAT	-51		// �f�R�[�h(�@�\�p�^�[��)�G���[
#define ERR_DECODE_CORRECT	-52		// �f�R�[�h(�G���[����)�G���[
#define ERR_DECODE_MEM		-59		// �f�R�[�hmalloc�G���[


#define BIN_TEMP	2		// ��Ɨp�Q�l�摜�������ԍ�


#define QR_BOARD_ID0 0x00	//�p�q���[�_�g�p�{�[�h ID = 0
#define QR_BOARD_ID1 0x10	//�p�q���[�_�g�p�{�[�h ID = 1
#define QR_BOARD_ID2 0x20	//�p�q���[�_�g�p�{�[�h ID = 2
#define QR_BOARD_ID3 0x30	//�p�q���[�_�g�p�{�[�h ID = 3
#define QR_CHANNEL_0 0x00	//�p�q���[�_�g�p�摜���̓`�����l�� CH = 0
#define QR_CHANNEL_1 0x01	//�p�q���[�_�g�p�摜���̓`�����l�� CH = 1
#define QR_CHANNEL_2 0x02	//�p�q���[�_�g�p�摜���̓`�����l�� CH = 2
#define QR_CHANNEL_3 0x03	//�p�q���[�_�g�p�摜���̓`�����l�� CH = 3



// ���W�\����
typedef struct {
	int x;
	int y;
} POINT_DATA;

// QR�R�[�h���\����
typedef struct {
	int status;				// QR�f�R�[�h�X�e�[�^�X
	int module;				// ���W���[����
	int version;			// �o�[�W����
	int model;				// ���f��
	int level;				// �G���[�������x��
	int mask;				// �}�X�NNo
	int format1_err;		// �t�H�[�}�b�g���P���bit��					2002. 6.27 ���Ή�
	int *format1_adr;		// �t�H�[�}�b�g���P���ʒu�ւ̃A�h���X		2002. 6.27 ���Ή�
	int format2_err;		// �t�H�[�}�b�g���Q���bit��					2002. 6.27 ���Ή�
	int *format2_adr;		// �t�H�[�}�b�g���Q���ʒu�ւ̃A�h���X		2002. 6.27 ���Ή�
	int ver1_err;			// �o�[�W�������P���bit��					2002. 6.27 ���Ή�
	int *ver1_adr;			// �o�[�W�������P�����ʒu�ւ̃A�h���X		2002. 6.27 ���Ή�
	int ver2_err;			// �o�[�W�������Q���bit��					2002. 6.27 ���Ή�
	int *ver2_adr;			// �o�[�W�������Q�����ʒu�ւ̃A�h���X		2002. 6.27 ���Ή�
	int data_err;			// �f�[�^���byte��								2002. 6.27 ���Ή�
	int *data_adr;			// �f�[�^���ʒu�ւ̃A�h���X					2002. 6.27 ���Ή�
	POINT_DATA finder[3];	// �t�@�C���_�p�^�[�����o���W(�����o����-1)		2002. 7. 2
} QR_INFO;


// QR�R�[�h�֐��v���g�^�C�v
int R_QR_open(void);
void R_QR_close(void);
int R_QR_gray2bin(int gray_no, int bin_no);
int R_QR_read(int bin_no, QR_INFO *qr, unsigned char *data);	// sai henkou 2002/07/01

// QR�R�[�h���[�_�֐��v���g�^�C�v
int	R_QR_start(char *str, QR_INFO *qr, int flag, int channel);
//int R_QR_start(char *data, QR_INFO *qr, char channel);
int R_QR_init(void);

#ifdef __cplusplus
}
#endif

#endif	// R_QR_H_
