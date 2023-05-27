/******************************************************************************
	R_ROBUST.H / R_ROBUST.C
	
	�ύX����
		Ver 3.41	2004/01/14	���������[�X
******************************************************************************/

#ifndef	__R_ROBUST_H__
#define	__R_ROBUST_H__

#ifdef __cplusplus
extern "C" {
#endif

#define	ROBUST_MODE			0x01	//�q�X�g�O�����̕]������	�ő�l
#define	ROBUST_SQUARE		0x02	//							��敽��

//���o�X�g��������\����
typedef struct{
	int deg_min;			//�ŏ��p�x 10�{�l[deg]
	int deg_max;			//�ő�p�x 10�{�l[deg]
	int deg_step;			//�p�x�X�e�b�v 10�{�l[deg]
	int frequency_rate_min;	//���W�_���ɑ΂���ŕp�l�̓x���̍Œኄ��(0-100%)
	int moving_average;		//�q�X�g�O�����ړ����ϋ��
	int method;				//�q�X�g�O�����]������
}ROBUST_CONDITION;

//���o�X�g���茋�ʍ\����
typedef struct{
	int angle;				//���W�_�Q�̊p�x 10�{�l[deg]
	int frequency;			//�ŕp�l�̓x��
	int position;			//�ŕp�l�̍��W(�A�t�B���ϊ���)
}ROBUST_RESULT;

#define	ROBUST_BUFF_MAX_NB	3
#ifdef __x86_64__
typedef struct {
	int*	RobustBuff[ROBUST_BUFF_MAX_NB];
	int*	RobustPrt;
	int		RobustBuffMaxSize;
} R_ROBUST_OBJ;
#endif

#ifdef __x86_64__
R_ROBUST_OBJ*	R_robust_open(int max_number);
int	R_robust_close(R_ROBUST_OBJ*);
int	R_robust_get_angle(R_ROBUST_OBJ*, int n, R_POINT* p, ROBUST_CONDITION condition, ROBUST_RESULT* result);
int	R_robust_sampling_line(R_ROBUST_OBJ*, int src_n, R_POINT* src_p, int* dst_n, R_POINT* dst_p, ROBUST_CONDITION condition, int number);
int	R_robust_get_line(R_ROBUST_OBJ*, int n, R_POINT* p, ROBUST_CONDITION condition, R_LINE* l);
int	R_robust_get_max_number(R_ROBUST_OBJ*);
#else
int R_robust_open( int max_number );
int R_robust_close( void );
int R_robust_get_angle(int n, R_POINT *p, ROBUST_CONDITION condition, ROBUST_RESULT *result);
int R_robust_sampling_line(int src_n, R_POINT *src_p, int *dst_n, R_POINT *dst_p, ROBUST_CONDITION condition, int number);
int R_robust_get_line(int n, R_POINT *p, ROBUST_CONDITION condition, R_LINE *l);
int R_robust_get_max_number( void );																							//�ő�\�f�[�^�Z�b�g���̎擾(2007.12.29 Nakahara)
#endif

#ifdef __cplusplus
}
#endif

#endif	// __R_ROBUST_H__
