/******************************************************************************
	R_cunet.h / R_cunet.c
	
	�ύX����
	Ver 4.01	2012/03/15	�V�K�ǉ�
	Ver 5.018	2019/10/24	MECY35�Ή�
******************************************************************************/

#ifndef	R_CUNET_H_
#define	R_CUNET_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//int	r_cunet_open(int id);
//int	r_cunet_inp(int handle, int channel);
//int	r_cunet_inpw(int handle, int channel);
//int	r_cunet_outp(int handle, int channel, int data);
//int	r_cunet_outpw(int handle, int channel, int data);
/*
typedef struct {
	short address;
	char width;
} RCUNET_REG;
*/

#define CUNET_MECY32	0
#define CUNET_MECY35	1
#define CUNET_UNKNOWN	-1


uintptr_t r_cunet_base_port(int id);
uintptr_t r_cunet_system_port(int id);

#ifdef __DOS__
void	r_cunet_print_info(int, int, int);
#elif __GNUC__
int	RCunetOpenALL(int num, int multi);
#endif

/**
*	@brief	CUnet��̃I�[�v��
*
*	@param	int id ���ID �i���0��ݒ�j
*			int mode ���샂�[�h
*				�� ��						�� ��
*				R_CUNET_EXCLUSIVE_MODE		���L���[�h
*				R_CUNET_SHARE_MODE			����L���[�h
*
*	@return
*			OK		�ݒ芮���i����I���j
*			ERROR	CUnet������݂��Ȃ�
*			ERROR2	�f�o�C�X�h���C�o�̃o�[�W�������Â��A����L���[�h�ɑΉ����Ă��Ȃ��B
*
*
*
*
*/
int	R_cunet_open(int id, int mode);
#define	R_CUNET_EXCLUSIVE_MODE	0
#define	R_CUNET_SHARE_MODE		1

/**
*	@brief	CUnet���L�������ւ̏������݌����̎擾
*
*	@param	int offset �I�t�Z�b�g�A�h���X
*			int bytes �o�C�g��
*
*	@return
*			OK		�ݒ芮���i����I���j
*			ERROR	CUnet����I�[�v������Ă��Ȃ�
*			ERROR2	����L���[�h�ŃI�[�v������Ă��Ȃ�
*
*
*
*
*/
int	R_cunet_make_own_bytes(int offset, int bytes);

/**
*	@brief	CUnet���L�������ւ̏������݌����̎擾
*
*	@param	int offset �I�t�Z�b�g�A�h���X
*			int bytes �o�C�g��
*
*	@return
*			OK		�ݒ芮���i����I���j
*			ERROR	CUnet����I�[�v������Ă��Ȃ�
*			ERROR2	����L���[�h�ŃI�[�v������Ă��Ȃ�
*
*
*
*
*/
int	R_cunet_inp(int offset);
int	R_cunet_outp(int offset, int data);
int	R_cunet_on(int offset, int bits);
int R_cunet_off(int offset, int bits);

int	R_cunet_inpw(int offset);
int	R_cunet_outpw(int offset, int data);
int	R_cunet_inpd(int offset);
int	R_cunet_outpd(int offset, int data);
uint64_t	R_cunet_inpq(int offset);
int	R_cunet_outpq(int offset, uint64_t data);

int	R_cunet_debug(void);
int	R_cunet_set_write_sa(int sa, int own);
int	R_cunet_set_read_sa(int sa, int own);
int	R_cunet_read_output_status(int offset);

int	R_cunet_start(int msec);
int	R_cunet_stop(void);

void*	R_cunet_base_ptr(void);
void*	R_cunet_reg_ptr(void);
int		R_cunet_get_current_board_type(void);
int		R_cunet_get_current_board_id(void);


#ifdef __GNUC__

int		R_cunet_get_read_sa();
int		R_cunet_get_write_sa();
int		R_cunet_get_read_own();
int		R_cunet_get_write_own();

#endif

uint16_t R_cunet_read_bcr();
uint16_t R_cunet_read_scr();
uint16_t R_cunet_read_cctr();

int R_cunet_wrate_cctr(uint16_t arg);

int	R_cunet_count_of_mecy35(void);

int R_cunet_change_current_board(int id);

/* ***	prototype defines  ********************************************************************* */
// �b�t�������p�̊֐�
unsigned long CUNETf_init_once( int, int );
void CUNETf_check( int );
int CUNETf_init( unsigned long, int, int );
void CUNETf_clear_memory( int, int );
int CUNETf_start( void );
void CUNETf_clear_care( void );
int CUNETf_test_care( void );
int CUNETf_test_run( void );
int CUNETf_set_ls( int );
unsigned char CUNETf_mem_rd( unsigned long, unsigned long );
unsigned char CUNETf_mem_rd_end( unsigned long, unsigned long );
int CUNETf_mem_wr( unsigned long, unsigned char );
int CUNETf_mem_wr_end( unsigned long, unsigned char );
void * CUNETf_CUBPCI_get_baseaddress( void );

unsigned long CUNETf_get_base_address(void);




#ifdef __cplusplus
}
#endif

// MKY40 registers
#define	MKY40_SCR	0x44C	// System Control Register
#define	MKY40_BCR	0x498	// Basic Control Register
#define	MKY40_PWRCR	0x400	// Primary Window Read Control Register
#define	MKY40_PWWCR	0x404	// Primary Window Write Control Register

// MKY43 registers
#define	MKY43_SCR	0x366	// System Control Register
#define	MKY43_BCR	0x356	// Basic Control Register
#define	MKY43_RHCR0	0x338	// Read Hazard Control Register 0
#define	MKY43_RHPB0	0x370	// Read Hazard Protection Buffer 0
#define	MKY43_WHCR0	0x33C	// Write Hazard Control Register 0
#define	MKY43_WHPB0	0x380	// Write Hazard Protection Buffer 0

#endif	// R_CUNET_H_
