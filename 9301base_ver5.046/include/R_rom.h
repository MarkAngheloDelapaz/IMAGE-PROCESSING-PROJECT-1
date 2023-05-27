/************************************************

 フラッシュメモリ書き込み(HITACHI HN28F101series)

　ヘッダファイル

	Ver 3.50	2009/02/12	DOS & LINUX ソース統合

************************************************/

#ifndef	R_ROM_H_
#define	R_ROM_H_

#define	FLASH_READ_ID_CMD	0x90
#define	PROGRAM_CMD			0x40
#define	VERIFY_CMD			0xc0
#define	AUTO_ERASE_CMD		0x30

int		R_write_flash( char * );
int 	R_read_romid( void );
void	flash_vpp( int );

#ifdef FLASH_ROM
	int	ERR_FLASH_ADDRESS;
	char	ERR_FLASH_DATA;
	char	ERR_FLASH_TRUE_DATA;
#else
	extern int	ERR_FLASH_ADDRESS;
	extern char	ERR_FLASH_DATA;
	extern char	ERR_FLASH_TRUE_DATA;
#endif

#endif	// R_ROM_H_
