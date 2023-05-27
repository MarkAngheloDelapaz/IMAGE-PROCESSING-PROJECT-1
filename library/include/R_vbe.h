/******************************************************************************
	R_vbe.h / R_vbe.cpp
	
	�ύX����
	Ver 5.004	2018.05.24	�b��\�[�X
******************************************************************************/

#ifndef R_VBE_H_
#define R_VBE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef	struct {					// ====== SVGA���u���b�N ======
	char	VESASignature[4];		// VESA���ʎq
 	short	VESAVersion;			// �r�f�I�J�[�h��VESA�o�[�W����
 	char	*OEMStringPtr;			// �������[�J������̃|�C���^�[
	char	Capabilities[4];		// �r�f�I����
	unsigned short *VideoModePtr;	// �T�|�[�gVESA�r�f�I���[�h�̔z��̃|�C���^�[
	short	TotalMemory;			// �{�[�h��ɂ���64kbyte�������[�u���b�N��
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
typedef struct {					// ===== VESA�g�����[�h��� =====
// �������
	short	ModeAttributes;			// �g�����[�h�T�|�[�g��
	char	WinAAttributes;			// �E�B���h�E�P����
	char	WinBAttributes;			// �E�B���h�E�Q����
	short	WinGranularity;			// �E�B���h�E�ʒu�w��P�ʁi�O���j�������e�B�[�j
	short	WinSize;				// �E�B���h�E�T�C�Y
	short	WinASegment;			// �E�B���h�E�P�J�n�Z�O�����g
	short	WinBSegment;			// �E�B���h�E�Q�J�n�Z�O�����g
	uint32_t	WinFuncPtr;			// �E�B���h�E�ؑփt�@���N�V�����w��16bit far pointer
	short	BytesPerScanLine;		// �X�L�������C���P�s������̃o�C�g��
// �ȑO�̃I�v�V�������i�������j
	short	XResolution;			// �����𑜓x
	short	YResolution;			// �����𑜓x
	char	XCharSize;				// �����{�b�N�X�̕�
	char	YCharSize;				// �����{�b�N�X�̍���
	char	NumberOfPlanes;			// �������[�v���[����
	char	BitsPerPixel;			// �s�N�Z��������̃r�b�g��
	char	NumberOfBanks;			// VRAM�o���N��
	char	MemoryModel;			// VRAM���������f��
	char	BankSize;				// VRAM�o���N��
	char	NumberOfImagePages;		// �y�[�W��
	char	RsvdPageFunction;		// �y�[�W�t�@���N�V�����̂��߂̃��U�[�u
// �V�����_�C���N�g�J���[�t�B�[���h
	char	RedMaskSize;			// �_�C���N�g�J���[�ԃ}�X�N�̃T�C�Y�i�r�b�g�P�ʁj
	char	RedFieldPosition;		// �ԃ}�X�N�̍ŉ��ʃr�b�g�̃r�b�g�ʒu
	char	GreenMaskSize;			// �_�C���N�g�J���[�΃}�X�N�̃T�C�Y�i�r�b�g�P�ʁj
	char	GreenFieldPosition;		// �΃}�X�N�̍ŉ��ʃr�b�g�̃r�b�g�ʒu
	char	BlueMaskSize;			// �_�C���N�g�J���[�}�X�N�̃T�C�Y�i�r�b�g�P�ʁj
	char	BlueFieldPosition;		// �}�X�N�̍ŉ��ʃr�b�g�̃r�b�g�ʒu
	char	RsvdMaskSize;			// ���U�[�u���ꂽ�_�C���N�g�J���[�}�X�N�̃T�C�Y�i�r�b�g�P�ʁj
	char	RsvdFieldPosition;		// ���U�[�u���ꂽ�}�X�N�̍ŉ��ʃr�b�g�̃r�b�g�ʒu
	char	DirectColorModeInfo;	// �_�C���N�g�J���[�̃A�g���r���[�g
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
