/******************************************************************************
	R_DRCY83.H / R_DRCY83.C
	
	�ύX����
		Ver 3.31	2002/07/31	���������[�X
		Ver.3.32	2002/11/08	�w�b�_���������m�ɕύX
		Ver.3.33	2003/1/16	�����֐��v���g�^�C�v���L��
		Ver 3.34	2003/03/10	r_drcy83_dac_open�����֐���
		Ver 3.35	2003/04/7	r_drcy83_set_current�ǉ�
		Ver 3.39	2003/06/24	r_drcy83_get_revision�ǉ�
		Ver 3.42	2004/04/14	DR-CY-95�Ή�
		Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
		Ver 4.01	2010/04/22	DR-CY-83�}���`�v���Z�X�b��Ή�
		Ver 4.01	2013/02/08	DR-CY-95�}���`�v���Z�X�Ή�
******************************************************************************/

#ifndef	R_DRCY83_H_
#define	R_DRCY83_H_

int		r_drcy83_dac_open(int nID);
void	R_drcy83_dac_close(int nHandle);
int		R_drcy83_dac_set(int nHandle, int nChannel, int nData);

int r_drcy83_get_apic21_port(int nHandle);
int r_drcy83_get_register_port(int nHandle);
int r_drcy83_set_ref(int nHandle, int nChannel, int nData);
int r_drcy83_dac_current_initial(int nHandle);
int r_drcy83_set_current(int nHandle,int port,int data);
int r_drcy83_get_revision(int nHandle);

int		r_drcy95_dac_open(int nID);
int		r_drcy95_get_apic21_port(int nHandle);
int		r_drcy95_get_register_port(int nHandle);

#define R_drcy95_dac_set		R_drcy83_dac_set
#define R_drcy95_dac_close		R_drcy83_dac_close

#endif	// R_DRCY83_H_
