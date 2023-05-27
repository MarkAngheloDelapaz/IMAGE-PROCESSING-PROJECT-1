/******************************************************************************

	LED�����֐��w�b�_
	�o�[�W�����A�b�v���́A�K�������̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version			name	comment
	2004.03.15.000	hirata	�I���W�i��
	2004.04.08.001	matsu	LedOn(),LedOff()�ǉ�
	2007.09.18.002	matsu	R_LedAdjustDemo�n�ǉ�
	2007.09.21		matsu	R_LedAdjust�̃T���v�����O�\����MODE��STDEV�ǉ�
	2007.09.27		matsu	R_LedAdjust�̃��C�A�E�g�v�Z���ύX(VGA�J�����ł͉�ʊO�ɕ\��)
					Ver.1.07b1.0
	2007.10.26		matsu	�ȉ��̊֐��ǉ�
						R_LedAdjustDemoCurrentLimitMaxSet/Get	// LED�����f���d���ő�l�̐ݒ�A�擾
						R_LedAdjustSingleModeSet/Get			// �V���O�����[�h�ݒ�A�擾
						R_LedAdjustLoopTimerSet/Get				// ���[�v�^�C�}�̐ݒ�A�擾
					Ver.1.07b1.1
	2008.01.07		matsu	�ȉ��̊֐��ǉ�
						R_LedAdjustSamplingModeSet/Get			// �T���v�����O���[�h�̐ݒ�A�擾

	2008.04.18		matsu	�Ɩ��������̕\���F�̐ݒ�E�擾�֐��ǉ�
								R_LedAdjustDrawColorSet/Get�ǉ�
	2008.04.26		matsu	LED_CH*�̐錾�C��
	2008.05.28		matsu	R_LED_PARA�̃`�����l���ݒ��R_LED_ADJUST�Ɠ����`���ɑΉ�

	1.07b1.4	2008.08.07	matsu	R_LedAdjustSamplingAverageGet()�ǉ�
	1.07b1.7	2009.06.xx	matsu	���[�U�{�^���ǉ��Ή�

******************************************************************************/

#ifndef __R_LED_H__
#define __R_LED_H__

#include "R_button.h"

