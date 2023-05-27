/******************************************************************************
	R_chart.h
	
	�ύX����
	Ver 4.01	2014.02.09	�V�K�ǉ�
	Ver 4.01	2015.05.22	R_chart_multi_alarm_�`�AR_chart_scatter_plot_�`�֐��ǉ�
	Ver 4.01	2015.05.29	R_gui_display_clock_�`�֐��ǉ�
******************************************************************************/

#ifndef R_CHART_H_
#define R_CHART_H_

#include "R_rect.h"
#include "R_alarm.h"
#include "R_stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	const int*	x;
	const int*	y;
	int	num;
} R_CHART;

/**
*	@brief	�q�X�g�O�����\����
*/
typedef struct {
	const int*	data;		/*!< �q�X�g�O�����f�[�^�z�� */
	int	num;				/*!< �q�X�g�O�����f�[�^���i�f�[�^�z��̃T�C�Y�j */
    double	start;			/*!< �w���̊J�n�l�idata[0]�̂w���l�j */
	double	pitch;			/*!< �w���̃s�b�`�idata[n]��data[n+1]�̂w���l�̍��j */
	double	upper_limit;	/*!< ���E����l�i�O���t�ɐԐ��ŕ\�������A�w���l�Ŏw��j*/
	double	lower_limit;	/*!< ���E�����l�i�O���t�ɐԐ��ŕ\�������A�w���l�Ŏw��j*/
	double	upper_control;	/*!< �Ǘ�����l�i�O���t�ɉ����ŕ\�������A�w���l�Ŏw��j*/
	double	lower_control;	/*!< �Ǘ������l�i�O���t�ɉ����ŕ\�������A�w���l�Ŏw��j*/
	double	p_tile;			/*!< ���g�p */
} R_HISTOGRAM;

int R_chart_plot(const R_CHART* data);
int R_chart_plot_histogram(const R_HISTOGRAM* data);

/**
*	@brief	�q�X�g�O�����\���\����
*/
typedef struct {
	void*	p;				/*!< ���[�U�͎g�p���Ȃ��ł������� */
} R_HISTOGRAM_CHART;

/**
*	@brief	�q�X�g�O�����E�B���h�E�̃I�[�v��
*
*	@param	const char*	title	�^�C�g��
*			R_RECT*	rect		�E�B���h�E�̕\���̈�iX Window��ʏ�̍��W�w��j
*
*	@return	�I�u�W�F�N�g�̃|�C���^
*			NULL		�G���[����
*			NULL�ȊO	����
*/
R_HISTOGRAM_CHART* R_histogram_chart_open(const char* title, R_RECT* rect);

/**
*	@brief	�q�X�g�O�����E�B���h�E�̃N���[�Y
*
*	@param	R_HISTOGRAM_CHART*	�I�u�W�F�N�g�̃|�C���^
*
*	@return
*			OK			����
*			OK�ȊO		���s
*/
int	R_histogram_chart_close(R_HISTOGRAM_CHART*);

/**
*	@brief	�q�X�g�O�����f�[�^�̕`��
*
*	@param	R_HISTOGRAM_CHART*	�I�u�W�F�N�g�̃|�C���^
			const R_HISTOGRAM*	data	�q�X�g�O�����f�[�^
*
*	@return	�I�u�W�F�N�g�̃|�C���^
*			NULL		�G���[����
*			NULL�ȊO	����
*/
int	R_histogram_chart_draw(R_HISTOGRAM_CHART* p, const R_HISTOGRAM* data);

/**
*	@brief	�q�X�g�O�����f�[�^�̕`��ƃE�B���h�E�^�C�g���ύX
*
*	@param	R_HISTOGRAM_CHART*	�I�u�W�F�N�g�̃|�C���^
*			const R_HISTOGRAM*	data	�q�X�g�O�����f�[�^
*			const char*	title	�^�C�g��
*
*	@return	�I�u�W�F�N�g�̃|�C���^
*			NULL		�G���[����
*			NULL�ȊO	����
*/
int	R_histogram_chart_draw_with_title(R_HISTOGRAM_CHART* p, const R_HISTOGRAM* data, const char* title);




