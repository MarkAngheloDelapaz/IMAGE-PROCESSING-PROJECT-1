/******************************************************************************
	R_STDLIB.H
	
	ローム株式会社　生産システム開発部　要素技術開発グループ
	
	変更履歴
	Ver 0.0		1993/04/12
	Ver 3.201	1999/12/17	Color_Addressの追加								by ohshima
	Ver 3.25	2000/06/21	エラー処理関連の変更							by ohshima
	Ver 3.36	2003/04/29	R_POINT,R_DPOINT,R_RECT,R_BOX構造体追加			by matsu
	Ver 3.39	2003/06/24	R_winb追加										by hirano
	Ver 3.40	2003/09/12	Gray_address,Bin_addressのページ数を100に拡張	by sassa
	Ver 3.41	2003/12/12	R_time_read_ms,R_time_read_sec対応				by sassa
				2003/01/10	R_LINE構造体追加								by hirano
	Ver 3.42	2004/04/22	UCHAR,USHORT,UINT,ULONG宣言追加					by sassa
				2004/04/28	未使用の変数initiator_bufferを削除				by sassa
				2004/05/07	Gray_address、Bin_addressの定義を変更			by araki
	Ver 3.44	2004/10/12	SSE_ID,SSE2_ID,SSE3_ID追加						by araki
	Ver 3.44	2004/10/13	RdtscStdTime変数などを廃止						by araki
	Ver 3.45	2005/03/25	R_stdlib_get_version_string関数追加				by araki
	Ver 3.46	2005/08/18	内部変数CheckRegistOverForDiv～をR_time.cへ移動	by araki
	Ver 3.46	2005/08/18	outpd、inpdを削除（標準ライブラリを使用）		by araki
	Ver 3.47	2006/02/10	型名宣言を追加(R_TYPE_～)						by matsu
	Ver 3.48	2006/03/22	R_TYPE_POINT,R_TYPE_DPOINT,R_TYPE_RECT,R_TYPE_BOX,R_TYPE_LINE追加	by matsu
	Ver 3.49	2006/10/18	VIDEO_CARD_MAXの初期値を８から４へ変更			by araki
	Ver 3.49	2006/10/24	r_get_cpuid,r_get_rdtscプロトタイプ宣言削除		by araki
	Ver 3.50	2008/02/19	VIDEO_PAGE_MAX	100->1000へ						by saito
	Ver 3.50	2008/05/07	R_MPOINT構造体追加								by nakahara
	Ver 3.50	2008/07/24	r_stdlib_version関数追加						by araki
	Ver 3.50	2008/08/19	r_stdlib_isa_is_supported関数追加				by araki
	Ver 3.50	2009/01/29	NULL宣言変更									by araki
	Ver 3.50	2009/06/16	VIDEO_CARD_MAXの初期値を４から８へ変更			by araki
	Ver 4.01	2009/08/07	R_ARBRECT構造体追加								by nakahara
	Ver 4.01	2010/07/07	R_stdlib_init関数追加							by araki
	Ver 4.01	2013/05/16	R_DLINE追加										by komatsu
******************************************************************************/

#ifndef	R_STDLIB_H_
#define	R_STDLIB_H_


#include "R_info.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

int	R_stdlib_init(int argc, char* argv[]);
void	R_stdlib_exit(void);

int	c_main(int argc, char* argv[]);

#ifdef MAIN_FILE
	int ROHM(int argc, char* argv[]);
#endif

#ifdef __x86_64__
#define UCHAR  uint8_t
#define USHORT uint16_t
#define UINT   uint32_t
#define ULONG  uint32_t
#else
#define UCHAR  unsigned char
#define USHORT unsigned short
#define UINT   unsigned int
#define ULONG  unsigned long
#endif

struct R_DWORDREGS {
	unsigned int	a;
};

struct R_WORDREGS {
	unsigned short	l;
};

struct R_BYTEREGS {
	unsigned char	l, h;
};

union R_REGS {
	struct R_DWORDREGS	d;
	struct R_WORDREGS	w;
	struct R_BYTEREGS	b;
};

