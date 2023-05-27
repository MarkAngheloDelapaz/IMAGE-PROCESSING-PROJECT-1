/******************************************************************************

	R_CS_GRP.H
	
	�O���t�\���֐��w�b�_
	�o�[�W�����A�b�v���́A�K�������̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		2004.05.05.000		�I���W�i��
				2007.10.25.001		�܂���O���t�\����ǉ�

******************************************************************************/

// --- define ---
#define GRAPH_SCATTER	0							//�U�z�}
#define GRAPH_BAR		1							//�_�O���t
#define GRAPH_LINE		2							//�܂���O���t

#define GRAPH_DATA_NB	4

#ifdef __cplusplus
extern "C"{
#endif


//�O���t�\���\����
typedef struct{
	char			*label_j;					//���x��(���{��)
	char			*label_e;					//���x��(�p��)
	int				min;						//�ŏ��l
	int				max;						//�ő�l
}R_CS_GRAPH_ITEM_AXIS;

typedef struct{
	char			*tytle_j;					//�O���t�̃^�C�g��(���{��)
	char			*tytle_e;					//�O���t�̃^�C�g��(�p��)
	int				*data[GRAPH_DATA_NB];		//�`�悷��f�[�^�z��̐擪�|�C���^
	int				number;						//�`�悷��f�[�^�z��̌�
	R_CS_GRAPH_ITEM_AXIS	axis_x;						//X���̏��
	R_CS_GRAPH_ITEM_AXIS	axis_y;						//Y���̏��
	R_RECT			disp_area;					//�`��G���A
	int				mode;						//�`�惂�[�h
}R_CS_GRAPH_ITEM;

// --- variable ---

// --- prototype ---
int R_CS_GraphDisp( R_CS_GRAPH_ITEM *item );
int R_CS_GraphDispRing( R_CS_GRAPH_ITEM *item, int start, int end, int size );


#ifdef __cplusplus
}
#endif
