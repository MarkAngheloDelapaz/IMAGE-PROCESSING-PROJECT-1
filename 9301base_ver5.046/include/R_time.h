/******************************************************************************
	R_time.h / R_time.c
	
	�ύX����
	Ver 3.20	1999/12/12	R_time_start�AR_time_read�ǉ�
	Ver 3.21	1999/12/21	R_start_clk_count,R_read_clk_count��TSC���[�h��ǉ�
	Ver 3.??	2002/09/11	TimeUp()��R_video.c����ړ�
	Ver 3.41	2003/12/12	R_time_read_ms,R_time_read_sec�ǉ�
	Ver 3.44	2004/10/13	R_time_init�AR_time_get_cpu_frequency��ǉ�
	Ver 3.46	2005/08/18	�����ϐ�CheckRegistOverForDiv�`��R_stdlib.h����ړ�
	Ver 3.49	2006/11/15	R_daytim.c R_set_DATE_TIME() �𑜓x���Ⴂ�Ƃ��̃G���[������ǉ�
	Ver 3.49	2006/12/20	r_time_read_tsc_us�ǉ�
	Ver 3.49	2007/03/29	bRdtscIsAvailable�ǉ�
	Ver 3.50	2009/03/12	DOS & LINUX �\�[�X����
	Ver 3.50	2009/07/07	r_time_i�`�֐��ǉ�
	Ver 4.01	2010/09/17	gettimeofday��clock_gettime�ɕύX
	Ver 4.01	2014/07/24	R_time_sleep�֐��ǉ�
	Ver 4.01	2014/10/28	r_time_hpet_open�֐��̃G���[�\�����폜
	Ver 4.01	2017/09/07	R_time_read��_read_ms�̃t���[���̖߂�l��ύX
******************************************************************************/

#ifndef	R_TIME_H_
#define	R_TIME_H_

#include "R_stdlib.h"

#ifdef __GNUC__
#include <sys/time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define	BIOS_TCOUNT	0x46c			/* ���������@�����������i�T�T�l�r�d�b�j	*/
#define	BIOS_TIME_BASE			54925		/*	�P�ʂ́A�ʂ�����	*/
// #define	BIOS_TIME_BASE			55000		/*	�P�ʂ́A�ʂ�����	*/
#define	HS_TIME_BASE			64			/*	�P�ʂ́A�ʂ�����	*/
#define	HS_COUNTER_TIME_BASE	512			/*	�P�ʂ́A�ʂ�����	64�� �~ 8���� 					*/
#define	HS_COUNTER_TIME_MAX		131072		/*	�P�ʂ́A�ʂ�����	64�� �~ 8���� �~ 256�J�E���g	*/


#define	COUNT_64	0x10		/*	I/O output data	*/
#define	COUNT_128	0x20
#define	COUNT_256	0x30
#define	COUNT_512	0x40
#define	COUNT_1024	0x50
#define	COUNT_2048	0x60
#define	COUNT_4096	0x70

#define	R_wait_condition(time,condition,func)	{ R_start_clk();	while((time)>R_read_clk()){ if(condition) {func; break; }}}

#ifdef	TIME_FILE
	DLONG Start_timer_P5;							/* �X�^�[�g����P5 RDTSC�̃J�E���^�[�@*/
	unsigned int Start_timer_8254;					/* �X�^�[�g����8254�̃J�E���^�[�@*/
	unsigned char Start_timer_count;				/* �X�^�[�g����HS�̃J�E���^�[�@*/
	unsigned short Start_timer_countbios;			/* �X�^�[�g����BIOS�J�E���^�[ */
#else
	extern DLONG Start_timer_P5;					/* �X�^�[�g����P5 RDTSC�̃J�E���^�[�@*/
	extern unsigned int Start_timer_8254;			/* �X�^�[�g����8254�̃J�E���^�[�@*/
	extern unsigned char Start_timer_count;			/* �X�^�[�g����HS�̃J�E���^�[�@*/
	extern unsigned short Start_timer_countbios;	/* �X�^�[�g����BIOS�J�E���^�[ */
#endif




int	R_time_init(void);
int	R_time_get_cpu_frequency(void);

int	R_start_clk( void ) ;
int	R_read_clk( void ) ;
int	R_time_delay( unsigned int time ) ;
#ifdef __DOS__
	#define	R_time_sleep(micro_sec)	R_time_delay(micro_sec)
#elif __GNUC__
	int	R_time_sleep(unsigned int micro_sec);
#endif
int	R_start_clk_count( int mode ) ;
int	R_read_clk_count( void ) ;
int	R_get_time_bios( void ) ;
int	R_set_time_bios( void ) ;

// R_daytim.c	�c�n�r���Ԃ̐ݒ�
int R_set_DATE(int x,int y);
int R_set_TIME(int x,int y);
int R_set_DATE_TIME(int x,int y);

int R_GetSystemTimerCOCZ27( void );

union BIOS_HS {
	unsigned char	hs;					// �X�^�[�g����HS�̃J�E���^
	unsigned short	bios;				// �X�^�[�g����BIOS�J�E���^
};

#ifdef __DOS__			// 2009/03/11
typedef union {
	DLONG			timer_P5;			// �X�^�[�g����P5 RDTSC�̃J�E���^
	int				timer_COCZ27;		// �X�^�[�g����COCZ27�̃J�E���^
	unsigned int	timer_8254;			// �X�^�[�g����8254�̃J�E���^
	union BIOS_HS	timer_bios_hs;
} RTIME;
#elif __GNUC__
//typedef	struct timeval	RTIME;
typedef	struct timespec	RTIME;
#endif

int		R_time_start(RTIME*);
int		R_time_read(RTIME*);
int		R_time_read_ms(RTIME*);
int		R_time_read_sec(RTIME*);

int	TimeUp(int timeup);

// �����֐�
unsigned long	r_time_read_tsc_us(void);	//  R_plane���p

#ifdef __DOS__
	#define	r_time_istart(pTime)	R_time_start(pTime)
	#define	r_time_iread(pTime)		R_time_read(pTime)
	#define	r_time_idelay(nTime)	R_time_delay(nTime)
#elif __GNUC__
	int	r_time_istart(RTIME*);
	int	r_time_iread(RTIME*);
	int	r_time_idelay(unsigned int);
	int	r_time_hpet_open(void);
	int	r_time_hpet_close(void);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_TIME_H_
