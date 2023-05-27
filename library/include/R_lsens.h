/******************************************************************************

	line sensor I/F

;	FIFO  low memory address 0C0000h
;	FIFO high memory address 0C0001h
;	MB86250	I/O address	0x010c, 0x010d, 0x050c, 0x050d, 0x090c,0x090d, 0x0d0c, 0x0d0d
;	IN0			0x110c
;	OUT0			0x110c
;	OUT1			0x110d
	
	変更履歴：
		1999/02/15	CO-CY-32(2K)対応
		2009/06/09	再インクルード防止
******************************************************************************/

#ifndef	R_LSENS_H_
#define	R_LSENS_H_

// camera code

#define	SH2048B		0		// NED SH-2048B
#define	BLACK_COLOR	0
#define	WHITE_COLOR	1


	#define	GRAY_PLANE	0x01
	#define	BIN_PLANE	0x02
	#define	EXPAND_PLANE	0x10000


/*		port number		*/
#define	MB86250_CCRA	0x810c
#define	MB86250_CCRBl	0x810d
#define	MB86250_CCRBh	0x910c
#define	MB86250_CCRC	0x910d
#define	MB86250_CCRD	0x890c
#define	MB86250_CCRE	0x890d
#define	MB86250_CCRF	0x990c
#define	MB86250_IN0		0x110c
#define	MB86250_IN1		0x110d
#define	MB86250_OUT0	0x110c
#define	MB86250_OUT1	0x110d

//	MB86250_CCRA
#define	CCRA0			0x01
#define	ACCF			0x20
#define	HSYNC_CRY		0x20

//	MB86250_IN0
#define	LSstart_int		0x01
#define	LSenable_int	0x02
#define	FIFO_LOW_EMP	0x04
#define	FIFO_HIGH_EMP	0x08
#define	FIFO_LOW_HF		0x10
#define	FIFO_HIGH_HF	0x20
#define	FIFO_LOW_FF		0x40
#define	FIFO_HIGH_FF	0x80

//	MB86250_IN1
#define	COCY25_EXTI0	0x01
#define	COCY25_EXTI1	0x02
#define	COCY25_EXTI2	0x04
#define	COCY25_EXTI3	0x08
#define	COCY25_I4		0x10
#define	COCY25_I5		0x20
#define	COCY25_I6		0x40
#define	VDEN			0x80

//	MB86250_OUT0
#define	EXTO0			0x01
#define	EXTO1			0x02
#define	EXTO2			0x04
#define	EXTO3			0x08
#define	CLKRES			0x10
#define	FIFORS			0x20
#define	PSYNC			0x40
#define	Vstart			0x80

//	MB86250_OUT1
#define	MB86250_RES		0x01
#define	COCY25_OUT1		0x02
#define	COCY25_OUT2		0x04
#define	COCY25_OUT3		0x08
#define	COCY25_OUT4		0x10
#define	COCY25_OUT5		0x20
#define	COCY25_OUT6		0x40
#define	COCY25_OUT7		0x80


#define	COCY25_FIFO		0xd0000


#ifdef	LSENS_FILE
	unsigned char	*EX_Gray_address[10]={0,0,0,0,0,0,0,0,0,0};
	unsigned char	*EX_Bin_address[10]={0,0,0,0,0,0,0,0,0,0};
	int	Lsens_Hlength;
	int	Lsens_HlengthShadeUse;
	int	Lsens_Vlength;
	int	Lsens_scanrate;
	int	Lsens_clamp;
	// I/O memory
	char	INCY25_IO_MEMORY0=0;
	char	INCY25_IO_MEMORY1=0;
	int		LineSensorBinaryThesold=0x80;
#else
	extern unsigned char	*EX_Gray_address[10];
	extern unsigned char	*EX_Bin_address[10];
	extern int	Lsens_Hlength;
	extern int	Lsens_HlengthShadeUse;
	extern int	Lsens_Vlength;
	extern int	Lsens_scanrate;
	extern int	Lsens_clamp;
	// I/O memory
	extern char	INCY25_IO_MEMORY0;
	extern char	INCY25_IO_MEMORY1;
	extern int	LineSensorBinaryThesold;
#endif

int R_lsensor_init(int camera_code, int scan_rate, int v_length);
int R_lsensor_init_shading_ram(int mode);
int R_lsensor_get_shading_ram(int memory_color, void *buffer_address);
int R_lsensor_set_shading_ram(int memory_color, void *buffer_address);
int R_lsensor_get_blacklevel(int mode);
int R_lsensor_get_whitelevel(int mode );
int R_lsensor_get_video(int mode, int page);
int R_lsensor_get_video_AnyLine( int line, int mode, unsigned char *buff );
int R_lsensor_binlevel( int level ) ;

void	R_output25_judge(int nJudge);
void	R_output25_external_io(int nValue);
void	R_wait25_external_io_start( void ) ;
int		R_check25_external_io_start( void ) ;
int		R_input25_external_io( void ) ;

#endif	// R_LSENS_H_
