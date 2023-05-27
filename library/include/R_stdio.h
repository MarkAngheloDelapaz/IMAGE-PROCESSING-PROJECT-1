/*
 *	ÅÉÇíÅQÇìÇîÇÑÇâÇèÅDÇàÅÑ	
 *
 *		ÇhÅ^Çn	ä÷åWÇÃêÈåæÇ®ÇÊÇ—ä÷êî
 *
 *
 *
 *
 *
 *		ÇqÇèÇàÇçÅ@ê∂éYÉVÉXÉeÉÄäJî≠ïîóvëfãZèpäJî≠
 *
 *		ÇÜÇèÇíÅ@ÇbÇnÅ]ÇbÇyÅ]ÇQÇR
 *
 *		[History]
 *			Ver 0.0	12/April/1993
 *			Ver 0.1 2/July/1993		add:rs232c
 *			Ver	1.12	1994. 6.17	R_judge_outputÇ®ÇÊÇ—íËêîïœçX
 *			Ver 3.28	2000.11.20	incy56ä÷òAÉ}ÉNÉççÌèú
 *			Ver 3.50	2009.01.29	R_STDIO_H_í«â¡
 *			Ver 3.50	2009.05.26	USBÉVÉäÉAÉãÉ|Å[ÉgÉTÉ|Å[Égä÷êîÇÃí«â¡
 *			Ver 5.028	2020.06.09	COCZ47í«â¡
 */

#ifndef R_STDIO_H_
#define R_STDIO_H_

#include "R_gpio.h"
#include "R_sio2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	OFF		0
#define	ON		1
#define	YES		1
#define	NO		0


/*		port number		*/
#define	CRTC0_PORT	0x010E
#define	CRTC1_PORT	0x010F


/*		I/O bit couner	*/
#define	CRTC_VS			0x01	/*	IN0_PORT	*/
#define	CRTC_HS			0x02	/*	IN0_PORT	*/
#define	FEILD			0x04	/*	IN0_PORT	*/
#define	FIELD			0x04	/*	IN0_PORT	*/
#define	TO_OS_SW		0x08	/*	IN0_PORT	*/
#define	EXT_START		0x10	/*	IN0_PORT	*/

#define	DISPT			0x01	/*	IN1_PORT	*/

#define	FLASH_POWER		0x01	/*	OUT3_PORT	*/
#define	ADC_6			0x02	/*	OUT3_PORT	*/
#define	HS_BASE_RESET	0x04	/*	OUT3_PORT	*/
#define	HS_USER_RESET	0x08	/*	OUT3_PORT	*/
#define	HS_COUNTER_MODE	0x70	/*	OUT3_PORT	*/
#define	DISPT_CTL		0x80	/*	OUT3_PORT	*/



/*	CO-CZ-31 PortNumber	*/
#define	COCZ31GeneralIn	0x050e
#define	COCZ31GeneralInE	0x250e
#define	COCZ31VstartIn	0x150e
#define	COCZ31AdcbufIn	0x050f
#define	COCZ31Timer0	0x050f
#define	COCZ31Timer1	0x150f
#define	COCZ31Timer2	0x250f
#define	COCZ31Timer3	0x350f


#define	VIDEOLINE	0x8
#define VideoOutMode0	0x10
#define VideoOutMode1	0x20



#define	COCZ31GeneralOut0	0x090e
#define	COCZ31Vstart		0x190e
#define	COCZ31GeneralOut0E	0x290e
#define	COCZ31AdcbufOut0	0x090f
#define	COCZ31AdcbufOut0E	0x290f
#define	COCZ31GeneralOut2	0x0d0e
#define	COCZ31GeneralOut2E	0x1d0e
#define	COCZ31AdcbufOut1	0x0d0f
#define	COCZ31CodeNo		0x1d0f

/*	CO-CZ-27 PortNumber	*/
#define	COCZ27GeneralIn	0x050e
#define	COCZ27VstartIn	0x150e
#define	COCZ27AdcbufIn	0x050f
#define	COCZ27Timer0	0x050f
#define	COCZ27Timer1	0x150f
#define	COCZ27Timer2	0x250f
#define	COCZ27Timer3	0x350f


#define	COCZ27GeneralOut0	0x090e
#define	COCZ27Vstart		0x190e
#define	COCZ27AdcbufOut0	0x090f
#define	COCZ27GeneralOut2	0x0d0e
#define	COCZ27GeneralOut2E	0x1d0e
#define	COCZ27AdcbufOut1	0x0d0f
#define	COCZ27CodeNo		0x1d0f




/*	JUDGE	*/
#define	JUDGE_LOCK	0x00
#define	JUDGE_BUSY	0x40
#define	JUDGE_GO	0x70
#define	JUDGE_NG	0x60