/**
*	@brief	�����A���[���J�E���^�\���E�B���h�E�\����
*
*			�����A���[���J�E���^�\���E�B���h�E�Ŏg�p���܂��B
*/
typedef struct {
	void*	p;		//!< �E�B���h�E�I�u�W�F�N�g�̃|�C���^
} R_MULTI_ALARM_CHART;

/**
*	@brief	�����A���[���J�E���^�\���E�B���h�E�̃I�[�v��
*
*			�E�B���h�E�I�u�W�F�N�g���쐬���A�����A���[���J�E���^�\����\�����܂��B
*
*	@param	const char*		title	�E�B���h�E�̕\���^�C�g��
*			R_RECT*			rect	�E�B���h�E�̕\���̈�iX Window��ʏ�̍��W�w��j
*
*	@return		�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*		@retval		NULL�ȊO	����
*		@retval		NULL		�G���[����
*/
R_MULTI_ALARM_CHART* R_chart_multi_alarm_open(const char* title, R_RECT* rect);

/**
*	@brief	�����A���[���J�E���^�\���E�B���h�E�̃N���[�Y
*
*			�����A���[���J�E���^�\���E�B���h�E����A�E�B���h�E�I�u�W�F�N�g��������܂��B
*
*	@param	R_MULTI_ALARM_CHART*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*
*	@return
*		@retval		OK			����
*		@retval		OK�ȊO		�G���[����
*/
int R_chart_multi_alarm_close(R_MULTI_ALARM_CHART* chart);

/**
*	@brief	�����A���[���J�E���^�\���E�B���h�E�̃f�[�^�ύX
*
*			�\������A���[���f�[�^��ύX���܂��B
*			�ύX�O�̃f�[�^�ւ̃|�C���^��z��T�C�Y�̕ύX���Ɏg�p���܂��B
*
*	@param	R_MULTI_ALARM_CHART*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			R_ALARM**		alarm	�A���[���\���̃|�C���^�̔z��ւ̃|�C���^�iR_ALARM* alarm[x][y]�j
*			int				x		�z��̂�
*			int				y		�z��̂�
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int R_chart_multi_alarm_set_alarm_struct(R_MULTI_ALARM_CHART* chart, R_ALARM** alarm, int x, int y);

/**
*	@brief	�����A���[���J�E���^�\���E�B���h�E�̍X�V
*
*	@param	R_MULTI_ALARM_CHART*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int R_chart_multi_alarm_update(R_MULTI_ALARM_CHART* chart);

/**
*	@brief	�����A���[���J�E���^�\���Z���̃_�u���N���b�N���̃R�[���o�b�N�֐��̓o�^
*
*	@param	R_MULTI_ALARM_CHART*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			int (*func)(int x, int y, int index)	�R�[���o�b�N�֐��̃|�C���^
*													x: �_�u���N���b�N���ꂽ�Z����X���W
*													y: �_�u���N���b�N���ꂽ�Z����Y���W
*													index: �_�u���N���b�N���ɕ\������Ă���A���[����ނ̃C���f�b�N�X
*													�Ώۂ̃f�[�^�ւ�alarm[x][y].item[index]�ŃA�N�Z�X�ł���
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_multi_alarm_set_cell_double_clicked_callback_func(R_MULTI_ALARM_CHART* chart, int (*func)(int,int,int));

/**
*	@brief	�����A���[���J�E���^�\���Z����ԐF�\������臒l
*
*	@param	R_MULTI_ALARM_CHART*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			int						limit	臒l�B�A���[���J�E���^�����̒l�ȏ�ł���Ε\���Z����ԐF�\������B
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_multi_alarm_set_upper_limit(R_MULTI_ALARM_CHART* chart, int limit);

/**
*	@brief	�����A���[���J�E���^�\�̂w�����\�����t�]����
*
*	@param	R_MULTI_ALARM_CHART*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			int						on_off	OFF:	�w�����\���́A1,2,3,...(X-2),(X-1)
*											ON:		�w�����\���́A(X-1),(X-2),...3,2,1
*											�E�B���h�E�I�[�v�����ɂ�OFF�ɂȂ��Ă���B
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_multi_alarm_set_column_reversed(R_MULTI_ALARM_CHART* chart, int on_off);	// on_off: ON/OFF

/**
*	@brief	�����A���[���J�E���^�\�̂w�����w�b�_�̕\���������ݒ肷��
*
*	@param	R_MULTI_ALARM_CHART*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			int						column	�w�����\���ʒu
*			const char*				label	�\��������
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_multi_alarm_set_column_header_label(R_MULTI_ALARM_CHART* obj, int column, const char* label);

/**
*	@brief	�����A���[���J�E���^�\�̂x�����w�b�_�̕\���������ݒ肷��
*
*	@param	R_MULTI_ALARM_CHART*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			int						row		�x�����\���ʒu
*			const char*				label	�\��������
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_multi_alarm_set_row_header_label(R_MULTI_ALARM_CHART* obj, int row, const char* label);




/**
*	@brief	�U�z�}�\���E�B���h�E�\����
*
*			�U�z�}�\���E�B���h�E�Ŏg�p���܂��B
*/
typedef struct {
	void*	p;		//!< �E�B���h�E�I�u�W�F�N�g�̃|�C���^
} R_CHART_SCATTER_PLOT;

