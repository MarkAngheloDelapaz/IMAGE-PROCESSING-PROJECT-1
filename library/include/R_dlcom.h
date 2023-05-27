/******************************************************************************
	R_dlcom.h / R_dlcom.c
	
	�ύX����
	Ver 3.46		2005/06/20	���������[�X
	Ver 3.50beta48	2008/11/17	�쓮�ʐM�֐��ǉ�		by saito
	Ver 3.50beta60	2009/12/18	NAK����0x15�ɕύX		by saito
	Ver 3.50		2009/05/29	r_dlio_reg_port�֐��ǉ�
	Ver 3.50		2009/07/07	�^�C�}�֐���ύX
******************************************************************************/

#ifndef	R_DLCOM_H_
#define	R_DLCOM_H_

#ifdef __cplusplus
extern "C" {
#endif

//�쓮�ʐM�ݒ�			2008.11.17 saito
//�ʐM�G���[�R�[�h
#define DLIO_COM_ERROR_INIT			-1	//�쓮�ʐM�������ُ�
#define DLIO_COM_ERROR_GET_START	-2	//��M��	�f�[�^�Ȃ��iSTB bit off)
#define DLIO_COM_ERROR_PUT_START	-3	//���M��	���葤�������ł��ĂȂ�
#define DLIO_COM_ERROR_TIME_UP		-4	//����M��	�^�C���A�b�v�G���[
#define DLIO_COM_ERROR_SUM			-5	//����M��	�`�F�b�N�T���G���[
#define DLIO_COM_ERROR_RESET		-6	//����M��	���葤���Z�b�g


//�ʐM���[�h
#define DLIO_COM_SLAVE			0		//�ʐM�X���[�u�ݒ�i�X�L���[�����s�j
#define DLIO_COM_MASTER			1		//�ʐM�}�X�^�ݒ�@�i�X�L���[�����\�j

//����r�b�g
#define DLIO_COM_RX_STB			0x01	//�R�}���h��M�J�n�i���́j
#define DLIO_COM_TX_STB			0x01	//�R�}���h���M�J�n�i�o�́j
#define DLIO_COM_TX_ACK			0x02	//�R�}���h���M�����i���́j
#define DLIO_COM_RX_ACK			0x02	//�R�}���h��M�����i�o�́j

//����R�[�h
#define DLIO_COM_CODE_DLM		0x0d	//�f���~�^�R�[�h
#define DLIO_COM_CODE_ACK		0x06	//�`�b�j�R�[�h
#define DLIO_COM_CODE_NAK		0x15	//0xF9	//�m�`�j�R�[�h�i�`�b�j�̑S�r�b�g���]�j

//�X�L���[����
#define DLIO_COM_SKEW_RX_SETUP	0x00	//��MDATA��SETUP����
#define DLIO_COM_SKEW_TX_SETUP	0x01	//���MDATA��SETUP����

//�쓮�ʐM�֐�			2008.11.17 saito
int R_dlio_com_open( int nHandle, int nMode );
int R_dlio_com_close( int nHandle );
int R_dilo_com_reset( int nHandle );
int R_dlio_com_gets( int nHandle, char *pCommand ); 
int R_dlio_com_puts( int nHandle, char *pCommand );
int R_dlio_com_set_skew( int nHandle, int nSkewMode, int nTime );
int R_dlio_com_set_timeup( int nHandle, int nTime );
void R_dlio_com_debug_disp_on ( int nHandle );
void R_dlio_com_debug_disp_off( int nHandle );
void R_dlio_com_debug_time_on ( int nHandle );
void R_dlio_com_debug_time_off( int nHandle );
void R_dlio_com_reset( int nHandle );

//�f�o�b�O������A�����֐���
//void R_dlio_com_set_timeup_start( int nTime );
//void R_dlio_com_set_timeup_char ( int nTime );
//void R_dlio_com_set_timeup_str  ( int nTime );

#ifdef __cplusplus
}
#endif

#endif	// R_DLCOM_H_