#ifdef __x86_64__
#include <stdint.h>
union R_POINTER {
	uint8_t*	b;
	uint16_t*	w;
	uint32_t*	l;
};
#else
union R_POINTER {
	unsigned char*	b;
	unsigned short*	w;
	unsigned long*	l;
};
#endif

//　６４ビット数値用構造体
#ifndef	DLONG_DEF
#define	DLONG_DEF
typedef struct {
	unsigned long	lo;
	unsigned long	hi;
} DLONG;
#endif

// POINT構造体
// int形式
typedef struct {
	int x;
	int y;
} R_POINT;

#define	R_point_is_inside_rect(point,rect)		(((rect)->left<=(point)->x)&&((point)->x<=(rect)->right)&&((rect)->top<=(point)->y)&&((point)->y<=(rect)->bottom))

// double形式
typedef struct {
	double x;
	double y;
} R_DPOINT;

// RECT構造体
typedef struct {
	int left;
	int top;
	int right;
	int bottom;
} R_RECT;

// BOX構造体
typedef struct {
	int xs;
	int ys;
	int xsize;
	int ysize;
} R_BOX;

// LINE構造体
// 2017.04.08	komatsu		R_LINE構造体が5M(2500x2000に対応できないためcをint64_tに拡張)
typedef struct {
	int a;
	int b;
	//int c;
	int64_t c;
} R_LINE;

// DLINE構造体
// 直線の式は a*x+b*y+c=0 を使用すること
typedef struct {
	double a;
	double b;
	double c;
} R_DLINE;

// MPOINT構造体(点群)
typedef struct {
	R_POINT *point;
	int max_number;
	int cur_number;
	int memsize;
} R_MPOINT;

//2009.08.07 by nakahara added
typedef struct {			//任意長方形構造体
	int			xsize;
	int			ysize;
	int			angle;
	R_POINT		center;
} R_ARBRECT;

// R_FLOAT3構造体 点やベクトル、色などを表す
typedef struct{
	float x;
	float y;
	float z;
}R_FLOAT3;


#define R_TYPE_INT			0	// an integral number (decimal number)
#define R_TYPE_CHAR			1	// character type
#define R_TYPE_DOUBLE		2	// double (decimal fraction format)
#define R_TYPE_HEX			3	// an integral number (hexadecimal number)
#define R_TYPE_STRING		4	// character string 
#define R_TYPE_SHORT		5	// short int (decimal number)
#define R_TYPE_SHORT_HEX	6	// short int (hexadecimal number)
#define R_TYPE_POINT		7	// R_POINT struct		2006.03.22 by matsu
#define R_TYPE_DPOINT		8	// R_DPOINT struct		"
#define R_TYPE_RECT			9	// R_RECT struct		"
#define R_TYPE_BOX			10	// R_BOX struct			"
#define R_TYPE_LINE			11	// R_LINE struct		"
#define R_TYPE_LONGLONG		12	// long long number		2016.11.14 by tanaka

#define	VIDEO_PAGE_MAX	1000	// 2008/02/19 by saito	100->1000
#define	VIDEO_CARD_MAX	8		// 2006/10/18 by araki
#define	VIDEO_CARD_0	-1		// 2004/05/07 by araki
#define	VIDEO_CARD_1	-2		// 2004/05/07 by araki
#define	VIDEO_CARD_2	-3		// 2004/05/07 by araki
#define	VIDEO_CARD_3	-4		// 2004/05/07 by araki
#define	VIDEO_CARD_4	-5		// 2004/05/07 by araki
#define	VIDEO_CARD_5	-6		// 2004/05/07 by araki
#define	VIDEO_CARD_6	-7		// 2004/05/07 by araki
#define	VIDEO_CARD_7	-8		// 2004/05/07 by araki

