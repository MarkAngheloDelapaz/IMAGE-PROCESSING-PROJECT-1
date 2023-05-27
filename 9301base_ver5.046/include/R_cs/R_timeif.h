/******************************************************************************

	R_TIMEIF.H

	���ԏ��擾�֐��w�b�_
	�o�[�W�����A�b�v���́A�K�������̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		2004.06.13.000		�I���W�i��
				2006.10.09			<test> ltm�ǉ�

******************************************************************************/

#include <time.h>

// --- define ---
//�����\����
typedef struct{
	int		year;				//�N
	int		month;				//��
	int		day;				//��
	int		hour;				//��
	int		minute;				//��
	int		second;				//�b
	struct tm	ltm;			// ��ctime,gmtime�Ăяo���ŏ㏑�������̂Ŏ��̂�錾
}R_TIME_INFO;

// --- variable ---

// --- prototype ---
void R_TimeInfoGetNow( R_TIME_INFO *date );
int R_TimeInfoGetFile( char *filename, R_TIME_INFO *date );

