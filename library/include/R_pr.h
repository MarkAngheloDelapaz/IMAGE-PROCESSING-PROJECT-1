/******************************************************************************
	R_pr.h / R_pr.c
	
	�ύX����
	Ver 3.50	2008/03/05	���������[�X
	Ver 3.50	2009/02/10	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_PR_H_
#define	R_PR_H_

//#ifndef __R_STDLIB_H__
//#include "R_STDLIB.h"
//#endif
#include "R_stdlib.h"
//#include "R_matrix.h"

#define R_SVM_TAG_PASS	1.0
#define R_SVM_TAG_FAIL	-1.0
#define R_PR_LIN	0				//���`
#define R_PR_NOLIN	1				//����`
#define R_PR_CLASS_ELEMENT	-1		//�^�O�v�f
#define R_VECTOR_MULTI_MAX	10000
typedef struct{
	int nNumber;
	int nDim;
	R_VECTORD stVector[R_VECTOR_MULTI_MAX];
	R_VECTORD stBufferVector[R_VECTOR_MULTI_MAX];
	double *pY;
}R_PRLEARN_DATA;

typedef struct{
	int		nMode;					//�T�|�[�g�x�N�^�[�}�V���̃��[�h�i0:���` 1:����`�j
	int		nDim;					//SVM�x�N�g��������
	int		nMaxRoop;				//�ő�J�Ԃ���
	int		nNumber;				//�x�N�g���f�[�^�̌�
	int		nCategoryANumber;		//�J�e�S���[A�i1�j�̊w�K�f�[�^��
	int		nCategoryBNumber;		//�J�e�S���[B�i1�j�̊w�K�f�[�^��
	double	dNormalizeScale;			//�T���v���f�[�^�̐��K���X�P�[��
	double *pLmd;					//Lagrangue����搔
	double *pLmdDelta;				//Lagrangue����搔�̒����ω�����
	double *pGaussianKernel;		//�J�[�l��
	double *pW;						//�d�݌W��
	double *pB;						//�o�C�A�X�W��
	double *pVectorAve;				//�T���v���f�[�^�̕��ϒl
	double *pY;						//�f�[�^�̊w�K�^�O
	R_VECTORD	*pX;				//�w�K�x�N�g���f�[�^
	R_VECTORD	*pOrgX;				//�w�K�x�N�g���f�[�^
}R_PRSVM_PARA;
// --- variable ---
#ifdef _R_PR_FILE_
int nLearnErrorCode = -1;
#else
extern int nLearnErrorCode;
#endif


// --- prototype ---
int R_pr_svm_set_data( int dim, int number, int max_roop,R_VECTORD *str_org, R_VECTORD *str_buff,double *tag );			//SVM�w�K�f�[�^�̃Z�b�g
int R_pr_svm_close_data( int nHandle );																					//SVM�w�K�f�[�^�̃N���[�Y
int R_pr_svm_transfer_data( int nHandle );																				//�����x�N�g���̐��K��
int R_pr_svm_count_category_number( int nHandle );																		//�N���X�ɑ����Ă���f�[�^�����v�Z
int R_pr_svm_get_error( void );																							//R_pr_svm_set_data�ł̃G���[�R�[�h
int R_pr_svm_draw_parameter( int nHandle );																				//SVM�p�s��p�����[�^�̕\��
int R_pr_svm_count_category_number( int nHandle );																		//�w�K�^�O�f�[�^�̌v��
int R_pr_svm_draw_support_vector( int nHandle );																		//�T�|�[�g�x�N�^�[�̕\���i�Q�����̂݁j
int R_pr_svm_draw_line( int nHandle );																					//���E�����̕\���i�Q�����A���`SVM�̂݁j
int R_pr_svm_get_parameter( int nHandle );																				//�d�݌W���A�o�C�A�X���̌v�Z
int R_pr_svm_draw_area( int nHandle, unsigned char *sp );																//���E�̈�̕\���i�Q�����A����`SVM�̂݁j
int R_pr_svm_lmd_normalize( int nHandle );																				//Lagrangue����搔�̐��K��
int R_pr_svm_get_delta_lmd( int nHandle );																				//Lagrangue����搔�̑����v�Z
int R_pr_svm_set_type( int nHandle, int mode );																			//SVM���[�h�̑I���i�O�F���`�@�P�F����`�j
int R_pr_svm_lmd_initial( int nHandle );																				//Lagrangue����搔�̏������K��
int R_pr_svm_set_kernel( int nHandle );																					//����`SVM�pMercel�J�[�l���̍쐬
int R_pr_svm_get_support_vector_number( int nHandle );																	//�T�|�[�g�x�N�g���̌����擾
int R_pr_svm_return_parameter( int nHandle );																			//�p�����[�^�̌���Ԃւ̎ʑ�
int R_pr_svm_normalize_parameter( int nHandle );																		//�p�����[�^�̐��K��Ԃւ̎ʑ�
int R_pr_svm_save_data( int nHandle, int mode, char *filename, char *comment  );										//SVM�f�[�^�̃t�@�C����������


int R_pr_learn_data_open( int number, int dimension );																	//�w�K�p�f�[�^�̈�m��
int R_pr_learn_data_close( int nHandle );																				//�w�K�p�f�[�^�̈���
int R_pr_learn_svm_data_charge( int nLearnDataHandle, int nDataNumber, int nMaxRoop );									//�w�K�p�f�[�^��SVM�\���̂Ɋ��蓖�Ă�
int R_pr_learn_svm_data_discharge( int nHandle );																		//SVM�\���̂���̊w�K�p�f�[�^�̉��
int R_pr_learn_svm_execute( int nHandle, int nMode );																	//SVM�ɂ��w�K���s
int R_pr_learn_set_element( int nLearnDataHandle, int number, int dim, double element );								//�w�K�p�f�[�^����
int R_pr_learn_get_element( int nLearnDataHandle, int number, int dim, double *element );								//�w�K�p�f�[�^�o��
int R_pr_learn_svm_judgement( int nHandle, R_VECTORD *str_org, int mode );												//�f�[�^��SVM���ʊ�ɂ�锻��
int R_pr_learn_svm_transfer_data( int nHandle, R_VECTORD *str_org, R_VECTORD *str_buff );								//�f�[�^��SVM���K��Ԃւ̎ʑ�


//int R_pr_learn_disp_data_contents( int cx, int cy, int nLearnDataHandle, int number );									
//int R_pr_learn_ability_of_classifier( int nHandle, int *percent );
//int R_pr_learn_consideration_from_file( unsigned char *filename );

#endif	// R_PR_H_
