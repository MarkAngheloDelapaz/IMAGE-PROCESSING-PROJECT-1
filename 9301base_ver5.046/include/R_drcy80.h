/******************************************************************************
	R_DRCY80.H / R_DRCY80.C
	
	�ύX����
	Ver 3.35	03/04/7		���Ń����[�X
	Ver 3.36	03/05/13	�����Ń����[�X
	Ver 3.49	07/02/22	DR-CY-105�Ή�	by tanaka
	Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
	Ver 5.030	2020/06/22	PCI������Ώۂ̃o�X�ԍ��ő�l���C��
	Ver 5.042	2021/03/02	�o�͓d��0�ݒ�̎��A�t�o�C�A�X��������
	Ver 5.042	2021/03/02	�����֐�r_drcy80_disable_bias_shifting�ǉ�
******************************************************************************/

#ifndef	R_DRCY80_H_
#define	R_DRCY80_H_

int		r_drcy80_dac_open(int nID);
void	r_drcy80_dac_close(int nHandle);
int		R_drcy80_dac_set(int nHandle, int nChannel, int nData);

int r_drcy80_get_apic21_port(int nHandle);
int r_drcy80_get_register_port(int nHandle);
int r_drcy80_set_ref(int nHandle, int nChannel, int nData);
int r_drcy80_dac_current_initial(uintptr_t nHandle);
int r_drcy80_set_current(uintptr_t nHandle,int port,int data);
int r_drcy80_get_revision(int nHandle);
int	r_drcy80_disable_bias_shifting(uintptr_t handle);

int		r_drcy105_dac_open(int nID);
int		R_drcy105_dac_set(int nHandle, int nChannel, int nData);

int r_drcy105_get_apic21_port(int nHandle);
int r_drcy105_get_register_port(int nHandle);
int r_drcy105_set_ref(int nHandle, int nChannel, int nData);
int r_drcy105_dac_current_initial(int nHandle);
int r_drcy105_set_current(int nHandle,int port,int data);

#define R_drcy105_dac_close			  r_drcy80_dac_close
#define r_drcy105_get_revision		  r_drcy80_get_revision

#endif	// R_DRCY80_H_
