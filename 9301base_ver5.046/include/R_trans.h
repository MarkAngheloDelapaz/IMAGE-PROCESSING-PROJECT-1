/******************************************************************************
	�P�������ڃf�[�^��̓��C�u����
	R_TRANS.C / R_TRANS.H
	
	����
	Version 3.50		2007.12.07.000		saito	�I���W�i��
						2008.03.18.001		saito	�}�N���s��C��
													
******************************************************************************/

#ifndef	R_TRANS_H_
#define	R_TRANS_H_

#define		TRANS_TYPE_INT			0				//int�^�f�[�^����������
#define		TRANS_TYPE_GRAYIMAGE	1				//GrayImage(unsigned char�^�j�f�[�^����������
#define		TRANS_DIFF_STEP_MAX		3				

//�G�b�W���o����
#define		TRANS_DIR_MASK			0x0000000f		//dir   �̃`�F�b�N�p�̃}�X�N
#define		TRANS_DIR_0				0x00000001		//�E  �����փf�[�^����	only gray image
#define		TRANS_DIR_45			0x00000002		//�E�������փf�[�^����	only gray image
#define		TRANS_DIR_90			0x00000003		//��  �����փf�[�^����	only gray image
#define		TRANS_DIR_135			0x00000004		//���������փf�[�^����	only gray image
#define		TRANS_DIR_180			0x00000005		//��  �����փf�[�^����	only gray image
#define		TRANS_DIR_225			0x00000006		//��������փf�[�^����	only gray image
#define		TRANS_DIR_270			0x00000007		//��  �����փf�[�^����	only gray image
#define		TRANS_DIR_315			0x00000008		//�E������փf�[�^����	only gray image

//�G�b�W���o���[�h
#define		TRANS_MODE_MASK			0x000000f0		//mode  �̃`�F�b�N�p�̃}�X�N
#define		TRANS_MODE_BtoW			0x00000010		//�������i�l������j�̕ψʓ_�����o
#define		TRANS_MODE_WtoB			0x00000020		//�������i�l�偨���j�̕ψʓ_�����o
#define		TRANS_MODE_BWtoWB		0x00000030		//�������i�l�偨���j�A�������i�l������j�̕ψʓ_�����o
#define		TRANS_MODE_BtoWtoB		0x00000040		//�����������i�l�����偨���j�̕ψʓ_�����o
#define		TRANS_MODE_WtoBtoW		0x00000050		//�����������i�l�偨������j�̕ψʓ_�����o

//�G�b�W���o�I��
#define		TRANS_CHOICE_MASK		0x00000f00		//choide�̃`�F�b�N�p�̃}�X�N
#define		TRANS_CHOICE_N_BEGIN	0x00000100		//�f�[�^�_��̊J�n�����炎�Ԗڂ̕ψʓ_�����o
#define		TRANS_CHOICE_N_END		0x00000200		//�f�[�^�_��̏I�������炎�Ԗڂ̕ψʓ_�����o
#define		TRANS_CHOICE_MAX_AMP	0x00000300		//�f�[�^�_��̐U���ő�̕ψʓ_�����o
#define		TRANS_CHOICE_MAX_AREA	0x00000400		//�f�[�^�_��̐U���̈�̕ψʓ_�����o
#define		TRANS_CHOICE_CLOSED		0x00000500		//���g�p

//�f�[�^��������@
#define		TRANS_SMOOTH_MASK		0x0000f000		//smooth�̃`�F�b�N�p�̃}�X�N
#define		TRANS_SMOOTH_AVE		0x00001000		//���ϒl��p����
#define		TRANS_SMOOTH_MEDIAN		0x00002000		//�����l��p����

//�f�[�^�쐬�p�i���Ή��j
#define		TRANS_DIR_X				0				//�w�����Ƀf�[�^����
#define		TRANS_DIR_Y				1				//�x�����Ƀf�[�^����

#define		TRANS_CREATE_AVE		0				//���ϒl��p����
#define		TRANS_CREATE_MEDIAN		1				//�����l�i���f�B�A���j��p����
#define		TRANS_CREATE_SUM		2				//���a��p����

