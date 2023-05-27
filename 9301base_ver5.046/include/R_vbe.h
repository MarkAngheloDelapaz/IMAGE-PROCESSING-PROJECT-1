/******************************************************************************
	R_vbe.h / R_vbe.cpp
	
	変更履歴
	Ver 5.004	2018.05.24	暫定ソース
******************************************************************************/

#ifndef R_VBE_H_
#define R_VBE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef	struct {					// ====== SVGA情報ブロック ======
	char	VESASignature[4];		// VESA識別子
 	short	VESAVersion;			// ビデオカードのVESAバージョン
 	char	*OEMStringPtr;			// 供給メーカ文字列のポインター
	char	Capabilities[4];		// ビデオ環境の
	unsigned short *VideoModePtr;	// サポートVESAビデオモードの配列のポインター
	short	TotalMemory;			// ボード上にある64kbyteメモリーブロック数
// Added for VBE2.0+
	uint16_t	OemSoftwareRev;			// VBE implementation Software revision
	uint32_t	OemVendorNamePtr;		// VbeFarPtr to Vendor Name String
	uint32_t	OemProductNamePtr;		// VbeFarPtr to Product Name String
	uint32_t	OemProductRevPtr;		// VbeFarPtr to Product Revision String
	char	Reserved[222];			// Reserved for VBE implementation scratch area
	char	OemData[256];			// Data Area for OEM Strings
} SVGAINFO;
#pragma pack()

#pragma pack(1)
typedef struct {					// ===== VESA拡張モード情報 =====
// 強制情報
	short	ModeAttributes;			// 拡張モードサポート状況
	char	WinAAttributes;			// ウィンドウ１属性
	char	WinBAttributes;			// ウィンドウ２属性
	short	WinGranularity;			// ウィンドウ位置指定単位（グラニュラリティー）
	short	WinSize;				// ウィンドウサイズ
	short	WinASegment;			// ウィンドウ１開始セグメント
	short	WinBSegment;			// ウィンドウ２開始セグメント
	uint32_t	WinFuncPtr;			// ウィンドウ切替ファンクションヘの16bit far pointer
	short	BytesPerScanLine;		// スキャンライン１行当たりのバイト数
// 以前のオプション情報（強制情報）
	short	XResolution;			// 水平解像度
	short	YResolution;			// 垂直解像度
	char	XCharSize;				// 文字ボックスの幅
	char	YCharSize;				// 文字ボックスの高さ
	char	NumberOfPlanes;			// メモリープレーン数
	char	BitsPerPixel;			// ピクセルあたりのビット数
	char	NumberOfBanks;			// VRAMバンク数
	char	MemoryModel;			// VRAMメモリモデル
	char	BankSize;				// VRAMバンク数
	char	NumberOfImagePages;		// ページ数
	char	RsvdPageFunction;		// ページファンクションのためのリザーブ
// 新しいダイレクトカラーフィールド
	char	RedMaskSize;			// ダイレクトカラー赤マスクのサイズ（ビット単位）
	char	RedFieldPosition;		// 赤マスクの最下位ビットのビット位置
	char	GreenMaskSize;			// ダイレクトカラー緑マスクのサイズ（ビット単位）
	char	GreenFieldPosition;		// 緑マスクの最下位ビットのビット位置
	char	BlueMaskSize;			// ダイレクトカラー青マスクのサイズ（ビット単位）
	char	BlueFieldPosition;		// 青マスクの最下位ビットのビット位置
	char	RsvdMaskSize;			// リザーブされたダイレクトカラーマスクのサイズ（ビット単位）
	char	RsvdFieldPosition;		// リザーブされたマスクの最下位ビットのビット位置
	char	DirectColorModeInfo;	// ダイレクトカラーのアトリビュート
// Mandatory infomation for VBE2.0 and above
	uint32_t	PhysBasePtr;		// physical address for flat memory frame buffer
	char	RsvdVbe20[6];			// Reserved - always set to 0
// Mandatory infomation for VBE3.0 and above
	uint16_t	LinBytesPerScanLine;	// bytes per scan line for linear modes
	uint8_t		BnkNumberOfImagePages;	// number of images for banked modes
	uint8_t		LinNumberOfImgePages;	// number of images for linear modes
	uint8_t		LinRedMaskSize;			// size of direct color red mask (linear modes)
	uint8_t		LinRedFieldPosition;	// bit position of lsb of red mask (linear modes)
	uint8_t		LinGreenMaskSize;		// size of direct color green mask (linear modes)
	uint8_t		LinGreenFieldPosition;	// bit position of lsb of green mask (linear modes)
	uint8_t		LinBlueMaskSize;		// size of direct color blue mask (linear modes)
	uint8_t		LinBlueFieldPosition;	// bit position of lsb of blue mask (linear modes)
	uint8_t		LinRsvdMaskSize;		// size of direct color reserved mask (linear modes)
	uint8_t		LinRsvdFieldPosition;	// bit position of lsb of reserved mask (linear modes)
	uint32_t	MaxPixelClock;			// maximum pixel clock (in Hz) for graphics mode
	
	char	Reserved[190];			// remainder of ModeInfoBlock
} VESAMODE;
#pragma pack()

int	r_vbe_svga_info(SVGAINFO* info);				// 4F00h
int	r_vbe_mode_info(int mode, VESAMODE* info);		// 4F01h
int	r_vbe_set_vbe_mode(int mode);					// 4F02h
int	r_vbe_set_color_palette(int index, int r, int g, int b);

void	r_vbe_init(void);
void*	r_vbe_linear_base_ptr(void);
const VESAMODE*	r_vbe_current_mode_info(void);

#ifdef __cplusplus
}
#endif

#endif	// R_VBE_H_
