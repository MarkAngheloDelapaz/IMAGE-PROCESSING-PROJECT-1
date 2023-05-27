/******************************************************************************
	R_WINB.H / R_WINB.C
	
	�ύX����
		Ver 3.39	2003/06/06	���������[�X
		Ver 3.41	2004/01/10	R_winb_parameter
								R_winb_parameter_with_mess�ǉ�
		Ver 3.42	2004/02/26	R_swinb�n�Ή�
					2004/04/22	R_draw_winb�n�Ή�
		Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
		Ver.4.01	2012/07/25	R_winb_callback�֐��ǉ�
******************************************************************************/

#ifndef	R_WINB_H_
#define	R_WINB_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WINB_NON_CLOSE	0
#define WINB_CLOSE	1

#define WINB_WAIT	0
#define WINB_NOWAIT	0x10

struct winb_title_t {
	char	*j;							/*�@�^�C�g��(���{��)*/
	char	*e;							/*�@�^�C�g��(�p��)	*/
};
struct winb_item_t {
	char	*j;							/*�@����(���{��)				*/
	char	*e;							/*�@����(�p��)					*/
	int		(*func)(int);				/*�@�ďo�֐�					*/
	struct	winb_t	*winb;				/*�@�E�C���h�E���j���[�\����	*/
};
struct winb_t {
	struct	winb_title_t winb_title;	/*�@�^�C�g��		*/
	int		xlen;						/*�@������������	*/
	int		ylen;						/*�@������������	*/
	int		n;							/*�@�I�����ڐ�		*/
	struct	winb_item_t winb_item[];	/*�@������			*/
};
struct winb_para_t {
	char	*j;							/*�@�p�����[�^������(���{��)	*/
	char	*e;							/*�@�p�����[�^������(�p��j		*/
	int		*para_value;				/*�@�p�����[�^					*/
	int		para_vmax;					/*�@����l						*/
	int		para_vmin;					/*�@�����l						*/
	int		para_step;					/*�@�ύX�X�e�b�v				*/
};
//�@�p�����[�^�ݒ�\����(���b�Z�[�W�t���j	
struct winb_para_menu_t {					
	char	*j;							/*�@���j���[������(���{��)		*/
	char	*e;							/*�@���j���[������(�p��j		*/
};
struct winb_para_tm {						
	char	*j;									/*�@�p�����[�^������(���{��)	*/
	char	*e;									/*�@�p�����[�^������(�p��j		*/
	int		*para_value;						/*�@�p�����[�^					*/
	int		para_vmax;							/*�@����l						*/
	int		para_vmin;							/*�@�����l						*/
	int		para_step;							/*�@�ύX�X�e�b�v				*/
	struct	winb_para_menu_t *winb_para_menu;	/*�@���j���[������				*/
};

//#if R_WINB | R_SWINB | RWINB
#if defined(R_WINB) || defined(R_SWINB) || defined(RWINB)	// 2009/07/28
static struct {
	struct winb_t *winb;
	int		cx;
	int		cy;
	int		top;
	int		xs;
	int		ys;
} Menu_bilingual[ WIN_NEST_MAX ];
#endif

/* �R�����g�A�E�g 2009/07/28
//�@�E�B���h�E���b�Z�C�W�p
struct winb_mes_t {
	int		xlen;
	int		ylen;
	char	*str[];
};
//�@�t�@���N�V�����ݒ�p
#ifdef	R_WINB
struct winb_mes_t winb_mes = { 14, 1, "function" };
#endif
//#if R_SWINB | RWINB
#if defined(R_SWINB) || defined(RWINB)	// 2009/07/28
extern struct winb_mes_t winb_mes;
#endif
*/

#define	WINB_STEP_ATTR_MASK	0xFF000000
#define	WINB_STEP_ATTR_DEC	0x00000000
#define	WINB_STEP_ATTR_HEX	0x80000000

//�@�p�����[�^�ύX�֐�
int	R_winb_parameter( struct winb_para_t [], int, char *, char * );
int	R_winb_parameter_with_mess( struct winb_para_tm [], char *, char * );

/**
*	@brief	�|�b�v�A�b�v������������
*	@param	int	x					�ŏ��̃E�B���h�E���゘���W
*			int	y					�ŏ��̃E�B���h�E���゙���W
*			int	disp_mode			�E�B���h�E����
*				WINB_NON_CLOSE	�V�����E�B���h�E���J���Ă��O�̃E�B���h�E�͎c��
*				WINB_CLOSE   	�V�����E�B���h�E���J������O�̃E�B���h�E�͏���
*				WINB_WAIT		�L�����Z���ŏI������Ƃ��}�E�XSW�������̂�҂��Ċ֐��𔲂���
*				WINB_NOWAIT		�`�F�b�N���Ȃ�
*			struct winb_t*	winb	�E�C���h�E���j���[�\����
*	@return	int
*			0�`		�ŏ��̃E�B���h�E�ł̑I��ԍ��i�ďo�֐��ƃ��j���[�\���̂�����NULL�̍��ڂ�I�������ꍇ�j
*			-2		�ŏ��̃E�B���h�E�ŃL�����Z��
*			ERROR3	�֐��̖߂�l��ERROR�ŏI�����͂Q�Ԗڈȍ~�̃E�B���h�E�ŏI��
*/
int R_winb_menu(int x, int y, int disp_mode, struct winb_t* winb);

// 2012.07.25
void	R_winb_callback(int (*func)(int,int));

#ifdef __cplusplus
}
#endif

#endif	// R_WINB_H_