/*	external io	*/
#define	EX_IO_NO1	0x10
#define	EX_IO_NO2	0x20
#define	EX_IO_NO3	0x40
#define	EX_IO_NO4	0x80

/*	INCY56	*/
/*
#define	INCY56_8255_1_A_port	0x0108
#define	INCY56_8255_1_B_port	0x0109
#define	INCY56_8255_1_C_port	0x010A
#define	INCY56_8255_1_control	0x010B
#define	INCY56_8255_2_A_port	0x0508
#define	INCY56_8255_2_B_port	0x0509
#define	INCY56_8255_2_C_port	0x050A
#define	INCY56_8255_2_control	0x050B
#define	INCY56_8254_counter0	0x0908
#define	INCY56_8254_counter1	0x0909
#define	INCY56_8254_counter2	0x090A
#define	INCY56_8254_control		0x090B

#define	INCY56_8255_1	0x0108
#define	INCY56_8255_2	0x0508
#define	INCY56_8254		0x0908
*/

#define	COCZ31MGeneralOut0	COCZ27MGeneralOut0
#define	COCZ31MVstart		COCZ27MVstart
#define	COCZ31MAdcbufOut0	COCZ27MAdcbufOut0
#define	COCZ31MGeneralOut2	COCZ27MGeneralOut2
#define	COCZ31MGeneralOut2E	COCZ27MGeneralOut2E
#define	COCZ31MAdcbufOut1	COCZ27MAdcbufOut1
#define	COCZ31MCodeNo		COCZ27MCodeNo


#ifdef	MAIN_FILE
//	union REGS	regs;
	
	unsigned		IN0_PORT = 0x050E;
	unsigned		IN1_PORT = 0x050F;
	unsigned		OUT0_PORT = 0x090E;
	unsigned		OUT1_PORT = 0x090F;
	unsigned		OUT2_PORT = 0x0D0E;
	unsigned		OUT3_PORT = 0x0D0F;
	unsigned		OUT4_PORT = 0xFFFF;
	unsigned		OUT5_PORT = 0xFFFF;
	unsigned		OUT6_PORT = 0xFFFF;
	unsigned		OUT7_PORT = 0xFFFF;
	
	unsigned char	OUT0_DATA = 0x40;
	unsigned char	OUT1_DATA = 0;
	unsigned char	OUT2_DATA = 0;
	unsigned char	OUT3_DATA = 0;
	unsigned char	OUT4_DATA = 0;
	unsigned char	OUT5_DATA = 0;
	unsigned char	OUT6_DATA = 0;
	unsigned char	OUT7_DATA = 0;
	
	unsigned char	COCZ27MGeneralOut0=0;
	unsigned char	COCZ27MVstart=0;
	unsigned char	COCZ27MAdcbufOut0=0;
	unsigned char	COCZ27MAdcbufOut0E=0;
	unsigned char	COCZ27MGeneralOut2=0;
	unsigned char	COCZ27MGeneralOut2E=0;
	unsigned char	COCZ27MAdcbufOut1=0;
	unsigned char	COCZ27MCodeNo=0;
	
	unsigned char	COCZ31MGeneralOut0E=0;
	
	//int				INCY56_INSTALL=0;
	short int		Printer_IO_address_1;
	short int		Printer_IO_address_2;
	short int		Printer_IO_address_3;
	short int		Printer_IO_address_4;
	int				COCZ23 = 0;
	int				COCY25 = 0;
	int				COCZ27 = 0;
	int				COCZ31 = 0;
	int				COCZ27ID[4]={0,0,0,0};

	int		SystemIsDPMI = 0;		// check from R_video.c (initiator)

	char BootStringsBuffer[50][100];
	int BootBufferCounter=0;

