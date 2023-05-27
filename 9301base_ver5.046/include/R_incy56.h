/******************************************************************************
	ＩＮ－ＣＹ－５６（差動ドライバーインターフェース）ライブラリー
	
	R_INCY56.H / R_INCY56.C
	
	変更履歴
	Ver			1995/03/02	release
	Ver 3.24	2000/04/14	IN-CY-84対応
	Ver 3.28	2000/11/20	マクロの変数化
	Ver 3.46	2005/06/16	IN-CY-84複数枚対応
	Ver 3.50	2009/02/12	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_INCY56_H_
#define	R_INCY56_H_

extern int	INCY84;

extern int	INCY56_INSTALL;
extern int	INCY56_IO_MEMORYa;
extern int	INCY56_IO_MEMORYb;

#define	INCY56_8255_1_A_port	(INCY56_8255_1+0)	// #define	INCY56_8255_1_A_port	0x0108
#define	INCY56_8255_1_B_port	(INCY56_8255_1+1)	// #define	INCY56_8255_1_B_port	0x0109
#define	INCY56_8255_1_C_port	(INCY56_8255_1+2)	// #define	INCY56_8255_1_C_port	0x010A
#define	INCY56_8255_1_control	(INCY56_8255_1+3)	// #define	INCY56_8255_1_control	0x010B
#define	INCY56_8255_2_A_port	(INCY56_8255_2+0)	// #define	INCY56_8255_2_A_port	0x0508
#define	INCY56_8255_2_B_port	(INCY56_8255_2+1)	// #define	INCY56_8255_2_B_port	0x0509
#define	INCY56_8255_2_C_port	(INCY56_8255_2+2)	// #define	INCY56_8255_2_C_port	0x050A
#define	INCY56_8255_2_control	(INCY56_8255_2+3)	// #define	INCY56_8255_2_control	0x050B
#define	INCY56_8254_counter0	(INCY56_8254+0)		// #define	INCY56_8254_counter0	0x0908
#define	INCY56_8254_counter1	(INCY56_8254+1)		// #define	INCY56_8254_counter1	0x0909
#define	INCY56_8254_counter2	(INCY56_8254+2)		// #define	INCY56_8254_counter2	0x090A
#define	INCY56_8254_control		(INCY56_8254+3)		// #define	INCY56_8254_control		0x090B

extern int	INCY56_8255_1;			// #define	INCY56_8255_1	0x0108
extern int	INCY56_8255_2;			// #define	INCY56_8255_2	0x0508
extern int	INCY56_8254;			// #define	INCY56_8254		0x0908

#define	IN00	0x000001
#define	IN01	0x000002
#define	IN02	0x000004
#define	IN03	0x000008
#define	IN04	0x000010
#define	IN05	0x000020
#define	IN06	0x000040
#define	IN07	0x000080
#define	IN08	0x000100
#define	IN09	0x000200
#define	IN10	0x000400
#define	IN11	0x000800
#define	IN12	0x001000
#define	IN13	0x002000
#define	IN14	0x004000
#define	IN15	0x008000
#define	IN16	0x000100
#define	IN17	0x000200
#define	IN18	0x000400
#define	IN19	0x000800
#define	IN20	0x001000
#define	IN21	0x002000
#define	IN22	0x004000
#define	IN23	0x008000
#define	IN24	0x010000
#define	IN25	0x020000
#define	IN26	0x040000
#define	IN27	0x080000
#define	IN28	0x100000
#define	IN29	0x200000
#define	IN30	0x400000
#define	IN31	0x800000

#define	OUT00	0x000001
#define	OUT01	0x000002
#define	OUT02	0x000004
#define	OUT03	0x000008
#define	OUT04	0x000010
#define	OUT05	0x000020
#define	OUT06	0x000040
#define	OUT07	0x000080
#define	OUT08	0x000100
#define	OUT09	0x000200
#define	OUT10	0x000400
#define	OUT11	0x000800
#define	OUT12	0x001000
#define	OUT13	0x002000
#define	OUT14	0x004000
#define	OUT15	0x008000
#define	OUT16	0x000100
#define	OUT17	0x000200
#define	OUT18	0x000400
#define	OUT19	0x000800
#define	OUT20	0x001000
#define	OUT21	0x002000
#define	OUT22	0x004000
#define	OUT23	0x008000
#define	OUT24	0x010000
#define	OUT25	0x020000
#define	OUT26	0x040000
#define	OUT27	0x080000
#define	OUT28	0x100000
#define	OUT29	0x200000
#define	OUT30	0x400000
#define	OUT31	0x800000

#define	INCY56CNa	0
#define	INCY56CNb	1

int	R_open_incy56(void);
int	R_input_8254(void);
int	R_start_8254(void);
int	R_read_8254(void);

int	R_write_incy56(int connector, int data);
int	R_read_incy56(int connector);
int	R_on_incy56(int connector, int bit);
int	R_off_incy56(int connector, int bit);

#define R_open_incy74		R_open_incy56
#define R_write_incy74		R_write_incy56
#define R_read_incy74		R_read_incy56
#define R_on_incy74			R_on_incy56
#define R_off_incy74		R_off_incy56

#define R_open_incy84		R_open_dlio_pci
#define R_write_incy84		R_write_incy56
#define R_read_incy84		R_read_incy56
#define R_on_incy84			R_on_incy56
#define R_off_incy84		R_off_incy56

#endif	// R_INCY56_H_
