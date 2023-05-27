/******************************************************************************
	R_qr_enc.cpp / R_qr_enc.h
	
	�ύX����
	Ver 4.01	2017/10/13	�V�K�쐬
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief QR�G���R�[�h�\���̂̃|�C���^
 *	���[�U�\�[�X����͍\���̂̃����o�ɃA�N�Z�X���Ȃ��ł��������B
 */
typedef struct {
	int	init;
	
	int	casesensitive;
	int	eightbit;
	int	version;
	int	margin;
	int	micro;
	
	int	level;
	int	color0;
	int	color1;
} R_QR_ENCODE;

/*!
 * @brief QR�R�[�h�쐬�p�����[�^�������֐�
 *	�f�t�H���g�l�ŏ��������ꂽQR�G���R�[�h�\���̂�Ԃ��܂��B
 * @retval	���������ꂽQR�G���R�[�h�\����
 */
R_QR_ENCODE	R_qr_encode_struct(void);

/*!
 * @brief QR�R�[�h�\���֐�
 *	�O���[�v���[����QR�R�[�h��\�����܂��B
 * @param	qr	QR�G���R�[�h�\���̂̃|�C���^
 * @param	page	�\����O���[�������̃y�[�W�ԍ�
 * @param	x		�\���ʒu�AQR�R�[�h�̍����W
 * @param	y		�\���ʒu�AQR�R�[�h�̏���W
 * @param	size	�P�Z��������̃s�N�Z����
 * @param	string	�G���R�[�h���镶����
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_string_on_gray_plane(const R_QR_ENCODE* qr, int page, int x, int y, int size, const char* string);

int	R_qr_encode_string_on_binary_plane(const R_QR_ENCODE* qr, int page, int x, int y, int size, const char* string);

int	R_qr_encode_string_on_draw_plane(const R_QR_ENCODE* qr, int x, int y, int size, const char* string);

/*!
 * @brief �}�[�W���i�]���j�̐ݒ�
 *	QR�R�[�h�̂܂��̗]����ݒ肵�܂��B
 *	�f�t�H���g��4�ł����A�ύX���������Ɏg�p���Ă��������B
 *	QR�R�[�h�K�i�̂ł�4�ȏ�K�v�ł��B
 * @param	qr	QR�G���R�[�h�\���̂̃|�C���^
 * @param	cells	�}�[�W���̃Z����
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_set_margin(R_QR_ENCODE* qr, int cells);

/*!
 * @brief QR�R�[�h�\���F�̐ݒ�
 *	�f�t�H���g�́Acolor0=0�Acolor1=0x00FFFFFF�ł��B
 *	�ύX���������Ɏg�p���Ă��������B
 * @param	qr	QR�G���R�[�h�\���̂̃|�C���^
 * @param	color0	�Z���̕\���F�i�ʏ�͍��F�j
 * @param	color1	�w�i�̕\���F�i�ʏ�͔��F�j
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_set_color(R_QR_ENCODE* qr, int color0, int color1);

/*!
 * @brief ���������x���̐ݒ�
 *	QR�R�[�h�̌��������x���iL,M,Q,H�j��ݒ肵�܂��B
 *	�f�t�H���g��0�ł����A�ύX���������Ɏg�p���Ă��������B
 * @param	qr	QR�G���R�[�h�\���̂̃|�C���^
 * @param	level	0:L�A1:M�A2:Q�A3:H
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_set_error_correction_level(R_QR_ENCODE* qr, int level);

/*!
 * @brief �o�[�W�����̐ݒ�
 *	QR�R�[�h�̃o�[�W������ݒ肵�܂��B
 *	version=1�ŃZ������21x21�Aversion=40�ŃZ������177x177�ɂȂ�܂��B
 *	version=0�ɐݒ肵���ꍇ�́A�������ɂ���Ď����I�Ƀo�[�W���������肵�܂��B
 *	�f�t�H���g��0�ł����A�ύX���������Ɏg�p���Ă��������B
 * @param	qr	QR�G���R�[�h�\���̂̃|�C���^
 * @param	version	0:auto�A1�`40
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_set_symbol_version(R_QR_ENCODE* qr, int version);

/*!
 * @brief �Z�����̎擾
 *	QR�R�[�h�̕\���Z�������擾���܂��B
 *	�}�[�W���͊܂݂܂���B
 * @param	qr	QR�G���R�[�h�\���̂̃|�C���^
 * @param	string	�G���R�[�h���镶����
 * @retval	�Z����
 * @retval	0	�G���[
 */
int	R_qr_encode_cells(R_QR_ENCODE* qr, const char* string);

#ifdef __cplusplus
}
#endif
