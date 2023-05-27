/********************************************************************
	�p�������|�[�g����T�u���[�`��  for PC/AT
		(�o�W�F�g���Z1995.10, 1996.6�Ȃ�)

										Program by K.Matsunaga
********************************************************************/

/**********************************************************
	�g�p���@
		ParallelOpen()					�p�������|�[�g�̏�����
		ParallelClose()					�p�������|�[�g�̏I��
		ParallelMode()					�p�������|�[�g�̕����ݒ�
		ParallelBitSet(port,bit,data)	�p�������|�[�g�̃r�b�g�Z�b�g
		ParallelGetBit(port,bit)		�p�������|�[�g�̃r�b�g��Ԃ̎擾

**********************************************************/

// *** ��` ***
// �|�[�g�A�h���X
// LPT-1
//#define PARALLEL_DATA    0x3bc				// �f�[�^
//#define PARALLEL_STATUS  (PARALLEL_DATA+1)	// �������
//#define PARALLEL_CONTROL (PARALLEL_DATA+2)	// ����o��

// LPT-2
#define PARALLEL_DATA    0x378					// �f�[�^
#define PARALLEL_STATUS  (PARALLEL_DATA+1)		// �������
#define PARALLEL_CONTROL (PARALLEL_DATA+2)		// ����o��

// LPT-3
//#define PARALLEL_DATA    0x278				// �f�[�^
//#define PARALLEL_STATUS  (PARALLEL_DATA+1)	// �������
//#define PARALLEL_CONTROL (PARALLEL_DATA+2)	// ����o��

// �X�e�[�^�X�|�[�g��`
#define SLCT    0x10		// select
#define PE      0x20		// paper end
#define ACK     0x40		// ack
#define BUSY    0x80		// busy
#define ER		0x08		// error

// �R���g���[���|�[�g��`
#define STROBE  0x01
#define AUTOFD  0x02		// 
#define INIT    0x04		// 
#define SLCTIN  0x08		// 
#define INT     0x10		// 
#define DIR     0x20		// 

// �p�������|�[�g���[�h��`
#define PARALLEL_IN   0
#define PARALLEL_OUT  1
#define PARALLEL_ON   2
#define PARALLEL_OFF  3

// �X�e�[�^�X�|�[�g �r�b�g��`
#define QR_START      ER			// input<0x08>
#define QR_RESERVE_1  SLCT			// input<0x10>
#define QR_RESERVE_2  ACK			// input<0x40>

int ParallelOpen(void);
void ParallelClose(void);
/*
ParallelMode()					�p�������|�[�g�̕����ݒ�
ParallelBitSet(port,bit,data)	�p�������|�[�g�̃r�b�g�Z�b�g
ParallelGetBit(port,bit)		�p�������|�[�g�̃r�b�g��Ԃ̎擾
*/