#ifdef	MAIN_FILE
//	unsigned char	*Gray_address[100];		// 2004/05/07 by araki
//	unsigned char	*Bin_address[100];		// 2004/05/07 by araki
	
	unsigned char*	pGrayAddress[VIDEO_PAGE_MAX + VIDEO_CARD_MAX * 2];	// 2015/04/06
	unsigned char**	Gray_address = &(pGrayAddress[VIDEO_CARD_MAX]);		// 2004/05/07 by araki
	unsigned char*	pBinAddress[VIDEO_PAGE_MAX + VIDEO_CARD_MAX];		// 2004/05/07 by araki
	unsigned char**	Bin_address = &(pBinAddress[VIDEO_CARD_MAX]);		// 2004/05/07 by araki
	
	int	boot_title=1;
	unsigned char	*BoardAddress;
	int initiator_destination_page=1;
	unsigned long *Color_address[10];
	int bEnableToPrintOutOnVGA=0;			//00/06/21 ohshima
	int bEnableToPrintOutOnBinaryPlane=0;	//00/06/21 ohshima
	int nLastError=0;						//00/06/21 ohshima
#else
//	extern unsigned char	*Gray_address[100];		// 2004/05/07 by araki
//	extern unsigned char	*Bin_address[100];		// 2004/05/07 by araki
	
	extern unsigned char**	Gray_address;			// 2004/05/07 by araki
	extern unsigned char**	Bin_address;			// 2004/05/07 by araki
	
	extern int	boot_title;
	extern unsigned char *BoardAddress;
	extern int initiator_destination_page;
	extern unsigned long *Color_address[10];
	extern int bEnableToPrintOutOnVGA;			//00/06/21 ohshima
	extern int bEnableToPrintOutOnBinaryPlane;	//00/06/21 ohshima
	extern int nLastError;						//00/06/21 ohshima
#endif

/******************************************************************************
	関数名：	Ｒ＿ｓｔｄｌｉｂ＿ｇｅｔ＿ｖｅｒｓｉｏｎ＿ｓｔｒｉｎｇ
	機　能：	９３０１ベースライブラリのバージョン表示用文字列を取得する。
	形　式：	#include <R_stdlib.h>
				const char* R_stdlib_get_version_string(void)
	引き数：	なし
	
	戻り値：	文字列へのポインタ
	
	解　説：	リンクしている９３０１ベースライブラリのバージョンを表示したいときに使用します。
				バージョンが3.45ならば、"3.45"という文字列へのポインタが返されます。
	
	留意事項：	なし
	
	例：		printf("ライブラリのバージョンは %s です。\n", R_stdlib_get_version_string());
				このプログラムを実行すると、標準出力に以下のように出力されます。
				
				ライブラリのバージョンは 3.45 です。
				
******************************************************************************/
const char*	R_stdlib_get_version_string(void);

int			r_stdlib_version(void);
int			r_stdlib_isa_is_supported(void);
int			r_stdlib_multi_process_is_enabled(void);
int			r_stdlib_dos_extender_is_dos32a(void);

#ifdef QT32
void	r_stdlib_standard_user_interface_enable(void);
void	r_stdlib_standard_user_interface_disable(void);
int		r_stdlib_standard_user_interface_is_enabled(void);
#else
#define	r_stdlib_standard_user_interface_is_enabled()	(!0)
#endif

/**
*	@brief	メインウィンドウの非表示
*	@param	なし
*	@return
*			OK				正常終了
*			OK以外			異常終了
*/
int R_stdlib_hide_main_window(void);

#define R_get_last_error(void)	nLastError
#define R_set_last_error(nErrorCode)	nLastError=nErrorCode
#define R_set_printout_message_vga(void)	bEnableToPrintOutOnVGA=1
#define R_set_printout_message_binaryplane(void)	bEnableToPrintOutOnBinaryPlane=1
#define R_reset_printout_message_vga(void)	bEnableToPrintOutOnVGA=0
#define R_reset_printout_message_binaryplane(void)	bEnableToPrintOutOnBinaryPlane=0

#ifdef	MAIN_FILE
	int		CPUIDsupport=0;
	char	VendorId[13]={0};
	int		HighestInputValue=0;
	int		FamilyId=0;
	int		ModelId=0;
	int		SteppingId=0;
	int		FPUOnChip=0;
	int		MachineCheckException=0;
	int		CMPXCHG8BInstruction=0;
	int		MMX_ID=0;
	int		SSE_ID=0;
	int		SSE2_ID=0;
