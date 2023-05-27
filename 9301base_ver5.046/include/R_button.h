/******************************************************************************
	R_BUTTON.H / R_BUTTON.C
	
	�ύX����
	3.42		2004.4.21	�}�N�����̓��ꉻ
	3.50		2009.03.25	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_BUTTON_H_
#define	R_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

//�V�}�N��
//����
#define BUTTON_CLOSE			0x00000000
#define BUTTON_NON_CLOSE		0x00000001
#define BUTTON_CONTINUOUS_PUSH	0x00000002
#define BUTTON_NON_WAIT_PUSH	0x00000004
#define BUTTON_NON_CURSOR		0x00000008
//�߂�l
#define BUTTON_CANCEL			0
#define BUTTON_NON_PUSH			-2
//���}�N��
#define CLOSE_BUTTON		0x00000000
#define NON_CLOSE_BUTTON	0x00000001
#define CONTINUOUS_PUSH		0x00000002
#define NON_WAIT_PUSH		0x00000004
#define NON_CURSOR			0x00000008
//#define NON_OPEN_WINDOW		0x00000010

struct button_t
{
	char *name;
	int x;
	int y;
};

int R_button_set(struct button_t *p);
int R_button_set_and_get(struct button_t *p,int flag);
int R_button_get(int flag);
int R_button_close(void);

int R_SVGA_button_set(struct button_t *p);
int R_SVGA_button_set_and_get(struct button_t *p,int flag);
int R_SVGA_button_get(int flag);
int R_SVGA_button_close(void);

#ifdef __cplusplus
}
#endif

#endif	// R_BUTTON_H_
