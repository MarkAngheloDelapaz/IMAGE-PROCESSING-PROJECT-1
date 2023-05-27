/******************************************************************************
	R_alarm.h
	
	�ύX����
	Ver 4.01	2014/10/30	�V�K�쐬
******************************************************************************/

#ifndef	R_ALARM_H_
#define	R_ALARM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int		code;
	int		class_code;			// �A���[���̃N���X
	int		priority;			// �A���[���̗D�揇��
	const char*	title_jp;		// �A���[���̃��b�Z�[�W
	const char*	title_en;		// �A���[���̃��b�Z�[�W

	int		total_counter;		// �A���[�����׉񐔃J�E���^
	int		prior_counter;		// �A���[���̉񐔃J�E���^
} R_ALARM_ITEM;

typedef struct {
	int		code;
	const char*	title_jp;		// �A���[���̃��b�Z�[�W
	const char*	title_en;		// �A���[���̃��b�Z�[�W
} R_ALARM_CLASS;

typedef struct {
//	int		prior_number;		// �ŏI����̃A���[���ԍ�
//	int
	int		prior_code;
	int		current_priority;

	int		total_of_alarm;		// �A���[������������
/*
	int*	total_counter;		// �A���[�����׉񐔃J�E���^
	int*	prior_counter;		// �A���[���̉񐔃J�E���^
	
	int*	priority;			// �A���[���̗D�揇��
	const char**	message;	// �A���[���̃��b�Z�[�W
	int*	class;				// �A���[���̃N���X
*/
	R_ALARM_ITEM*	item;		//
	R_ALARM_CLASS*	class_item;
	
	int		num_of_code;			// �A���[���R�[�h�̍ő吔
	int		num_of_class;
	
	int		bytes_of_flags;			//
	int*	flags;					// �����A���[���t���O
	int*	backup_of_flags;		// �����A���[���t���O�̕��A�p�o�b�N�A�b�v
	int		backup_of_prior_code;	// �ŏI����A���[���ԍ��̕��A�p�o�b�N�A�b�v
} R_ALARM;

R_ALARM*	R_alarm_open(int max_code);
void	R_alarm_close(R_ALARM* alarm);
int	R_alarm_counter_init(R_ALARM* alarm);
int	R_alarm_set(R_ALARM* alarm, int alarm_number);
int	R_alarm_decide(R_ALARM* alarm, int mode);
int	R_alarm_resume(R_ALARM* alarm);
int	R_alarm_load(R_ALARM* alarm, const char* filename);

// �J�E���^���ލ\����
typedef struct {
	int nClass;						// �J�E���^���ޔԍ�
	char* sMessJp;					// ���ޖ���(���{��)
	char* sMessEn;					// ���ޖ���(�p��)
//} R_CS_COUNTER_CLASS;
} R_ALARM_COUNTER_CLASS;

// �J�E���^�m�f�ԍ��\����
typedef struct {
	int   nCode;					// �m�f�ԍ�
	char* sMessJp;					// �m�f����(���{��)
	char* sMessEn;					// �m�f����(�p��)
	int   nClass;					// ���ޔԍ�
	int		priority;
//} R_CS_COUNTER_NG_CODE;
} R_ALARM_COUNTER_NG_CODE;

int R_alarm_init_by_struct(R_ALARM* alarm, R_ALARM_COUNTER_NG_CODE* ng_code, R_ALARM_COUNTER_CLASS* class_code);

#ifdef __cplusplus
}
#endif

#endif	// R_ALARM_H_
