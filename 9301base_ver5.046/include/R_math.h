/******************************************************************************
	R_math.h / R_math.c
	
	変更履歴
	Ver 3.50	2009/03/26	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_MATH_H_
#define	R_MATH_H_

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define		NULL	0
#define		OK		0
#define		ERROR	(-1)
#define		YES		1
#define		NO		0

/*		const data		 */
#define		PI		(double)3.141592654
#define		SCALE	(double)0x8000


/*		proto type 		*/
int		R_OPEN_MATHLIB( void );


/*		data teble		*/
#ifdef	R_MATH
	long int	Char_abs[256];			/*	abs signed char	*/
	long int	Char_sqr[256];	
	long int	Char_mul[256][256];	
#else
	extern long int	Char_abs[256];			/*	abs signed char	*/
	extern long int	Char_sqr[256];	
	extern long int	Char_mul[256][256];	
#endif


#ifndef	DLONG_DEF
#define	DLONG_DEF
typedef struct {
	unsigned long	lo;
	unsigned long	hi;
} DLONG ;
#endif


/* ***********************************************************
			r_atan36.c
************************************************************* */
#ifndef	R_MATH_TAN
	extern short	atan_data[1025];
#endif
int R_atan3600( int x, int y ) ;
#define R_atan( x, y ) (R_atan3600( x,y)/32)


/* ************************************************************
			r_sincos.c
*************************************************************** */
#ifndef	R_MATH_COS
	extern short	cos_data[901];
#endif
int R_sinfunc( int angle) ;
int R_cosfunc( int angle) ;


/* ************************************************************
			r_sqrt32.asm
*************************************************************** */
short	R_sqrt32( unsigned long val );

#ifdef __GNUC__	// 2009/07/21
	short	R_sqrt32_(unsigned long val);
#endif

/* ************************************************************
			r_sqrt64.asm
*************************************************************** */
unsigned long	R_sqrt64( unsigned long data_low, unsigned long data_high );

/* ******************************************************************************
			r_math64.asm
****************************************************************************** */
void	R_add6464( DLONG *src, DLONG *op, DLONG *dest );
void	R_sub6464(const DLONG* src, const DLONG* op, DLONG* dest);
void	R_div6464( DLONG *src, DLONG *op, DLONG *dest );

void	R_add6432( DLONG *, long, DLONG *dest );
void	R_sub6432( DLONG *, long, DLONG *dest );
int		R_mul6432( long, long, long, DLONG * );	// overflow : -1 OK:0
void	R_div6432( long, long, long, DLONG * );

void	R_shl64( DLONG *src, DLONG *dest ,int count );
void	R_shr64( DLONG *src, DLONG *dest ,int count );

#ifdef __GNUC__	// 2009/07/21
	void	R_add6464_(DLONG* src, DLONG* op, DLONG* dest);
	void	R_sub6464_(DLONG* src, DLONG* op, DLONG* dest);
	void	R_add6432_(DLONG*, long, DLONG*);
	int		R_mul6432_(long entry1, long entry2, long entry3, DLONG* entry4);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_MATH_H_
