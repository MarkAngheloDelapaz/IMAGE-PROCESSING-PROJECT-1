/******************************************************************************

	R_cs_cod.h
	
	�R�[�h�D�揇�ʏ����w�b�_

									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	<version>	<date>		<name>		<comment>
	1.07b3.1	2011.05.18	matsu		�V�K�쐬

******************************************************************************/

#ifndef __R_CS_CODE_H__
#define __R_CS_CODE_H__


#include "R_cs_cnt.h"

typedef struct {
	int  nCode;							// NG�R�[�h
	char *sMess;						// �t�у��b�Z�[�W
} R_CS_CODE_PRIORITY_DATA;

typedef struct {
	R_CS_CODE_PRIORITY_DATA *pData;		// append���ꂽ�f�[�^���X�g
	int *pPriority;						// �D�揇�ʃ��X�g
	int *pStatus;						// �e�R�[�h�̏��
	int nCodeMax;						// NG�R�[�h�ő吔
	int nMessMax;						// �t�у��b�Z�[�W�ő啶����
	int nDataMax;						// NG�R�[�h�o�^�ő吔
	int nMode;							// ���[�h
	int nDataPos;						// �f�[�^�ێ��|�C���^
} R_CS_CODE_PRIORITY;


int R_CS_CodePriorityOpen(R_CS_CODE_PRIORITY *pPriority, int nCodeMax, int nMessMax, int nDataMax);		// �R�[�h�D�揇�ʏ����I�[�v��
int R_CS_CodePriorityClose(R_CS_CODE_PRIORITY *pPriority);												// �R�[�h�D�揇�ʏ����N���[�Y
int R_CS_CodePriorityClear(R_CS_CODE_PRIORITY *pPriority);												// �R�[�h�D�揇�ʏ����f�[�^�N���A
int R_CS_CodePriorityLoad(R_CS_CODE_PRIORITY *pPriority, char *pFileName, int nId, int nMode);			// �R�[�h�D�揇�ʏ����f�[�^�ǂݍ���
int R_CS_CodePrioritySave(R_CS_CODE_PRIORITY *pPriority, char *pFileName, int nId, int nMode);			// �R�[�h�D�揇�ʏ����f�[�^�ۑ�
int R_CS_CodePrioritySetOrder(R_CS_CODE_PRIORITY *pPriority, int nCode, int nOrder);					// �R�[�h�D�揇�ʏ������ʐݒ�
int R_CS_CodePriorityGetOrder(R_CS_CODE_PRIORITY *pPriority, int nCode);								// �R�[�h�D�揇�ʏ������ʎ擾
int R_CS_CodePrioritySetMode(R_CS_CODE_PRIORITY *pPriority, int nMode);									// �R�[�h�D�揇�ʏ������[�h�ݒ�
int R_CS_CodePriorityGetMode(R_CS_CODE_PRIORITY *pPriority);											// �R�[�h�D�揇�ʏ������[�h�擾
int R_CS_CodePriorityAppend(R_CS_CODE_PRIORITY *pPriority, int nCode, char *sMess);						// �R�[�h�D�揇�ʏ����m�f�R�[�h�ǉ�
int R_CS_CodePriorityCheck(R_CS_CODE_PRIORITY *pPriority, int *pResultCode, char *pResultMess);			// �R�[�h�D�揇�ʏ������ʔ���
int R_CS_CodePriorityEdit(R_CS_CODE_PRIORITY* pPriority, R_CS_COUNTER_NG_CODE* pCode);					// �R�[�h�D�揇�ʏ������ʐݒ�

#endif
