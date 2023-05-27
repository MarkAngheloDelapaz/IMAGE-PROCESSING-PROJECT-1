/******************************************************************************
	R_CONIO.H / R_CONIO.C
******************************************************************************/

#ifndef	R_CONIO_H_
#define	R_CONIO_H_

#ifdef __GNUC__
#include <sys/io.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
	extern int	R_RAWIO_CAPABLE;
	#if defined(__x86_64__) || defined(QT32)
		#define	inp(port)			(R_RAWIO_CAPABLE ? inb((unsigned short)(port)) : 0xFF)
		#define	outp(port,data)		if (R_RAWIO_CAPABLE) outb((unsigned char)(data),(unsigned short)(port))
		#define	inpw(port)			(R_RAWIO_CAPABLE ? inw((unsigned short)(port)) : 0xFFFF)
		#define	outpw(port,data)	if (R_RAWIO_CAPABLE) outw((unsigned short)(data),(unsigned short)(port))
		#define	inpd(port)			(R_RAWIO_CAPABLE ? inl((unsigned short)(port)) : 0xFFFFFFFF)
		#define	outpd(port,data)	if (R_RAWIO_CAPABLE) outl(data,(unsigned short)(port))
	#else
		#define	inp(port)			inb((unsigned short)(port))
		#define	outp(port,data)		outb((unsigned char)(data),(unsigned short)(port))
		#define	inpw(port)			inw((unsigned short)(port))
		#define	outpw(port,data)	outw((unsigned short)(data),(unsigned short)(port))
		#define	inpd(port)			inl((unsigned short)(port))
		#define	outpd(port,data)	outl(data,(unsigned short)(port))
	#endif
	int	kbhit(void);
	int	getch(void);
	int	getche(void);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_CONIO_H_