#ifdef __cplusplus
extern "C"
{
#endif

// --- define ---
#define		LED_FLAG		0x80000000			// �ǉ�  2008.04.26
#define		LED_CH1			(0x01|LED_FLAG)		// DacChannel1	0x00000001 �� 0x80000001  �錾�C��  2008.04.26
#define		LED_CH2			(0x02|LED_FLAG)		// DacChannel2	0x00000002 �� 0x80000002
#define		LED_CH3			(0x04|LED_FLAG)		// DacChannel3	0x00000004 �� 0x80000004
#define		LED_CH4			(0x08|LED_FLAG)		// DacChannel4	0x00000008 �� 0x80000008
#define		LED_CH5			(0x10|LED_FLAG)		// DacChannel5	0x00000010 �� 0x80000010
#define		LED_CH6			(0x20|LED_FLAG)		// DacChannel6	0x00000020 �� 0x80000020
#define		LED_CH7			(0x40|LED_FLAG)		// DacChannel7	0x00000040 �� 0x80000040
#define		LED_CH8			(0x80|LED_FLAG)		// DacChannel8	0x00000080 �� 0x80000080

#define		LIGHT_MAX		10				// ���䂷��ő�Ɩ���

typedef struct {
	int channel;				//��d����̃`���l��(0�`7)
	int *data;					//�o�͂���d���l[uA](0�`100,000)
} LED_PARA_ITEM;

typedef struct {
	int port;					//��d�����RDSW�̔ԍ�
	LED_PARA_ITEM item[8];		//��d����̊e�`���l���̐ݒ�
} R_LED_PARA;

//�@��d����o�̓p�����[�^�ݒ�\����
typedef struct {
	char	*name_j;		//	�`�����l�����́i���{��j
	char	*name_e;		//	�`�����l�����́i�p��j
	int		ch;				//	�ύX�����������c�`�b�̃`�����l�����w��i�����̏ꍇOR���邱�Ɓj
							//	�l		�萔			�Ӗ��i��d�����DAC�`�����l���j
							//	����������������������������������������������������������������������
							//	0x01	LED_CH1			0
							//	0x02	LED_CH2			1
							//	0x04	LED_CH3			2
							//	0x08	LED_CH4			3
							//	0x10	LED_CH5			4
							//	0x20	LED_CH6			5
							//	0x40	LED_CH7			6
							//	0x80	LED_CH8			7
	int		*current;		//	�`�����l���̏o�͓d���l
	int		min_current;	//	�d���ŏ��l
	int		max_current;	//	�d���ő�l
	int		step;			//	�d���l�ύX���̍ŏ��X�e�b�v��
}R_LED_ADJUST_ITEM;

//�@��d����o�̓p�����[�^�ݒ�\����
typedef struct {
//	int		board_channel;	//	��d�����DSW�ʒu�Ʊ��ڽ��OR
	char	*title_j;		//	�^�C�g�����́i���{��j
	char	*title_e;		//	�^�C�g�����́i�p��j
	void	(*func)(int mode);
							//	�I�v�V�����@�\���s�֐��i�{�֐��ɂȂ�������s���ꍇ�́A
							//  ���̊֐����Őݒ肷��j
							//	���P�j�摜�捞���ɏƖ����Ƀp�����[�^��ύX�������ꍇ�A
							//	������mode�Ő؂�ւ�����悤�ɗ\�ߊ֐����쐬����B
							//	�A���Amode�@�́AR_LED_ADJUST_ITEM�@light[]�@�ɑΉ������邱�ƁB
							//	�imode=0�́Alight[0]�ł̉摜�捞���̏����j
							//	�Ⴆ�΁A�����_���V���^�[���Ԃ��Ɩ����ɐݒ肷��ꍇ�́A
							//		void func(int mode)
							//		{
							//			switch( mode ){
							//				case 0: R_video_set_shutter_speed(4000);	break;
							//				case 1: R_video_set_shutter_speed(10000);	break;
							//		}
							//			R_change_camera_mode( STANDBY|XCHR50);
							//			R_get_video( SHUTTER|XCHR50, GRAY_PLAHE);
							//		}
	void	(*end_func)(int mode);
							//	�I�v�V�����@�\�I�����s�֐��i��L��func�֐���LED OFF��ɓ����
							//	�s���ꍇ�́A���̊֐����Őݒ肷��B
							//	��	�I���I����A�k�d�c���n�e�e���Ă���捞�I���҂����s���ꍇ
							//	void func(int mode){
							//		R_get_video_auto(SHUTTER|XCHR50, GRAY_PLAHE);
							//	}
							//	
							//	void end_func(int mode){
							//		for(;;){
							//			if(OK==R_get_video_auto_end_check())	break;
							//		}
							//	}
	R_LED_ADJUST_ITEM	light[LIGHT_MAX];
							//	�Ɩ������p�����[�^�\����
	int		*camera;		//	�J�����\���́i�_�~�[�j
} R_LED_ADJUST;

// --- variable ---

// --- prototype ---
int R_LedOpen(int channel);
int R_LedOn(R_LED_PARA *para);
int R_LedOff(R_LED_PARA *para);
int R_LedAdjust( int channel, R_LED_ADJUST *led);
int R_LedAdjustDemo(void);
int R_LedAdjustDemoFunctionSet(void (*func)(int mode), void (*end_func)(int mode));
int R_LedAdjustDemoMenu(void);
int R_LedAdjustDemoRdswSet(int channel);
int R_LedAdjustDemoRdswGet(void);
int R_LedAdjustDemoChannelDataSet(int num, int *pGroupList, int *pCurrentList);
int R_LedAdjustDemoChannelDataGet(int *num, int *pGroupList, int *pCurrentList);
int R_LedAdjustDemoCurrentLimitMaxSet(int ch, int value);		// LED�����f���d���ő�l�̐ݒ�  2007.10.26  by matsu
int R_LedAdjustDemoCurrentLimitMaxGet(int ch);					// LED�����f���d���ő�l�̎擾  "

int R_LedAdjustSamplingModeSet(int nMode);						// �T���v�����O���[�h�̐ݒ�  2008.01.07  by matsu
int R_LedAdjustSamplingModeGet(void);							// �T���v�����O���[�h�̎擾  "
int R_LedAdjustSamplingAreaSet(R_RECT *area, int num);			// �T���v�����O�̈�̐ݒ�
int R_LedAdjustSamplingAreaGet(R_RECT *area, int *num);			// �T���v�����O�̈�̎擾
int R_LedAdjustSamplingAreaNumberMaxSet(int num);				// �T���v�����O�̈�ő吔�̐ݒ�  2007.04.20  by matsu
int R_LedAdjustSamplingAreaNumberMaxGet(void);					// �T���v�����O�̈�ő吔�̎擾  "
#define R_LedAdjustSingleMode R_LedAdjustSingleModeSet			// �֐����ύX  2007.10.26  by matsu
int R_LedAdjustSingleModeSet(int mode);							// �V���O�����[�h�̐ݒ�  �ǉ�  2007.10.26  by matsu
int R_LedAdjustSingleModeGet(void);								// �V���O�����[�h�̎擾  "
int R_LedAdjustSamplingAverageGet(int *pNum, int *pAvg);		//<1.07b1.4> �T���v�����O�̈敽�ϖ��邳�̎擾

int R_LedAdjustUserButtonSet(struct button_t *pUserBtn);		//<1.07b1.7> 
int R_LedAdjustUserButtonGet(void);								//<1.07b1.7> 

int R_LedAdjustFocusMode(int mode);
int R_LedAdjustInitiatorPageSet(int nPage);
int R_LedAdjustInitiatorPageGet(void);
int R_LedAdjustLoopTimerSet(int time);							// ���[�v�^�C�}�̐ݒ�  2007.10.26  by matsu
int R_LedAdjustLoopTimerGet(void);								// ���[�v�^�C�}�̎擾  "

int R_LedAdjustDrawColorSet(int nColor);						// �Ɩ��������̕\���F�̐ݒ�  2008.04.18
int R_LedAdjustDrawColorGet(void);								// �Ɩ��������̕\���F�̎擾  2008.04.18

int R_LedAdjustGetCurrent(int no);								// �O�����璲�����̓d���l���擾����		2013.04.10		komatsu
void R_LedAdjustUserLight(int mode);							// ���[�U��ݒ�̔����K�����g�p����		2013.04.10		komatsu

#endif

#ifdef __cplusplus
}
#endif