//�T�[�`�p�����[�^�ݒ�֐��i�}�N���j
#define R_trans_para_set_start(trans,start)				((trans).point_start=start)		// �G�b�W���o�̏����J�n�_�̐ݒ�
#define R_trans_para_set_end(trans,end)					((trans).point_end=end)			// �G�b�W���o�̏����I���_�̐ݒ�
#define R_trans_para_set_count_smooth(trans,smooth)		((trans).count_smooth=smooth)	// �f�[�^�̕������񐔂̐ݒ�
#define R_trans_para_set_index(trans,idx)				((trans).index=idx)				// ���Ԗڂ̃G�b�W�����o���邩
#define R_trans_para_set_thickness(trans,thin)			((trans).thickness=thin)		// �������̃f�[�^������
#define R_trans_para_set_diff_step(trans,step)			((trans).diff_step=step)		// ����STEP : d(i) = d(i+step)-d(i-step)
#define R_trans_para_set_thresh_amp(trans,th_amp)		((trans).thresh_amp=th_amp)		// �U���������l�F����ȉ��̐U���̃f�[�^�͖�������
#define R_trans_para_set_thresh_area(trans,th_area)		((trans).thresh_area=th_area)	// �ʐς������l�F����ȉ��̖ʐς̃f�[�^�͖�������

#define R_trans_para_get_start(trans)					((trans).point_start)
#define R_trans_para_get_end(trans)						((trans).point_end)
#define R_trans_para_get_count_smooth(trans)			((trans).count_smooth)
#define R_trans_para_get_index(trans)					((trans).index)
#define R_trans_para_get_thickness(trans)				((trans).thickness)
#define R_trans_para_get_diff_step(trans)				((trans).diff_step)
#define R_trans_para_get_thresh_amp(trans)				((trans).thresh_amp)
#define R_trans_para_get_thresh_area(trans)				((trans).thresh_area)
#define R_trans_para_get_detect_point(trans)			((trans).point_edge)


// --- variable ---
//�G�b�W���_�\����
typedef struct{
	int		num;					//�f�[�^�ԍ��i���Ԗڂ��j
	int		amp;					//�U���l�i�����P�O�O�̒l�j
	int		area;					//�ʐϒl�i�U���l�̐ϕ��j
}R_TRANS_INDEX;

//�P���f�[�^�z��\����
typedef struct{
	int		id;						//�\����ID �� CREATE����TRANS_ID����Ă����
	int 	*data_int;				//�f�[�^��iint   *�^)
	UCHAR	*data_gray;				//�f�[�^��iuchar *�^)
	int		*data_work;				//��Ɨ̈�
	int		*data_smooth;			//������������̃f�[�^��iuchar*�^�j
	int		*data_diff;				//�f�[�^��̔����l data_diff(n) = data(n+1)-data(n-1)
	int		*data_area;				//�G�b�W���o�_�̎��ʐ�
	int		*data_edge;				//�G�b�W���o�t���O 0:not edge  1:edge point
	int		data_type;				//�f�[�^�^�C�v�̑I���@�iINT�^/GRAYIMAGE�^�j
	int		dir;					//GRAYIMAGE�^�̂݁@�f�[�^����
	int		mem_offset;				//GRAYIMAGE�^�̂݁@�������A�h���X�I�t�Z�b�g�T�C�Y
	int		point_start;			//start �ʒu �G�b�W���o�� �����̈�
	int		point_end;				//end   �ʒu �G�b�W���o�� �����̈�
	int		point_edge;				//�ŏI���o�G�b�W�ʒu �f�[�^�ԍ�(0,1,2,...)
	int		point_edge_fine;		//�ŏI���o�G�b�W�ʒu�i�����x�j�f�[�^�ԍ���1000�{�l (0,1000,2000,...)
	int		count_data;				//�f�[�^�v�f��
	int		count_data_max;			//�f�[�^�v�f�ő吔
	int		count_smooth;			//��������
	int		thickness;				//�������������̃f�[�^��
	int		index;					//�G�b�W�I���@�`N_BEGIN,N_END���ɉ��Ԗڂ̃G�b�W�����o���邩
	int		diff_step;				//�����r�s�d�o
	int		diff_max;				//�����l�̍ő�l
	int		thresh_diff;			//�U���������l�F����ȉ��̐U���̃f�[�^�͖�������i���̒l�j
	int		thresh_amp;				//�U���������l�F����ȉ��̐U���̃f�[�^�͖�������i���̒l�j
	int		thresh_area;			//�ʐς������l�F����ȉ��̖ʐς̃f�[�^�͖�������
	R_TRANS_INDEX	*edge_index;	//�G�b�W���_�\����
	int		edge_index_count;		//�G�b�W���_��
}R_TRANS_DATA;

// --- prototype ---
int R_trans_data_open( R_TRANS_DATA *trans, int type_data, int count_data_max );							// �P�����f�[�^�z��\���̂̃I�[�v��
int R_trans_data_close( R_TRANS_DATA *trans );																// �P�����f�[�^�z��\���̂̃N���[�Y
int R_trans_edge_detect_int( R_TRANS_DATA *trans, int *data, int data_count, int mode );
int R_trans_edge_detect_grayimage( R_TRANS_DATA *trans, int src_mem_no, int posi_x, int posi_y, int length, int mode );

#endif	// R_TRANS_H_
