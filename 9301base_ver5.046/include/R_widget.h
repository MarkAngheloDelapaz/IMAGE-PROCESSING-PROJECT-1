/******************************************************************************
	R_widget.h / R_widget.c

	�ύX����
	Ver 4.01	2014.02.24	�V�K�ǉ�								by araki
******************************************************************************/

#ifndef R_WIDGET_H
#define R_WIDGET_H

#include "R_rect.h"
#include "R_plane.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void*	p;
} R_IMAGE_VIEW;

/**
*	@brief	�摜�\���E�B���h�E�̃I�[�v��
*
*	@param	const char*	title	�^�C�g��
*			R_RECT*	rect		�\���̈�iX Window��ʏ�̍��W�w��j
*
*	@return	�I�u�W�F�N�g�̃|�C���^
*			NULL		�G���[����
*			NULL�ȊO	����
*/
R_IMAGE_VIEW*	R_widget_image_view_open(const char* title, R_RECT* rect);

int R_widget_image_view_close(R_IMAGE_VIEW* p);

/**
*	@brief	�摜�̐ݒ�
*
*	@param	R_IMAGE_VIEW*	�I�u�W�F�N�g�̃|�C���^
*			const R_PLANE*	plane	�\���摜
*			const char*	comment	�\������R�����g
*			int	code	�\�����f�p�R�[�h
*
*	@return	����
*			OK		����
*			OK�ȊO	���s
*/
int	R_widget_image_view_append(R_IMAGE_VIEW* p, const R_PLANE* plane, const char* comment, int code);

int	R_widget_image_view_clear(R_IMAGE_VIEW* p);




/**
*	@brief	�摜�\���̐ݒ�
*
*	@param	R_IMAGE_VIEW*	�I�u�W�F�N�g�̃|�C���^
*			int	accept_code	�\�����R�[�h
*
*	@return	����
*			OK		����
*			OK�ȊO	���s
*/
/*
	�\������R�[�h��R_widget_image_view_append_accept_code�Œǉ����Ă����܂��B
	R_widget_image_view_clear_accept_code�����s����ƁA���ׂẴR�[�h��\�����܂��B
*/
int	R_widget_image_view_append_accept_code(R_IMAGE_VIEW* p, int accept_code);
int	R_widget_image_view_remove_accept_code(R_IMAGE_VIEW* p, int accept_code);
int R_widget_image_view_clear_accept_code(R_IMAGE_VIEW* p);

#ifdef __cplusplus
}
#endif

#endif // R_WIDGET_H