#else
//	extern union REGS	regs;
	
	extern unsigned			IN0_PORT;
	extern unsigned			IN1_PORT;
	extern unsigned			OUT0_PORT;
	extern unsigned			OUT1_PORT;
	extern unsigned			OUT2_PORT;
	extern unsigned			OUT3_PORT;
	extern unsigned			OUT4_PORT;
	extern unsigned			OUT5_PORT;
	extern unsigned			OUT6_PORT;
	extern unsigned			OUT7_PORT;
	
	extern unsigned char	OUT0_DATA;
	extern unsigned char	OUT1_DATA;
	extern unsigned char	OUT2_DATA;
	extern unsigned char	OUT3_DATA;
	extern unsigned char	OUT4_DATA;
	extern unsigned char	OUT5_DATA;
	extern unsigned char	OUT6_DATA;
	extern unsigned char	OUT7_DATA;
	
	extern unsigned char	COCZ27MGeneralOut0;
	extern unsigned char	COCZ27MVstart;
	extern unsigned char	COCZ27MAdcbufOut0;
	extern unsigned char	COCZ27MAdcbufOut0E;
	extern unsigned char	COCZ27MGeneralOut2;
	extern unsigned char	COCZ27MGeneralOut2E;
	extern unsigned char	COCZ27MAdcbufOut1;
	extern unsigned char	COCZ27MCodeNo;
	
	extern unsigned char	COCZ31MGeneralOut0E;
	
	//extern int				INCY56_INSTALL;
	extern short int		Printer_IO_address_1;
	extern short int		Printer_IO_address_2;
	extern short int		Printer_IO_address_3;
	extern short int		Printer_IO_address_4;
	extern int				COCZ23;
	extern int				COCY25;
	extern int				COCZ27;
	extern int				COCZ31;
	extern int				COCZ27ID[4];

	extern int	SystemIsDPMI;	// by ARAKI 1997.09.04


	extern char BootStringsBuffer[50][100];
	extern int BootBufferCounter;

#endif

extern int	COCZ47;




/*

Å@ÇqÇrÅQÇQÇRÇQÇbä÷òA

*/

int		R_open_sio( int, int, int, int, int, int );
void	R_close_sio( void );
int		R_getsio( void );
int		R_putsio( int );
int		R_putsio_str( char * );
int		R_getsio_sndbuff( void );
int		R_getsio_rcvbuff( void );
int		R_get_line_status( void );
int		r_sio_isNS16550( int ch );
/*
#ifdef	__GNUC__
#define	P_ACM	1
#define	P_USB	2
int		R_open_sio_n( int, int, int, int, int, int, int );	// USBÉVÉäÉAÉãëŒâûÅ@2009.05.21 tanaka
void	R_close_sio_n( int );
int		R_getsio_n( int );
int		R_putsio_n( int, int );
int		R_putsio_str_n( int, char * );
int		R_getsio_sndbuff_n( int );
int		R_getsio_rcvbuff_n( int );
#endif
*/

/*	external contol	*/

//	void R_judge_output(short int judge ) ;
	void R_output_judge(short int judge ) ;
	void R_output_status(short int mode ) ;
	int R_check_status_sw( void ) ;
	void R_wait_status_sw( void ) ;
	void R_output_external_io( int value ) ;
	int R_input_external_io( void ) ;
	int R_check_external_io_start( void );
	void R_wait_external_io_start( void );


// #define R_output_judge( judge ) {if(COCZ27){ COCZ27MGeneralOut2 = (COCZ27MGeneralOut2 & 0x8f)|(judge & 0x70); outp( COCZ27GeneralOut2, COCZ27MGeneralOut2); }
//									else { OUT2_DATA = (OUT2_DATA & 0x8f)|(judge & 0x70); outp( OUT2_PORT, OUT2_DATA);}}
// #define R_output_status( mode ) {if(COCZ27){ COCZ27MGeneralOut2 = (COCZ27MGeneralOut2 & 0xf7)|((mode<<3) & 0x08); outp( COCZ27GeneralOut2, COCZ27MGeneralOut2); }
//									else { OUT2_DATA = (OUT2_DATA & 0xf7)|((mode<<3) & 0x08); outp( OUT2_PORT, OUT2_DATA); }}
//	#define R_check_status_sw( )	( 0x01 & (inp(IN0_PORT)>>3))
//	#define R_wait_status_sw( )		{ while( 0x01 & (inp(IN0_PORT)>>3)); while(!( 0x01 & (inp(IN0_PORT)>>3))); }
// #define R_output_external_io( value )   {if(COCZ27){ COCZ27MGeneralOut2 = (COCZ27MGeneralOut2 & 0x7f)|(value & 0x80); outp( COCZ27GeneralOut2, COCZ27MGeneralOut2); }
//											else {  OUT2_DATA = (OUT2_DATA & 0x7f)|(value & 0x80); outp( OUT2_PORT, OUT2_DATA); }}


//	R_beep.c
void R_beep( int time );

#define minp(adr)	(*(volatile unsigned char *)adr)
#define moutp(adr,data)	{(*(volatile unsigned char *)adr)=data;}
#define minpw(adr)	(*(volatile unsigned short *)adr)
#define moutpw(adr,data)	{(*(volatile unsigned short *)adr)=data;}
#define minpd(adr)	(*(volatile unsigned int *)adr)
#define moutpd(adr,data)	{(*(volatile unsigned int *)adr)=data;}

#ifdef __cplusplus
}
#endif

#endif	// R_STDIO_H_
