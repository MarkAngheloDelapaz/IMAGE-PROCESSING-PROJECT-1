/**************************************************************************

	�J�[�\���n�v���O���� ���C�u����

	�v�`�s�b�n�l�@�b�^�R�W�U�@�{�@�c�n�r�S�f

	<R_dcurso.h>
	�ύX����
		Ver x.xx	2004. 3. 4	�x�[�X�쐬	by saito

**************************************************************************/

#ifndef	R_DCURSO_H_
#define	R_DCURSO_H_

// R_dcurso.c �v���g�^�C�v�錾
int		R_draw_cursor_open(void);
void	R_draw_move_cursor(int x, int y);
void	R_draw_draw_cursor(int x, int y);
void	R_draw_erase_cursor(void);
int		R_draw_get_cursor(void);
int		R_draw_get_position(void);
void	R_draw_cursor_on(void);
void	R_draw_cursor_off(void);
int		R_draw_get_box_area_n( int color, int step, int *x, int *y, int *xs, int *ys, int xmax, int ymax, int xmin, int ymin );
int		R_draw_get_box_area( int color, int *x, int *y, int *xs, int *ys, int xmax, int ymax, int xmin, int ymin );
int		R_draw_get_box_area16( int color, int *x, int *y, int *xs, int *ys, int xmax, int ymax, int xmin, int ymin );
void	R_draw_disp_axis( int xx, int yy, int xo, int yo );
void	R_draw_set_cursor_window( int x1, int y1, int x2, int y2 );
int		R_draw_init_cursor( void );


int		R_draw_get_cursor_x( void );
int		R_draw_get_cursor_y( void );

#endif	// R_DCURSO_H_
