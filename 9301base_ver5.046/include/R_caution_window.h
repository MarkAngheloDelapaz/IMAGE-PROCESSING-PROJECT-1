/******************************************************************************
	R_caution_window.h
	
	�ύX����
	Ver 4.01	2015.10.14	�V�K�ǉ�
******************************************************************************/

/*!
	@file	R_caution_window.h
	
	@brief	�x���E�B���h�E�C���^�t�F�[�X���C�u����
	
	X Window System�ł̂ݎg�p�\�ł��B@n
	�����\�t�g�E�F�A���x����Ԃł��邱�Ƃ�@n
	X Window System��ʏ�ɍő�T�C�Y�̃E�B���h�E�ŕ\�����܂��B@n
	���̌x���E�B���h�E�͓��ߕ\�������̂ŁA@n
	�����\�t�g�̉�ʂ��m�F���邱�Ƃ��\�ł��B@n
*/

#ifndef R_CAUTION_WINDOW_H
#define R_CAUTION_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
	@brief	�x���E�B���h�E�E�C���^�t�F�[�X�̃I�[�v��
	
	�x���E�B���h�E�Ƃ̃C���^�t�F�[�X���I�[�v�����܂��B@n
	�x���E�B���h�E�\�t�g"CautionWindow"���N�����Ă����Ă��������B@n
	���̊֐��Ōx���E�B���h�E�\�t�g���N�����Ă��邩���m�F���܂��B@n
	�x���E�B���h�E�ɂ͕����̃v���Z�X�̌x�����������ɕ\������܂��̂ŁA@n
	�v���Z�X�̎��ʂ��ł���悤�ȃ^�C�g�����w�肵�Ă��������B@n
	�x���E�B���h�E�ɂ́A���L�̗l�ɕ\������܂��B@n
	�@[ �^�C�g�������� ]@n
	�@�@�x��������
	
	@param	title	�^�C�g��������
	
	@return	���s����@n
			OK�@�@�@�@����I��@n
			OK�ȊO�@�@�ُ�I��
*/
int R_caution_window_open(const char* title);

/*!
	@brief	�x��������̐ݒ�
	
	�x���E�B���h�E�ɕ\������x���������ݒ肵�܂��B
	
	@param	string	�x��������
	
	@return	���s����@n
			OK�@�@�@�@����I��@n
			OK�ȊO�@�@�ُ�I��
*/
int R_caution_window_set_string(const char* string);

/*!
	@brief	�x���E�B���h�E��Ԃ̐ݒ�
	
	@param	on_off	�x�����@n
			ON  �@�@�@�x����ԗL��@n
			OFF �@�@�@�x����Ԗ���
	
	@return	���s����@n
			OK�@�@�@�@����I��@n
			OK�ȊO�@�@�ُ�I��
*/
int R_caution_window_activate(int on_off);

/*!
	@brief	�x���E�B���h�E��Ԃ̎擾
	
	�x���E�B���h�E�v���Z�X������ɓ��삵�Ă��邩���m�F���܂��B@n
	�x���E�B���h�E�v���Z�X�����炩�̌����ňُ�I�����Ă��܂����ꍇ�A@n
	�x���E�B���h�E���\������Ȃ��Ȃ��Ă��܂��܂��B@n
	���̊֐��ŃE�B���h�E�v���Z�X���ێ�����Ă��邩�����I�Ɋm�F���Ă��������B@n
	�x����Ԃ�L���ɂ��Ă����ԂŌx���E�B���h�E�v���Z�X���I�������ꍇ�A@n
	�x���E�B���h�E���ċN������悤�Ƀ��b�Z�[�W��\�����܂��B
	
	@return	�x���E�B���h�E���@n
			0�ȊO�@�@�x����ԗL��@n
			0�@�@�@�@�x����Ԗ���
*/
int	R_caution_window_is_active(void);

/**
*	@brief	�x���E�B���h�E�E�C���^�t�F�[�X�̃N���[�Y
*
*	@return	���s����@n
*			OK�@�@�@�@����I��@n
*			OK�ȊO�@�@�ُ�I��
*/
int R_caution_window_close(void);

#ifdef __cplusplus
}
#endif

#endif // R_CAUTION_WINDOW_H
