/******************************************************************************
	R_sio2.h / R_sio2.c
	
	�ύX����
	Ver 3.32	2002/09/27	���������[�X							by araki
	Ver 3.43	2004.08.06	����|�[�g�̃{�[���[�g�ύX�Ή�			by matsu
							R_sio2_close��R_sio2_open�ɂčĐݒ肷��
	Ver 4.01	2009/08/20	DOS & LINUX �\�[�X����					by araki
******************************************************************************/

#ifndef	R_SIO2_H_
#define	R_SIO2_H_

// �`�����l��
									//	DOS		Linux
#define	SIO_CHANNEL0	0x10000		//	COM1	/dev/ttyS0
#define	SIO_CHANNEL1	0x20000		//	COM2	/dev/ttyS1
#define	SIO_CHANNEL2	0x30000		//	COM3	/dev/ttyS2
#define	SIO_CHANNEL3	0x40000		//	COM4	/dev/ttyS3

#define	SIO_CHANNEL(num)	(((num)+1)<<16)
#ifdef __DOS__
	#define	SIO_CHANNEL_MASK	0xF0000
#elif __GNUC__
	#define	SIO_USB				0x20000000
	#define	SIO_ACM				0x40000000
	#define	SIO_CHANNEL_MASK	(0xF0000 | SIO_USB | SIO_ACM)
#endif

// �f�[�^�r�b�g��
#define	BIT_5		0x00
#define	BIT_6		0x01
#define	BIT_7		0x02
#define	BIT_8		0x03

// �X�g�b�v�r�b�g��
#define	STOP_1		0x00
#define	STOP_2		0x04

// �p���e�B
#define	PARITY_NONE	0x00
#define	PARITY_ODD	0x08
#define	PARITY_EVEN	0x18
#ifdef __DOS__
	#define	PARITY_1	0x28
	#define	PARITY_0	0x38
#endif

// �ʐM���x
#define	RATE115200BPS	115200
#define	RATE57600BPS	57600
#ifdef __DOS__
	#define	RATE28800BPS	28800
	#define	RATE23040BPS	23040
#endif
#define	RATE38400BPS	38400
#define	RATE19200BPS	19200
#define	RATE9600BPS		9600
#define	RATE4800BPS		4800
#define	RATE2400BPS		2400
#define	RATE1200BPS		1200
#define	RATE600BPS		600
#define	RATE300BPS		300
#define	RATE150BPS		150
#define	RATE75BPS		75

// �t���[����
#define	FLOW_NONE		0x00
#define	FLOW_SOFT		0x01
#define	FLOW_HARD		0x02