/**
*	@brief	�U�z�}�\���E�B���h�E�̃I�[�v��
*
*	@param	const char*		title	�E�B���h�E�̕\���^�C�g��
*			R_RECT*			rect	�E�B���h�E�̕\���̈�iX Window��ʏ�̍��W�w��j
*
*	@return
*			R_CHART_SCATTER_PLOT*	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*									NULL�̏ꍇ�G���[����
*/
R_CHART_SCATTER_PLOT*	R_chart_scatter_plot_open(const char* title, R_RECT* rect);

/**
*	@brief	�U�z�}�\���E�B���h�E�̕\���f�[�^�ݒ�
*
*	@param	R_CHART_SCATTER_PLOT*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			R_POINT*				point	�f�[�^�z��ipoint[num]�j
*			int						num		�f�[�^��
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_scatter_plot_set_points(R_CHART_SCATTER_PLOT* chart, R_POINT* point, int num);

/**
*	@brief	�U�z�}�\���E�B���h�E�̂w�X�P�[���ݒ�
*
*	@param	R_CHART_SCATTER_PLOT*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			int						min		�ŏ��l
*			int						max		�ő�l
*			int						step	�ڐ���Ԋu
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_scatter_plot_set_scale_x(R_CHART_SCATTER_PLOT* chart, int min, int max, int step);

/**
*	@brief	�U�z�}�\���E�B���h�E�̂x�X�P�[���ݒ�
*
*	@param	R_CHART_SCATTER_PLOT*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*			int						min		�ŏ��l
*			int						max		�ő�l
*			int						step	�ڐ���Ԋu
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_scatter_plot_set_scale_y(R_CHART_SCATTER_PLOT* chart, int min, int max, int step);

/**
*	@brief	�U�z�}�\���E�B���h�E�̍X�V
*
*	@param	R_CHART_SCATTER_PLOT*	chart	�E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^
*
*	@return
*			OK			����
*			OK�ȊO		�G���[����
*/
int	R_chart_scatter_plot_update(R_CHART_SCATTER_PLOT* chart);




/**
*	@brief	���ԕ\���E�B���h�E�\����
*
*			���ԕ\���E�B���h�E�Ŏg�p���܂��B
*/
typedef struct {
	void*	p;		//!< �E�B���h�E�I�u�W�F�N�g�̃|�C���^
} R_GUI_DISPLAY_CLOCK;

R_GUI_DISPLAY_CLOCK* R_gui_display_clock_open(const char* title, R_RECT* rect);
int R_gui_display_clock_start(R_GUI_DISPLAY_CLOCK* gui, R_RECT* rect);
int R_gui_display_clock_stop(R_GUI_DISPLAY_CLOCK* gui);




#ifdef __cplusplus
}
#endif

#endif // R_CHART_H
