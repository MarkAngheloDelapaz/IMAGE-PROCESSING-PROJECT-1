/******************************************************************************
	R_PFILE.H / R_PFILE.C
	
	�ύX����
	Ver 3.41	2003/12/27	���������[�X
	Ver 3.42	2004/01/29	�h���C�u�ړ��Ή�
	Ver 3.47	2005/11/17	PFILE_SEQ_FILE_MAX_NB 4->64 �ɕύX
	Ver 3.48	2006/05/08	static SetFileNameNumber -> static int SetFileNameNumber �C��
	Ver 3.48	2006/07/13	SVGA�\���Ή�
	Ver 3.49	2007/03/06	DRAW_PLANE�\���Ή�
				2007/03/06	�����񓙐���
				2007/03/07	r_pfile_sequential_view()�̃��[�h����ύX(���[�h�̍�����)
	Ver 3.50	2007/04/21	R_pfile_sequential_load_and_call_on_subdir()�̃}�E�X�ݒ�\��
	Ver 3.50	2009/04/06	DOS & LINUX �\�[�X����
	Ver 4.01	2011/06/01	�ő�t�@�C�����ݒ�Ή�
	Ver 4.01	2013/11/03	�t�@�C��OTHER�`���Ή�
	Ver 4.01	2013/11/12	�t�@�C��CMD�`���Ή�
	Ver 4.01	2014/12/10	R_pfile_sequential_set_func_other�̒ǉ�
******************************************************************************/

#ifndef	R_PFILE_H_
#define	R_PFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

//�摜�t�@�C���̎��
#define FILE_GMD 	0x00000000				//GMD�t�@�C��
#define FILE_BMP 	0x00000001				//BMP�t�@�C��
#define FILE_OTHER	0x00000002				//OTHER�t�@�C��
#define FILE_CMD	0x00000004				//CMD�t�@�C��

//���j���[�̍��ڔԍ�
#define PFILE_SEQ_MENU1 0	//���j���[�ԍ�1
#define PFILE_SEQ_MENU2 1	//���j���[�ԍ�2
#define PFILE_SEQ_MENU3 2	//���j���[�ԍ�3

//�\�����[�h
#define PFILE_SEQ_NO_WAIT		0x00000000	//�}�E�X�N���b�N�҂��Ȃ�
#define PFILE_SEQ_WAIT_CLICK	0x00000001	//��Ƀ}�E�X�N���b�N�҂�
#define PFILE_SEQ_WAIT_ERROR	0x00000002	//���s�֐���ERROR���̂݃}�E�X�N���b�N�҂�
#define PFILE_SEQ_WAIT_OK		0x00000004	//���s�֐���OK���̂݃}�E�X�N���b�N�҂�
#define PFILE_SEQ_MODE_SUBDIR	0x10000000	//R_pfile_sequential_load_and_call_on_subdir�����ݒ��L���ɂ���

#define PFILE_SEQ_FILE_NAME_MAX	16			//�t�@�C�����̍ő啶����
#define PFILE_SEQ_PATH_NAME_MAX	256			//�p�X���̍ő啶����
#define PFILE_SEQ_EXTENSION_NAME_MAX	5	//�g���q���̍ő啶����

typedef struct{								//PFILE_SEQ�f�[�^�\����
	char name   [PFILE_SEQ_PATH_NAME_MAX];	//���݃��[�h���Ă���t�@�C����(12�����܂�)
	char path   [PFILE_SEQ_PATH_NAME_MAX];	//���݃��[�h���Ă���t�@�C���̃t���p�X(256�����܂�)
	char comment[PFILE_SEQ_PATH_NAME_MAX];	//���݃��[�h���Ă���t�@�C���̃R�����g(GMD�`���̂�,256�����܂�)
}PFILE_SEQ_INFO;

//�֐��v���g�^�C�v�錾
int R_pfile_sequential_load_and_call(void);
int R_pfile_sequential_load_and_call_with_path(char *currentpath);
int R_pfile_sequential_load_and_call_on_subdir(void);
int R_pfile_sequential_set_filetype( int number, int file_type );
int R_pfile_sequential_set_filename( char *name, int page );
int R_pfile_sequential_set_func( int (*func)(PFILE_SEQ_INFO *info) );
int R_pfile_sequential_set_func_dir( int (*dir_func)(char *path) );
int R_pfile_sequential_set_menu( int number, char *j, char *e, void (*func)(void) );
int R_pfile_sequential_set_mode( int mode );
int R_pfile_sequential_set_dirname( char *dir_name );
int R_pfile_sequential_set_file_extension( char *extension_name );
int R_pfile_sequential_set_disp_time( int disp_time );
void R_pfile_set_svga_mode(void);
void R_pfile_set_draw_plane_mode(void);
void R_pfile_set_max_file_number(int number);
int R_pfile_sequential_set_func_other( int (*other_func)(PFILE_SEQ_INFO *info) );

#ifdef __cplusplus
}
#endif

#endif	// R_PFILE_H_
