/******************************************************************************
	R_COCY32.H / R_COCY32.C
	
	�ύX����
	Ver 3.00	1998/02/11	���������[�X
	Ver 3.00	1998/06/16	RCocy32CaptureEx��ǉ�
							���E���]��荞�݂��T�|�[�g
	Ver 3.00	1999/01/25	NED 2048B�Ή�
	Ver 3.04	1999/02/15	I/O�֐��ǉ�
	Ver 3.47	2005/09/12	�����֐�get_level��static�֐��ɕύX
	Ver 3.50	2007/08/30	�����֐�get_level�̈����錾���C��
******************************************************************************/

#ifndef __R_COCY32_H__
#define __R_COCY32_H__

extern int	COCY32;

#define	LINE_SIZE_2K	2048
#define	LINE_SIZE_4K	4096
#define	LINE_SIZE_5K	5120

#define	LINE_CAPTURE_MIRROR	0x00000001

extern int	Cocy32SystemPort;
extern int	Cocy32RegisterPort;
extern int	Cocy32IRQ;

int		RCocy32Open(int i, int j);
int		RCocy32Exist(void);
int		RCocy32Init(int line_size);
int		RCocy32PutcPROM(int adr, int data);
int		RCocy32GetcPROM(int adr);

int		RCocy32SetDACData(int ch, int data);
int		RCocy32PutDACData(int ch, int data);
int		RCocy32GetDACData(int ch);

int		RCocy32Capture(unsigned char* adr, int line);
int		RCocy32CaptureEx(unsigned char* adr, int line, int status);
int		RCocy32CaptureSunx(unsigned char* adr, int line);

int		RCocy32GetScanLine(void);
int		RCocy32SetScanLine(int line);

int		RCocy32GetScanClock(void);
int		RCocy32SetScanClock(int clock);

int		RCocy32GetLineSize(void);

unsigned char*	RCocy32GetOffsetAdr(void);
unsigned char*	RCocy32GetSpanAdr(void);

int		RCocy32Shading(void* pdata, void* poffset, void* pspan, int count);

void	RCocy32OutputJudge(int nJudge);
void	RCocy32OutputExternalIO(int nValue);
int		RCocy32InputExternalIO(void);
int		RCocy32CheckExternalIOStart(void);

int		RCocy32CheckBoard(void);

#endif