#ifdef __cplusplus
extern "C" {
#endif

/*
	�֐����F	�q�Q�������Q�Q��������

	�@�@�\�F	�q�r�Q�R�Q�b�h���C�o�������i�����`�����l���Ή��j

	�`�@���F	#include "R_sio2.h"
				int R_sio2_open( int channel, int bps, int len, int parity, int stops, int flow );

	�������F	channel	�������`�����l��
					�l		��@��			�Ӂ@��
											DOS�̏ꍇ		Linux�̏ꍇ
					0x10000	SIO_CHANNEL0	COM0			/dev/ttyS0
					0x20000	SIO_CHANNEL1	COM1			/dev/ttyS1
					0x30000	SIO_CHANNEL2	COM2			/dev/ttyS2
					0x40000	SIO_CHANNEL3	COM3			/dev/ttyS3
					�c�n�r�̏ꍇ
					�@channel�ɑ�������|�[�g��BIOS���F�����Ă��Ȃ��ꍇ�A��L�`�����l����or����
					�@�����I�Ƀx�[�X�A�h���X�|�[�g���w�肵�ď��������邱�Ƃ��o���܂��B�i���ӎ������Q�Ɓj
					�@EX)	SIO_CHANNEL|0x2f8
					�k���������̏ꍇ
					���L�r�b�g��or���邱�Ƃɂ��A�ʃf�o�C�X�����������܂�
					0x20000000		SIO_USB		/dev/ttyUSB?	�t�r�a�V���A��
					0x40000000		SIO_ACM		/dev/ttyACM?	�b�c�b�f�o�C�X

				bps	�ʐM���x
					�l		��@��			�Ӂ@��
					115200	RATE115200BPS	�P�P�T�Q�O�O������
					57600	RATE57600BPS	�T�V�U�O�O������
				*	28800	RATE28800BPS	�Q�W�W�O�O������
				*	23040	RATE23040BPS	�Q�R�O�S�O������
					38400	RATE38400BPS	�R�W�S�O�O������
					19200	RATE19200BPS	�P�X�Q�O�O������
					9600	RATE9600BPS		�X�U�O�O������
					4800	RATE4800BPS		�S�W�O�O������
					2400	RATE2400BPS		�Q�S�O�O������
					1200	RATE1200BPS		�P�Q�O�O������
					600		RATE600BPS		�U�O�O������
					300		RATE300BPS		�R�O�O������
					150		RATE150BPS		�P�T�O������
					75		RATE75BPS		�V�T������
					���@28800bps,23040bps�́A�c�n�r�̏ꍇ�̂ݎg�p�\

				len	�f�[�^��
					�l		��@��		�Ӂ@��
					0x00	BIT_5		�T�r�b�g
					0x01	BIT_6		�U�r�b�g
					0x02	BIT_7		�V�r�b�g
					0x03	BIT_8		�W�r�b�g

				parity	�p���e�B
					�l		��@��		�Ӂ@��
					0x00	PARITY_NONE	�Ȃ�
					0x08	PARITY_ODD	��p���e�B
					0x18	PARITY_EVEN	�����p���e�B
				*	0x28	PARITY_1	�O�Œ�
				*	0x38	PARITY_0	�P�Œ�
					���@�O�Œ�A�P�Œ�́A�c�n�r�̏ꍇ�̂ݎg�p�\

				stops	�X�g�b�v�r�b�g
					�l		��@��		�Ӂ@��
					0x00	STOP_1		�P�r�b�g
					0x04	STOP_2		�Q�r�b�g

				flow	�t���[����
					�l		��@��		�Ӂ@��
					0x00	FLOW_NONE	�t���[����Ȃ�
					0x01	FLOW_SOFT	�\�t�g�t���[�i�w�n�m�^�w�n�e�e�j����
					0x02	FLOW_HARD	�n�[�h�t���[�i�b�s�r�^�q�s�r�j����
	�߂�l�F
				�l						�Ӂ@��
				�n���h���ԍ�(1�ȏ�)		����I�����܂����B
				0						�ُ�I�����܂����B�i�����ُ�A�w��|�[�g���a�h�n�r�`�F�b�N�ő��݂��Ȃ��B�j

	���@���F	�w�肳�ꂽ�`�����l�����g�p�\�ɂ��܂��B
	���ӎ����F	�`�����l���w��ŁA�|�[�g���w�肵���ꍇ�A�|�[�g�w�肪�ԈႢ���ǂ����̃`�F�b�N�݂̂ɂȂ�܂��B
				�|�[�g�����݂��Ă��Ȃ��Ă��A�G���[�ɂ͂Ȃ�܂���B
*/
int		R_sio2_open(int channel, int bps, int len, int parity, int stops, int flow);




/*
	�֐����F	�q�Q�������Q�Q����������

	�@�@�\�F	�q�r�Q�R�Q�b�I������

	�`�@���F	#include "R_sio2.h"
				void R_sio2_close( int nHandle );

	�������F	nHandle		�n���h���ԍ�

	�߂�l�F	����܂���B

	���@���F	�q�Q�������Q�Q���������ŃI�[�v�����ꂽ�`���l���̎g�p���I�����܂��B

	���ӎ����F	���ɂ���܂���B
*/
void	R_sio2_close(int nHandle);




/*
	�֐����F	�q�Q�������Q�Q��������

	�@�@�\�F	��M�����̎擾

	�`�@���F	#include "R_sio2.h"
				int R_sio2_getc(int nHandle );

	�������F	nHandle  �n���h���ԍ�

	�߂�l�F	��M����������ꍇ���̃R�[�h�i�O���O�O�`�O�������j
	�@�@�@�@�@�@��M�o�b�t�@������̏ꍇ-1

	���@���F	��M�o�b�t�@����P�������o���܂��B

	���ӎ����F	���ɂ���܂���B
*/
int		R_sio2_getc(int nHandle);




/*
	�֐����F	�q�Q�������Q�Q��������

	�@�@�\�F	�P�������M

	�`�@���F	#include "R_sio2.h"
				void R_sio2_putc( int nHandle, int data );

	�������F	nHandle �n���h���ԍ�
				data	���M�����R�[�h

	�߂�l�F	����I��	�@�O
	�@�@�@�@�@�@���M���s	-1

	���@���F	�w�肳�ꂽ�����𑗐M���܂��B

	���ӎ����F	���ɂ���܂���B
*/
int		R_sio2_putc(int nHandle, int data);




/*
	�֐����F	�q�Q�������Q�Q��������

	�@�@�\�F	�����񑗐M

	�`�@���F	#include "R_sio2.h"
				void R_sio2_puts( int nHandle, char* str );

	�������F	nHandle		�n���h���ԍ�
				str			���M������|�C���^

	�߂�l�F	����I��	�@�O
	�@�@�@�@�@�@�ُ�I��	-1		���M�o�b�t�@����t

	���@���F	�w�肳�ꂽ������𑗐M���܂��B

	���ӎ����F	���ɂ���܂���B
*/
int		R_sio2_puts(int nHandle, char* str);




// �P�s�ǂݍ��݊֐�
// ���s�����܂ł̕������o�b�t�@�ɓǂݍ��ށB
// ���s�������Ȃ��Ă��A�w�肵����������ǂݍ��߂ΏI������B
// �^�C���A�b�v���Ԃ��w��ł��A�^�C���A�b�v����΃G���[�B
// �������A�^�C���A�b�v�܂œǂݍ��񂾕����̓o�b�t�@�Ɋi�[�����B
// �P�j���s�����ŏI��			������I�� return OK
// �Q�j�w�蕶�����܂œǂ�ŏI��	������I�� return OK
// �R�j�^�C���A�b�v�ŏI��		���ُ�I�� return ERROR
// �ǂ̃p�^�[���ŏI�����Ă��A�o�b�t�@�̒��̍Ō�̕����̂��ƂɕK��'\0'���t�������̂ŁA
// �ǂݍ��ݕ������́Astrlen�ȂǂŊm�F�ł���B
int		R_sio2_gets(int nHandle, char* str, int size, int time_limit);




/*
	�֐����F	�q�Q�������Q�Q��������������

	�@�@�\�F	���M�o�b�t�@�̖����M�������𓾂�

	�`�@���F	#include "R_sio2.h"
				void R_sio2_get_sndbuff( int nHandle );

	�������F	nHandle		�n���h���ԍ�

	�߂�l�F	�����M������

	���@���F	�܂����M���ꂸ�Ƀo�b�t�@���Ɏc���Ă���������𓾂܂��B

	���ӎ����F	���ɂ���܂���B
*/
int		R_sio2_get_sndbuff(int nHandle);



/*
	�֐����F	�q�Q�������Q�Q��������������

	�@�@�\�F	��M�o�b�t�@�̕������𓾂�

	�`�@���F	#include "R_sio2.h"
				void R_getsio_rcvbuff(int nHandle);

	�������F	nHandle		�n���h���ԍ�

	�߂�l�F	�����M������

	���@���F	��M�o�b�t�@�ɂ��܂��Ă��镶�����𓾂܂��B

	���ӎ����F	���ɂ���܂���B
*/
int		R_sio2_get_rcvbuff(int nHandle);




/*
	�֐����F	�q�Q�������Q���������Q������������

	�@�@�\�F	���C����Ԃ�Ԃ�
*/
int		R_sio2_get_line_status(int nHandle);

#ifdef __cplusplus
}
#endif

#endif	// R_SIO2_H_