#else
	extern int	CPUIDsupport;
	extern char	VendorId[13];
	extern int	HighestInputValue;
	extern int	FamilyId;
	extern int	ModelId;
	extern int	SteppingId;
	extern int	FPUOnChip;
	extern int	MachineCheckException;
	extern int	CMPXCHG8BInstruction;
	extern int	MMX_ID;
	extern int	SSE_ID;
	extern int	SSE2_ID;
#endif

#ifndef NULL
#define		NULL	0
#endif

#define		OK		0
#define		ERROR	(-1)
#define		ERROR2	(-2)
#define		ERROR3	(-3)
#define		ERROR4	(-4)
#define		ERROR5	(-5)
#define		ERROR6	(-6)
#define		ERROR7	(-7)
#define		ERROR8	(-8)
#define		ERROR9	(-9)
#define		YES		1
#define		NO		0

#define		GRAY_ADDRESS	Gray_address[0]
//#define		BIN_ADDRESS		(BoardAddress+0x40000)
#ifndef __x86_64__
#define		BIN_ADDRESS		Bin_address[0]
#endif
#define		ROM_ADDRESS		(BoardAddress+0x60000)
#define		COLOR_ADDRESS	Color_address[0]

extern		unsigned char*	IMAGE_ADDRESS;

#define	BIN_BX	64L					/* ビデオメモリ横方向バイト数	*/
#define	LONG_ACCESS	32
#define	LONG_WORD	4
#define	BIN_MSB		0x80000000
#define	BIN_LSB		0x00000001
#define BIT_OF_MEMORY   8


union R_ADR {
	unsigned int	*d;
	unsigned short	*w;
	unsigned char	*b;
};

int isdpmi(void);
int isvcpi(void);
int used(short intno);
long get_vector_address(short intno);

//	r_gfmb.c
int R_get_free_memory_block (int block_size) ;
int R_print_free_memory_block_counts (int block_size) ;

//	r_stime.asm
int	set_count(void);
int	get_count(void);
int	restart_count(void);

//	file time struct
#define	YEAR(t)		((( t & 0xfe00 ) >> 9 )+1980 )
#define	MONTH(t)	(( t & 0x01e0 ) >> 5 )
#define	DAY(t)		( t & 0x001f )
#define	HOUR(t)		(( t & 0xf800 ) >> 11 )
#define	MINUTE(t)	(( t & 0x07e0 ) >> 5 )
#define	SECOND(t)	(( t & 0x001f ) << 1 )

// r_eeprom.c
int R_read_EEPROM ( int address ) ;
int R_write_EEPROM ( int address, unsigned short int data );




//r_winb.c
#define JPN_DISP 1
#define ENG_DISP 0
#ifdef	R_WINB
int	R_language = JPN_DISP;
#else
extern int R_language;
#endif
#define R_set_language(mode)	R_language = mode
#define R_get_language()		R_language
int R_toggle_language(void);

#ifdef __GNUC__
	#ifndef __cplusplus
		#define	max(a,b)	(((a) > (b)) ? (a) : (b))	// ANSI C
		#define	min(a,b)	(((a) < (b)) ? (a) : (b))	// ANSI C
	#endif
#endif

#define	_FUNC_IS_NOT_SUPPORTED()	r_info_func_is_not_supported(__FUNCTION__)

#ifdef __x86_64__
void R_stdlib_textout_callback(int (*func)(const char*));
#endif

#ifdef __x86_64__
int	r_boot_print(char* format, ...);
#else
#define r_boot_print	printf
#endif

//パス区切り文字		<4.01b125>	komatsu
#ifdef __DOS__
	#define PATH_SEPARATOR	"\\"
#elif __GNUC__
	#define PATH_SEPARATOR	"/"
#endif

/*!
 * @brief	ディレクトリの削除
 *  ディレクトリ内のファイルとサブディレクトリも削除します。
 * @param	path	ディレクトリ名
 * @retval	OK		正常終了
 * @retval	OK以外	異常終了
 */
int	R_stdlib_rmdir(const char* path);

#ifdef __cplusplus
}
#endif

#endif	// R_STDLIB_H_
