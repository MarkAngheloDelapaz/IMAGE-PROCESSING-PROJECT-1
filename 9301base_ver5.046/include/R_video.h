/******************************************************************************
	
	＜ｒ＿ｖｉｄｅｏ．ｈ＞	
	
		Ｒｏｈｍ　生産システム開発部要素技術開発
	
		Rohm product sysytem deveropment
	
		ｆｏｒ　ＣＯ─ＣＺ─２３／ＣＯ─ＣＺ─２４／ＣＯ─ＣＹ─２５
	
	[History]
	Ver 0.0		1993/04/12	Ver 0.0
				1994/04/21	CO-CY-25(line sensor I/F)対応
				1995/03/10	R_get_video() DX4100の対応方法追加（DX266で未確認のため従来ソフトのまま）
				1995/07/31	CO-CZ-27 Support
				1995/10/01	ＣＯ─ＣＺ─２４対応 ※PULNIX TM6701には対応していない。
				1995/12/29	biosclkがFFFFから0になるタイミングでエラーが発生していたのを修正
				1996/01/24	R_get_video() で　ADD/EVENの指定の時のバクを修正
				1996/02/15	IK-542,IK-M41MFカメラ対応
				1998/03/11	サイズ自由化のため、変数追加 FxSize,MxSize
				1998/10/28	CO-CZ-33複数枚対応
				1999/02/23	CO-CZ-33 ｲﾝﾀﾚｰｽ ANY_FIELD取込時間設定可能
	Ver 3.20	1999/11/01	CO-CZ-35対応
				1999/11/10	FX_SIZEとFxSizeを統合(Ver3.10)
				2000/02/15	R_video_shutter、R_video_set_shutter_speed追加
	Ver 3.28	2000/09/27	CS8530D-01対応(CO-CZ-35)
				2000/11/01	XC-ST50ﾗﾝﾀﾞﾑｼｬｯﾀ(ｼﾝｸﾘｾｯﾄ)対応(CO-CZ-33)
				2000/11/17	R_channel CO-CZ-35対応
				2001/02/07	CO-CZ-33オートシーケンス取込対応
	Ver 3.31	2002/07/23	CO-CY-34 HIGH_SHUTTER対応
	Ver.3.33	2002/10/29	GRAY_PLANE、BIN_PLANE削除(多重定義)R_stdlib.hへ
	Ver 3.3x	2002/11/15	CS8550Di,CS8541D 対応
	Ver 3.33	2003/01/17	TimeUp()をR_time.cへ移動
	Ver 3.34	2003/01/09	CO-CZ-35オートシーケンス取込対応
	Ver 3.36	2003/05/21	R_video_set_initiator_page追加(暫定、ID未対応)
	Ver 3.36	2003/05/21	R_video_check_version_of_cocz33追加
	Ver 3.39	2003/06/23	R_video_set/get_initiator_page、ID対応
	Ver 3.39	2003/06/24	R_channelのモードにNO_CHECKを追加
	Ver 3.40	2003/06/26	R_video_binlevel CO-CZ-33/35複数枚対応バグ修正
	Ver 3.41	2003/12/05	CO-CZ-39暫定対応
	Ver 3.42	2003/03/18	CO-CZ-39暫定対応
	Ver 3.42	2003/03/23	ExternalI/O系関数をR_mainから移動
	Ver 3.42	2003/03/25	R_check_VS、R_check_DISPTをR_graphから移動
	Ver 3.42	2003/04/01	R_video_set/get_display_page追加
	Ver 3.42	2003/05/06	R_video_init_memory追加
	Ver 3.42	2003/05/07	R_video_get_card_id()追加
	Ver 3.44	2004/10/07	R_wait_VS、R_wait_dispt_wait、R_wait_disptをR_graphから移動
	Ver 3.45	2004/10/29	R_video_init_memory()COCY34のメモリサイズ変更
	Ver 3.45	2004/11/04	R_get_video_auto_shutter_end_check追加
	Ver 3.45	2004/11/06	R_check_status_sw/external_io_start,R_input_external_io修正
	Ver 3.45	2005/01/08	カラーカメラ用パラメータを追加
	Ver 3.45	2005/03/25	CO-CY-40,CO-CY-42対応
	Ver 3.46	2005/03/30	VM_BOARD_SYNCパラメータ追加
	Ver 3.47	2005/12/01	VIDEO_DISP_XGA/VGA追加
	Ver 3.48	2006/01/31	VM_1024_768_INT、VM_1024_768_EXT追加
	Ver 3.48	2006/02/01	XC-HR70パラメータ追加
	Ver 3.48	2006/03/09	R_video_load/save_adc_reference追加
	Ver 3.48	2006/03/15	IKTU50定義追加
	Ver 3.48	2006/03/24	R_change_video_mode、XC-HR70対応
	Ver 3.48	2006/05/08	R_video_check_version_of_cocz33 引き数修正
	Ver 3.48	2006/07/20	R_video_init_memory()解像度指定に対応
	Ver 3.49	2006/08/08	CO-CZ-43暫定対応
	Ver 3.49	2006/08/18	GV_NO_WENパラメータ追加
	Ver 3.49	2006/08/18	R_video_init追加
	Ver 3.49	2006/09/28	DRAW_PLANE_DEFAULT/OFFSET31追加
	Ver 3.49	2006/10/02	GV_EXT_TRIG追加
	Ver 3.49	2006/10/18	R_video_board_id/BOARD_ID_COCZxx追加
	Ver 3.49	2007/02/15	cocz33_t構造体をR_cocz33.hから移動
	Ver 3.49	2007/02/26	cocz33_t構造体にnCameraSpec追加
	Ver 3.49	2007/03/15	cocz33_t構造体にnBoardSpec,ulVramBaseAdr,ulVramByteSize追加
	Ver 3.50	2007/06/12	R_VIDEO_H_追加
	Ver 3.50	2007/09/05	CAM_KPF200PCLパラメータ追加
	Ver 3.50	2007/10/05	CAM_SU51/CAM_SU74パラメータ追加
	Ver 3.50	2007/10/11	CAM_TL5150UCL/CAM_TL7450UCLパラメータ追加
	Ver 3.50	2007/11/15	CAM_CM200MCLパラメータ追加
	Ver 3.50	2007/12/21	R_video_gpio_control追加
	Ver 3.50	2008/01/24	r_video_boards_max_size_is_512_480関数追加
	Ver 3.50	2008/03/17	CAM_PBZ2366パラメータ追加
	Ver 3.50	2008/03/18	cocz33_t構造体にnWaitVsyncLimit追加
	Ver 3.50	2008/03/24	ラインセンサカメラ用関数追加
	Ver 3.50	2008/03/26	cocz33_t構造体にnLinePeriod追加
	Ver 3.50	2008/03/27	内部関数r_video_pixel_clock追加
	Ver 3.50	2008/04/15	VR_1600_1200追加
	Ver 3.50	2008/05/08	DISP_WIN_ZOOM_BINNINGパラメータ追加
	Ver 3.50	2008/05/09	cocz33_t構造体にnVideoModeAssignment追加
	Ver 3.50	2008/06/02	int R_video_init_memory・CO-CY-45対応
	Ver 3.50	2008/06/17	CO-CZ-39時のR_check_VS・R_check_DISPT関数を変更
	Ver 3.50	2008/07/03	CAM_CM030PMCLRHパラメータ追加
	Ver 3.50	2008/07/09	cocz33_t構造体にnGain追加
	Ver 3.50	2008/07/14	CAM_SUI51/CAM_SUI74パラメータ追加
	Ver 3.50	2008/07/09	R_video_set/get_camera_gain関数追加
	Ver 3.50	2008/07/29	R_video_camera_is_present関数追加
	Ver 3.50	2008/07/30	R_video_init戻り値追加
	Ver 3.50	2008/09/02	R_change_video_card・ISAバスチェック追加
	Ver 3.50	2008/10/23	R_video_set/get_display_mode追加
	Ver 3.50	2008/11/04	キャプチャ時スルー表示自動切替改善
	Ver 3.50	2009/01/19	転送＆露光・オーバラップモード追加
	Ver 3.50	2009/02/26	R_video_set/get_trigger_input_delay関数追加
	Ver 3.50	2009/03/05	CAM_IKTF7Pパラメータ追加
	Ver 4.01	2010/06/14	R_video_init_memory、メモリサイズ計算オーバフロー対策
	Ver 3.50	2009/06/26	CAM_SUI51_12BITパラメータ追加
	Ver 4.01	2011/01/26	R_video_check_camera_vsync関数追加
	Ver 4.01	2011/02/03	CO-CZ-39、Linux暫定対応
	Ver 4.01	2011/03/15	R_video_dma_to_vram/end_check関数追加
	Ver 4.01	2011/06/29	R_set_line_page・CO-CZ-43対応
	Ver 4.01	2011/08/02	KP-FM200WCLパラメータ追加
	Ver 4.01	2011/09/16	cocz33_t構造体にnPixelClock追加
	Ver 4.01	2013/06/26	CAM_XCLC500パラメータ追加
	Ver 4.01	2013/07/31	CAM_ACA2040KCパラメータ追加
	Ver 4.01	2013/08/12	R_video_auto_bayer_color_interpolation関数追加
	Ver 4.01	2013/08/13	R_video_bayer_type関数追加
	Ver 4.01	2014/07/28	HORIZONTAL_MIRRORパラメータ追加
	Ver 4.01	2015/04/03	STC-MBCM200U3Vパラメータ追加
	Ver 4.01	2016/01/25	CAM_CM141MCLパラメータ追加
	Ver 4.01	2016/03/25	R_video_gpio_set_syncout_delay関数追加
	Ver 4.01	2016/04/25	CAM_CVM4PLUSパラメータ追加
	Ver 4.01	2016/11/11	VIDEO_DISP_RESO_DMT07H追加
	Ver 4.01	2017/04/27	VI_SHRINKED_RGB_1パラメータ追加
	Ver 4.01	2017/08/18	R_video_set_bayer_conv_mode関数追加
	Ver 5.005	2018/08/10	XCM6040SAT2とXCM8040SAT2でトリミング取込設定した時、
							取込画像がセンサ中央部分ではなく、センサ右端になっているバグを修正。
	Ver 5.005	2018/08/10	R_video_set/get_capture_start_pixel関数追加
	Ver 5.010	2019/02/04	CAM_STCSPB163PCLパラメータ追加
	Ver 5.015	2019/06/03	CAM_SUI7440パラメータ追加
	Ver 5.015	2019/06/03	CAM_BC160Mパラメータ追加
	Ver 5.021	2019/11/12	CAM_AP3200TPMCLパラメータ追加
	Ver 5.026	2020/02/27	CAM_SXCL5Rパラメータ追加
	Ver 5.029	2020/06/22	R_video_board_is_cocz43_compatible関数追加
	Ver 5.032	2020/07/07	R_video_use_cocz47_as_cocz43関数追加
	Ver 5.035	2020/08/18	CAM_GO5101MPMCLパラメータ追加
	Ver 5.036	2020/09/08	VI_BINNING_1X4、VI_BINNING_2X4追加
	Ver 5.040	2020/11/25	CAM_STCRBS43PCLパラメータ追加
	Ver 5.042	2021/03/24	CAM_LACM16K05Aパラメータ追加
	Ver 5.045	2021/06/22	CAM_ID2MBパラメータ追加
******************************************************************************/

#ifndef	R_VIDEO_H_
#define	R_VIDEO_H_

#include "R_stdio.h"
#include "R_stdlib.h"
#include "R_time.h"
#include "R_plane.h"

#ifdef __GNUC__
#include <stdint.h>
#include <pthread.h>
#include "R_memmgr.h"
#endif

#include "R_bayer.h"

#ifdef __cplusplus
extern "C" {
#endif

// video mode
#define	VM_DEFAULT			0xFE

#define	VM_512_480_INT		0x00
#define	VM_512_480_EXT		0x01
#define	VM_512_240_INT		0x04	// CO-CZ-24 or later
#define	VM_512_240_EXT		0x05	// CO-CZ-24 or later
#define	VM_256_240_INT		0x06
#define	VM_256_240_EXT		0x07

#define	VM_640_480_INT		0x08	// CO-CZ-35 or later
#define	VM_640_480_EXT		0x09	// CO-CZ-35 or later
#define	VM_640_240_INT		0x0C	// CO-CZ-35 or later
#define	VM_640_240_EXT		0x0D	// CO-CZ-35 or later
#define	VM_320_240_INT		0x0E	// CO-CZ-35 or later
#define	VM_320_240_EXT		0x0F	// CO-CZ-35 or later

#define	VM_1024_768_INT		0x10	// CO-CZ-39H or CO-CZ-43
#define	VM_1024_768_EXT		0x11	// CO-CZ-39H or CO-CZ-43

#define	VM_1280_1024_INT	0x20	// CO-CZ-43
#define	VM_1280_1024_EXT	0x21	// CO-CZ-43

#define	VM_1600_1200_INT	0x30	// CO-CZ-43
#define	VM_1600_1200_EXT	0x31	// CO-CZ-43

#define	VM_2048_1088_EXT	0x80	
#define	VM_1920_1080_EXT	0x81	

#define	VM_PROGRESSIVE	0x8000	// CO-CZ-35 or later
#define	VM_DOUBLE_SPEED	0x4000	// CO-CZ-35 or later
#define	VM_BOARD_SYNC	0x2000	// CO-CZ-35 or later
#define	VM_BINNING		0x10000	// CO-CZ-43 & XCL-5005 only

#define	VM_SCALABLE_BIT		0x80000000
#define	VM_(width,height)	(VM_SCALABLE_BIT|(height<<16)|width)	// CO-CZ-43

#define	NORMAL	0
#define	STANDBY	1

#define X_SIZE	512		// メモリーの水平画素数
#define Y_SIZE  480		// メモリーの垂直画素数

// resolution
#define VR_DEFAULT		0x00000000		// 画像ボードの標準解像度
#define VR_512_480		0x0003C000		// 解像度  512 X  480
#define VR_640_480		0x0004B000		// 解像度  640 X  480
#define VR_1024_768		0x000C0000		// 解像度 1024 X  768
#define VR_1600_1200	0x001D4C00		// 解像度 1600 X 1200	2008.04.15  by matsu

#define RESO_DEFAULT	VR_DEFAULT		// 画像ボードの標準解像度(旧)

extern long int	Video_mode;
extern long int	Video_out_mode;
#ifndef __x86_64
extern int		FxSize, FySize;
extern int		MxSize, MySize;
#endif
extern int		Camera_mode;
extern int		DisplayMemoryEnable;
extern int		CurrentVideoCardID;

#define FX_SIZE 	FxSize
#define FY_SIZE 	FySize

// R_get_video or R_video_init
#define	ANY_LINE		0
#define	ANY_FIELD		1
#define	ODD_FIELD		2
#define	EVEN_FIELD		3
#define	SHUTTER			4
#define	HIGH_SHUTTER	0x00200004
#define	F_SHUTTER		6
#define	OVERLAPPING_SHUTTER		0xC
#define	PARTIAL_300		((int)(0x00008000 | 300))	// XC-HR50 only
#define	PARTIAL_340		((int)(0x00008000 | 340))	// XC-HR50 only
#define	PARTIAL_480		((int)(0x00008000 | 480))	// XC-HR50 only

// Camera Type
// Analog I/F B/W Camera
#define	XC75			0x00000000
#define	TI124A			0x01000000
#define	CMA75			0x02000000
#define	IKM41MF			0x03000000
#define	IK542			0x04000000
#define	XC7500			0x05000000
#define	TI150A			0x06000000
#define	CS8531			0x07000000		// CS8531Z-02 (Teli)
#define	CS8530			0x08000000		// CS8530D-01 (Teli)
#define	XCST50			0x09000000		// XC-ST50 (SONY)
#define	CS8550D			0x0A000000		// CS8550D (Teli)
#define	CS8550Di		CS8550D			// CS8550Di (Teli)
#define	CS8541D			CS8550D			// CS8541D (Teli) 0x0b000000 reserve
#define	XCHR50			0x0C000000		// XC-HR50 (SONY)
#define	CS8541			0x0D000000		// CS8541 Normal Speed Interlace camera
#define	XCHR70			0x0E000000		// XC-HR70 (SONY)

// R_video_init
// Analog I/F Camera
#define	CAM_XCHR50			0x2C000000		// SONY XC-HR50						(B/W VGA 60fps)
// PoCL I/F Camera
#define	CAM_G22V31PCL		0x40000000		// CIS VCC-G22V31PCL				(B/W VGA 120fps)
#define	CAM_G21V31PCL		0x41000000		// CIS VCC-G21V31PCL				(B/W VGA 60fps)
#define	CAM_KPF30PCL		0x42000000		// HITACHI KOKUSAI DENKI KP-F30PCL	(B/W VGA 60fps)
#define	CAM_KPF80PCL		0x43000000		// HITACHI KOKUSAI DENKI KP-F80PCL	(B/W XGA 36fps)
#define	CAM_G22U21PCL		0x44000000		// CIS VCC-G22U21PCL				(B/W UXGA 20fps)
#define	CAM_G32U21PCL		0x45000000		// CIS VCC-G32U21PCL				(B/W UXGA 30fps)
#define	CAM_KPF31PCL		0x46000000		// HITACHI KOKUSAI DENKI KP-F31PCL	(B/W VGA 120fps)
#define	CAM_KPF230PCL		0x47000000		// HITACHI KOKUSAI DENKI KP-F230PCL	(B/W UXGA 30fps)
#define	CAM_CSB4000CL		0x48000000		// TELI CSB4000CL-10A				(B/W 4Mpixel 7.29fps)
#define	CAM_CS8581QCL		0x49000000		// TELI CS8581QCL					(B/W VGA 133fps)
#define	CAM_KPF200PCL		0x4A000000		// HITACHI KOKUSAI DENKI KP-F200PCL	(B/W UXGA 15fps)
#define	CAM_CM200MCL		0x4B000000		// JAI CM-200MCL					(B/W UXGA 25fps)
#define	CAM_PBZ2366			0x4C000000		// X-ray sensor
#define	CAM_CSCU15BC18		0x4D000000		// TELI CSCU15BC18					(B/W UXGA 15fps)
#define	CAM_CSCS20BC2		0x4E000000		// TELI CSCS20BC2					(B/W SXGA 20fps)
#define	CAM_CM030PMCLRH		0x4F000000		// JAI CM-030PMCL-RH				(B/W VGA 120fps)
#define	CAM_XCL5005			0x50000000		// SONY XCL-5005					(B/W 5Mpixel 15fps)
#define	CAM_KPF500WCL		0x51000000		// HITACHI KOKUSAI DENKI KP-F500WCL	(B/W 5Mpixel 16fps)
#define	CAM_CSCQS15BC23		0x52000000		// TELI CSCQS15BC23					(B/W 5Mpixel 15fps)
#define	CAM_CSCLV90BC3		0x53000000		// TELI CSCLV90BC3 Series			(B/W VGA 90fps)
#define	CAM_GC20V41PCL		0x54000000		// CIS VCC-GC20V41PCL				(B/W VGA 500fps CMOS)
#define	CAM_G21X31APCL		0x55000000		// CIS VCC-G21X31APCL				(B/W XGA 36fps)
#define	CAM_G21V21RM		0x56000000		// CIS VCC-G21V21RM					(B/W VGA 60fps 1/2"CCD)
#define	CAM_GC20U11PCL		0x57000000		// CIS VCC-GC20U11PCL				(B/W FullHD 70fps CMOS)
#define	CAM_KPFM200WCL		0x58000000		// HitachiKokusaiDenki KP-FM200WCL	(B/W FullHD 70fps CMOS)
#define	CAM_AM800CL			0x59000000		// JAI AM-800CL						(B/W 8Mpixel 17fps)
#define	CAM_AM800CL_12BIT	0x59010000		// JAI AM-800CL (12bit mode)		(B/W 8Mpixel 17fps)
#define	CAM_ACA2000KM		0x5A000000		// BASLER acA2000-340km				(B/W FullHD 70fps CMOS)
#define	CAM_ACA2000			0x5A000000		// BASLER acA2000-340km				(B/W FullHD 70fps CMOS)
#define	CAM_XCLC32			0x5B000000		// SONY XCL-C32						(B/W VGA 104fps 1/2"CCD)
#define	CAM_XCLS600_2TAP	0x5C010000		// SONY XCL-S600					(B/W 6Mpixel 13fps 1"CCD)
#define	CAM_XCLS600_4TAP	0x5C020000		// SONY XCL-S600					(B/W 6Mpixel 27fps 1"CCD)
#define	CAM_XCLC500			0x5D000000		// SONY XCL-C500					(B/W 5Mpixel 15fps)
#define	CAM_ID4MB			0x5E000000		// iDule ID4MB-CL					(B/W 4Mpixel 38fps)
#define	CAM_ACA2040KM		0x5F000000		// BASLER acA2040-180km				(B/W 4Mpixel 35fps)
#define	CAM_CM141MCL		0x60000000		// JAI CM-141MCL/PMCL				(B/W SXGA 30fps)
#define	CAM_CM141MCL_10BIT	0x60010000		// JAI CM-141MCL/PMCL (10bit mode)	(B/W SXGA 30fps)
#define	CAM_CM141MCL_12BIT	0x60020000		// JAI CM-141MCL/PMCL (12bit mode)	(B/W SXGA 30fps)
#define	CAM_KPF145WCL		0x61000000		// HITACHI KOKUSAI DENKI KP-F145WCL	(B/W SXGA 30fps)
#define	CAM_CVM4PLUS		0x62000000		// JAI CV-M4+CL						(B/W SXGA 24fps)
#define	CAM_CVM4PLUS_10BIT	0x62010000		// JAI CV-M4+CL (10bit mode)		(B/W SXGA 24fps)
#define	CAM_KPF32WCL		0x63000000		// HITACHI KOKUSAI DENKI KP-F32WCL	(B/W VGA 200fps)
#define	CAM_SXCL3M			0x64000000		// CIS VCC-CAM_SXCL3M				(B/W SXGA 83.8fps)
#define	CAM_STCSPB163PCL	0x65000000		// OMRON SENTECH					(B/W 1440x1080 103.2fps)
#define	CAM_BC160M			0x66000000		// Toshiba Teli BC160M				(B/W 1440x1080 99.585fps)
#define	CAM_GO5101MPMCL		0x67000000		// JAI GO-5101M-PMCL				(B/W 2464x2056)
#define	CAM_STCRBS43PCL		0x68000000		// OMRON SENTECH					(B/W 720x540 397.2fps)
#define	CAM_ID2MB			0x69000000		// iDule ID2MB-CL					(B/W 2Mpixel 70fps)
#define	CAM_IKTF5C			0xC1000000		// TOSHIBA IK-TF5C					(3CCD VGA 60fps)
#define	CAM_IKTF7C			0xC2000000		// TOSHIBA IK-TF7C					(3CCD XGA 30fps)
#define	CAM_IKTF5P			0xC3000000		// TOSHIBA IK-TF5P					(3CCD VGA 60fps)
#define	CAM_IKTF7P			0xC4000000		// TOSHIBA IK-TF7P					(3CCD XGA 30fps)
#define	CAM_F22V39APCL_RGB	0xC5000000		// CIS VCC-F22V39APCL (RGB output)
#define	CAM_IKTF9C			0xC6000000		// TOSHIBA IK-TF9C					(3CCD QXGA 20fps)
#define	CAM_HVF22CLS1		0xC7000000		// HITACHI KOKUSAI DENKI HV-F22CL-S1(3CCD SXGA 15fps)
#define	CAM_AT140CL			0xC8000000		// JAI AT-140CL						(3CCD SXGA 25fps)
#define	CAM_AT200CL			0xC9000000		// JAI AT-200CL						(3CCD UXGA 20fps)
#define	CAM_JCTTF5G_NORMAL	0xCA000000		// CANON MEDICAL SYSTEMS JCT-TF5G	(3CMOS 720x540 124fps)
#define	CAM_JCTTF5G_FAST	0xCA010000		// CANON MEDICAL SYSTEMS JCT-TF5G	(3CMOS 720x540 150fps)
#define	CAM_AP3200TPMCL		0xCB000000		// JAI AP-3200T-PMCL				(3CMOS 2064x1544)
#define	CAM_SU51			0xE0000000		// NED
#define	CAM_SU74			0xE1000000		// NED
#define	CAM_TL5150UCL		0xE2000000		// TAKEX
#define	CAM_TL7450UCL		0xE3000000		// TAKEX
#define	CAM_SUI51			0xE4000000		// NED SUi51 (8bit mode)			(B/W 5K Line Sensor)
#define	CAM_SUI74			0xE5000000		// NED SUi74 (8bit mode)			(B/W 7.5K Line Sensor)
#define	CAM_SUI51_12BIT		0xE4010000		// NED SUi51 (12bit mode)			(B/W 5K Line Sensor)
#define	CAM_XCM6040SAT2		0xE6000000		// NED XCM6040SAT2					(B/W 6K Line Sensor)
#define	CAM_L803K			0xE7000000		// BASLER L803k						(B/W 8K Line Sensor)
#define	CAM_XCM8040SAT2		0xE8000000		// NED XCM8040SAT2V1				(B/W 8K Line Sensor)
#define	CAM_RAL12288_66KM	0xE9000000		// BASLER raL12288-66km				(B/W 12K Line Sensor)
#define	CAM_SUI7440			0xEA000000		// NED SUi7440						(B/W 7.4K Line Sensor)
#define	CAM_LACM16K05A		0xEB000000		// DALSA LA-CM-16K05A-xx-R			(B/W 16K Line Sensor)
#define	CAM_KPFR31PCL		0xA0000000		// HITACHI KOKUSAI DENKI KP-FR31PCL	(Bayer VGA 120fps)
#define	CAM_F22V39PCL		0xA1000000		// CIS VCC-F22V39PCL				(Bayer VGA 120fps)
#define	CAM_KPFR80PCL		0xA2000000		// HITACHI KOKUSAI DENKI KP-FR80PCL	(Bayer XGA 36fps)
#define	CAM_F22V39APCL		0xA3000000		// CIS VCC-F22V39APCL				(Bayer VGA 120fps)
#define	CAM_FC20U19PCL		0xA4000000		// CIS VCC-FC20U19PCL				(Bayer FullHD 70.73fps)
#define	CAM_F22U29APCL		0xA5000000		// CIS VCC-F22U29APCL				(Bayer UXGA 20fps)
#define	CAM_XCL5005CR_RAW	0xA6000000		// SONY XCL-5005CR					(Bayer 5Mpixel 15fps)
#define	CAM_ACA2000KC		0xA7000000		// BASLER acA2000-340kc				(Bayer FullHD 70fps)
#define	CAM_ACA2040KC		0xA8000000		// BASLER acA2040-180kc				(Bayer 4M 35fps)
#define	CAM_CMC200PCL		0xA9000000		// SENTECH STC-CMC200PCL			(Bayer FullHD 73.8fps)
#define	CAM_KPFD32WCL		0xAA000000		// HITACHI KOKUSAI DENKI KP-FD32WCL	(Bayer VGA 200fps)
#define	CAM_ID4MC			0xAB000000		// iDule ID4MC-CL					(Bayer 4Mpixel 38fps)
#define	CAM_XCLC30C			0xAC000000		// SONY XCL-C30C					(Bayer VGA 130fps)
#define	CAM_SXCL5R			0xAD000000		// CIS VCC-SXCL5R					(Bayer 1.6M)

// USB3
#define	CAM_MBCM200U3V		0xFF000001		// Sentech STC-MBCM200U3V
#define	CAM_MBCM401U3V		0xFF000002		// Sentech STC-MBCM401U3V
#define	CAM_ACA2000_165UM	0xFF000003		// BASLER acA2000-165um
#define CAM_ACA2040_90UM	0xFF000004		// BASLER acA2040-90um
#define CAM_ACA1300_200UM	0xFF000005		// BASLER acA1300-200um
#define	CAM_BU205M			0xFF000006		// Teli BU205M						(B/W 2048x1088 170fps)
#define	CAM_BU505MG			0xFF000007		// Teli BU505MG						(B/W 2448x2048 75fps)
#define	CAM_DU1207MG		0xFF000008		// Teli DU1207MG					(B/W 4096x3000 30fps)

// Analog I/F Color Camera
#define	IKTU40			0x80000000		// IK-TU40,IKTU50,IK-TF1,IK-TF2 (Toshiba)
#define	IKTF5			0x81000000		// IK-TF5
#define	IKTF7			0x82000000		// IK-TF7
#define	IKTU50			0x83000000		// IK-TU40,IKTU50,IK-TF1,IK-TF2 (Toshiba)

//R_get_video_auto
#define WAIT_SHUTTER		0x00000000
#define NON_CHECK_SI		0x00000100	// for CO-CZ-33 only (use with R_get_video_auto_si_wait)
#define NON_WAIT_SHUTTER	0x00000200	// for CO-CZ-33 only (use with R_get_video_auto_si_wait)
#define	GV_NO_VRAM			0x00100000	// for CO-CZ-39 or later
#define	GV_NO_WAIT			0x00200000	// for CO-CZ-39 or later
#define	GV_NO_WEN			0x00400000	// for CO-CZ-39 or later
#define	GV_EXT_TRIG			0x00800000	// for CO-CZ-39 or later

#define	ANY_FEILD	ANY_FIELD
#define	ODD_FEILD	ODD_FIELD
#define	EVEN_FEILD	EVEN_FIELD

#define	GRAY_PLANE	0x01
#define	BIN_PLANE	0x02

// R_display_control & R_get_video_out_mode
#define	GRAY_MEMORY		0x01
#define	BIN_MEMORY		0x02
#define	GRAY_THROUGH	0x04
#define	BIN_THROUGH		0x08
#define	GRAY_BRIGHT		0x10
#define	GRAPH_MEMORY	0x20
#define	DRAW_PLANE_ONLY	0x80
#define	DISPLAY_OFF		0x80

#define	InternalControl		0x0000
#define	ExternalControl		0x1000
#define	PriorityToOriginal	0x2000
#define	PriorityToLine		0x3000
#define	NormalVideoOut		0x0000
#define	CH3VideoOut			0x0800
#define	VIDEO_OUT_MUX_MASK	0x3800

#define	MemBuffer240		0x10000

#define	TARTGET		0x00000000
#define	INITIATOR	0x00010000
#define	TURBO		0x00030000
#define	SLOW		0x00040000
#define	CPU_TRNS	0x00080000

#define OK		0
#define ERROR	(-1)
#define NO_WAIT		0
#define WAITING		1
#define NO_CHECK	2

// R_set_line_page() / R_get_line_page()
#define	USER_LINE_PAGE	0
#define	SYSTEM_LINE_PAGE	0x80

// R_video_set_display_page / R_video_get_display_page
#define	VIDEO_DISP_PAGE_0		0x00000000
#define	VIDEO_DISP_PAGE_1		0x00000001
#define	VIDEO_DISP_PAGE_2		0x00000002	// CO-CZ-43 or later
#define	VIDEO_DISP_PAGE_3		0x00000003	// CO-CZ-43 or later
#define	VIDEO_DISP_PAGE_EXT		0x00000010	// CO-CZ-39 only
#define	VIDEO_DISP_PAGE_OFF		0x00000080

#define	VIDEO_DISP_OFFSET_MASK	0xFF000000
#define	VIDEO_DISP_OFFSET_31	0x00000000
#define	VIDEO_DISP_OFFSET_0		0x01000000

#define	VIDEO_DISP_RESO_MASK		0x001F0000
#define	VIDEO_DISP_RESO_XGA			0x00000000	// CO-CZ-39 or CO-CZ-43
// VIDEO_DISP_RESO_XGA @ CO-CZ-39				// 1024x768@60Hz (4:3)			DMT ID:10h (error: +0.7%)
// VIDEO_DISP_RESO_XGA @ CO-CZ-43				// 1024x768@70Hz (4:3)			DMT ID:11h (no error)
#define	VIDEO_DISP_RESO_VGA			0x00010000	// CO-CZ-39 or CO-CZ-43
// VIDEO_DISP_RESO_VGA @ CO-CZ-39				// 640x480@60Hz (4:3)			DMT ID:04h (error: -2.4%)
// VIDEO_DISP_RESO_VGA @ CO-CZ-43				// 640x480@60Hz (4:3)			DMT ID:04h (error: -0.7%)
#define	VIDEO_DISP_RESO_SVGA		0x00020000	// CO-CZ-43
// VIDEO_DISP_RESO_SVGA @ CO-CZ-43				// 800x600@72Hz (4:3)			DMT ID:0Ah (no error)
#define	VIDEO_DISP_RESO_SXGA		0x00030000	// CO-CZ-43
// VIDEO_DISP_RESO_SXGA @ CO-CZ-43				// 1280x1024@72Hz (4:3)			Not standard
#define	VIDEO_DISP_RESO_UXGA		0x00040000	// CO-CZ-43
// CO-CZ-43/44 Ver.1.20 or later
#define	VIDEO_DISP_RESO_DMT10H		0x00050000	// 1024x768@60Hz (4:3)			DMT ID:10h  65.000MHz(error: )
#define	VIDEO_DISP_RESO_DMT11H		0x00150000	// 1024x768@70Hz (4:3)			DMT ID:11h  75.000MHz(no error)
#define	VIDEO_DISP_RESO_DMT23H		0x00060000	// 1280x1024@60Hz (5:4)			DMT ID:23h 108.000MHz(no error)
#define	VIDEO_DISP_RESO_CVT104M9R	0x00070000	// 1360x768@60HzRB (16:9)		CVT 1.04M9-R
#define	VIDEO_DISP_RESO_DMT2EH		0x00080000	// 1440x900@60HzRB (16:10)		DMT ID:2Eh  88.750MHz(error: )
#define	VIDEO_DISP_RESO_DMT39H		0x00090000	// 1680x1050@60HzRB (16:10)		DMT ID:39h 119.000MHz(error: )
#define	VIDEO_DISP_RESO_FULLHD_DELL	0x000A0000	// 1920x1080@60Hz (16:9)
#define	VIDEO_DISP_RESO_WUXGA_DELL	0x000B0000	// 1920x1200@60Hz (16:10)
#define	VIDEO_DISP_RESO_DMT07H		0x000C0000	// 640x480@85Hz (4:3)			DMT ID:07h  36.000MHz(no error)
#define	VIDEO_DISP_RESO_DMT53H		0x000D0000	// 1600x900@60HzRB (16:9)		DMT ID:53h 108.000MHz(no error)
#define	VIDEO_DISP_RESO_DMT56H		0x000E0000	// 1366x768@60Hz (16:9)			DMT ID:56h  72.000MHz(no error)
#define	VIDEO_DISP_RESO_DMT33H		0x00160000	// 1600x1200@60Hz (4:3)			DMT ID:33h 162.000MHz(no error)
// CO-CZ-47 or later
#define	VIDEO_DISP_RESO_DMT04H		0x000F0000	// 640x480@60Hz (4:3)			DMT ID:04h  25.175MHz
#define	VIDEO_DISP_RESO_DMT05H		0x00100000	// 640x480@72Hz (4:3)			DMT ID:05h  31.500MHz
#define	VIDEO_DISP_RESO_DMT06H		0x00110000	// 640x480@75Hz (4:3)			DMT ID:06h  31.500MHz
#define	VIDEO_DISP_RESO_DMT12H		0x00120000	// 1024x768@75Hz (4:3)			DMT ID:12h  78.750MHz
#define	VIDEO_DISP_RESO_DMT51H		0x00130000	// 1366x768@60Hz (16:9)			DMT ID:51h  85.500MHz
#define	VIDEO_DISP_RESO_DMT52H		0x00140000	// 1920x1080@60Hz (16:9)		DMT ID:52h 148.500MHz
#define	VIDEO_DISP_RESO_DMT44H		0x00170000	// 1920x1200@60HzRB (16:10)		DMT ID:44h 154.000MHz
#define	VIDEO_DISP_RESO_VGA_43		0x00180000	// 640x480@60Hz (4:3)			DMT ID:04h (error: -0.7%)

#define	VIDEO_DISP_VGA			VIDEO_DISP_RESO_VGA	// for compatibility
#define	VIDEO_DISP_XGA			VIDEO_DISP_RESO_XGA	// for compatibility

// R_video_load/save_adc_reference
#define	VIDEO_ADCREF_USER0	0x000
#define	VIDEO_ADCREF_INIT0	0x100

#define	R_get_video_mode()	Video_mode
#define	R_get_video_out_mode()	Video_out_mode
#define R_get_line_page()	(OUT0_DATA & 0x80)
int R_channel( int flag, int channel, int bin_level ) ;
int R_change_video_mode( int mode ) ;
int R_change_camera_mode( int mode);
int R_get_video( int mode, int flag );
int R_display_control( int mode );
int R_video_binlevel( int level );
void R_set_line_page( int mode );

int SetDac6325(int ch, int data);
int R_regulate_COCZ27( int Vrt, int Vrb, int Vref ) ;
int R_regulate_COCZ33( int Vrt, int Vrb, int Vref ) ;

int R_change_video_card(int id_num);	// id_num => 0 to (VIDEO_CARD_MAX - 1)
										// VIDEO_CARD_MAX is defined in R_stdlib.h
int	R_video_get_card_id(void);

int	R_video_set_shutter_speed(int nMicroSec);
int R_video_get_shutter_speed(void);

#define	SHUTTER_NOW			0
#define	SHUTTER_VS			1
#define	SHUTTER_WAIT_VS		2
int	R_video_shutter(int nMode);
int R_get_video_auto(int ,int);
int R_get_video_auto_end_check(void);
int R_get_video_auto_shutter_end_check(void);	// CO-CZ-39 or later
int R_get_video_auto_si_wait(int camera);		// CO-CZ-33 only
int	R_get_video_auto_line_check(void);			// CO-CZ-43 or later

void	R_get_video_overlap_reset(void);
int		R_get_video_overlap(int mode, int flag);
int		R_get_video_overlap_end_check(int handle);
int		R_get_video_overlap_shutter_end_check(int handle);
int		R_get_video_overlap_is_ready(void);

int	R_video_set_initiator_page( int ID, int page);
int	R_video_get_initiator_page( int ID );
int	R_video_check_version_of_cocz33(int ID);				// 引き数修正  2006.05.08  by matsu

int R_check_DISPT(void);

int	R_video_set_display_page(int nPage);
int	R_video_get_display_page(void);

#ifndef __x86_64__
int R_video_init_memory( int gray_number, int bin_number, int resolution );
int r_video_free_memory(void);

/*!
 * @brief R_video_gray_memory_allocate
 * @param page
 * @param size
 * @return 
 */
int	R_video_gray_memory_allocate(int page, int size);
int	R_video_gray_memory_free(int page);

/*!
 * @brief R_video_load_adc_reference
 * @param nNum
 * @return 
 */
int	R_video_load_adc_reference(int nNum);

/*!
 * @brief R_video_save_adc_reference
 * @param nNum
 * @return 
 */
int	R_video_save_adc_reference(int nNum);
#endif

/*!
 * @brief R_video_init
 * @param nCameraType
 * @param nFrameSize
 * @param nCameraMode
 * @param nMode
 * @retval OK
 * @retval ERROR
 */
int	R_video_init(int nCameraType, int nFrameSize, int nCameraMode, int nMode);
// R_video_init entry
#define	PARTIAL_300		((int)(0x00008000 | 300))	// XC-HR50 only
#define	PARTIAL_340		((int)(0x00008000 | 340))	// XC-HR50 only
#define	PARTIAL_480		((int)(0x00008000 | 480))	// XC-HR50 only
#define	VI_CAMERA_INIT			0
#define	VI_BOARD_INIT_ONLY		0x10000000
#define	VI_HORIZONTAL_MIRROR	0x00000001	// CO-CZ-43/44 or later
#define	VI_RESIZE_MODE_1		0x00000100	// CO-CZ-43/44 or later
#define	VI_RESIZE_MODE_2		0x00000200	// CO-CZ-43/44 or later
#define	VI_SHRINKED_RGB_1		0x00000300	// CO-CZ-43/44 or later
#define	VI_HORIZONTAL_BINNING_2	0x00110000	// Basler USB3
#define	VI_VERTICAL_BINNING_2	0x00140000	// Basler USB3
#define	VI_BINNING_2X2			0x00150000	// VCC-SXCL3M
#define	VI_AVERAGING_2X2		0x00250000	// VCC-SXCL3M
#define	VI_SUBSAMPLING_2X2		0x00350000	// VCC-SXCL3M
#define	VI_BINNING_1X4			0x00103000	// 
#define	VI_BINNING_2X4			0x00113000	// 
#define	VI_CAMERA_LINK_3TAP		0x40000000	// CO-CZ-47

// VI_RESIZE_MODE_1
// ┌────────────┐
// │ ┌──┐　　　┌──┐ │　　　┌──┬──┐
// │ │　　│　　　│　　│ │　　　│　　│　　│
// │ │ ∩ │　　　│ ↑ │ │　->　│ ∩ │ ↑ │
// │ │　　│　　　│　　│ │　　　│　　│　　│
// │ └──┘　　　└──┘ │　　　└──┴──┘
// └────────────┘

// VI_RESIZE_MODE_2
// ┌────────────┐
// │ ┌──┐　　　┌──┐ │　　　┌─────┐
// │ │　　│　　　│　　│ │　　　│　　⊃　　│
// │ │ ∩ │　　　│ ↑ │ │　->　├─────┤
// │ │　　│　　　│　　│ │　　　│　　→　　│
// │ └──┘　　　└──┘ │　　　└─────┘
// └────────────┘

// R_video_init return
#define	VI_ERROR_ENTRY		ERROR
#define	VI_ERROR_BOARD		ERROR2
#define	VI_ERROR_NO_CAMERA	1
#define	VI_ERROR_SIO		2

int	R_video_board_id(void);
// R_video_board_id return value
#define	BOARD_ID_COCZ23		(int)23
#define	BOARD_ID_COCZ24		(int)24
#define	BOARD_ID_COCZ27		(int)27
#define	BOARD_ID_COCZ31		(int)31
#define	BOARD_ID_COCZ33		(int)33
#define	BOARD_ID_COCZ35		(int)35
#define	BOARD_ID_COCZ39		(int)39
#define	BOARD_ID_COCZ41		(int)41
#define	BOARD_ID_COCZ43		(int)43
#define	BOARD_ID_COCZ47		(int)47
#define	BOARD_ID_USB		(int)20

/*!
 * @brief R_video_board_is_cocz43_compatible
 * @retval	!0	CO-CZ-43 or CO-CZ-47
 * @retval	0	the others
 */
int	R_video_board_is_cocz43_compatible(void);

/*!
 * @brief R_video_use_cocz47_as_cocz43
 * 		CO-CZ-47を使用している場合、次のように基板タイプを変換します。
 * 		・CO-CZ-47の基板数はグローバル変数COCZ43に加算されます。グローバル変数COCZ47は0になります。
 * 		・R_video_board_id関数はBOARD_ID_COCZ47ではなく、BOARD_ID_COCZ43を返すようになります。
 * 		CO-CZ-47以外の基板の動作に影響はありません。
 * 		起動オプションに"-use_cocz47as43"を使用するのと同じ処理になります。
 */
void	R_video_use_cocz47_as_cocz43(void);

#ifndef __x86_64__
int	R_video_gpio_control(int nBit, int nMode);
#endif
#define	VGPIO_BIT0		0x01	// nBit
#define	VGPIO_SYNC		0		// nMode
#define	VGPIO_ON		1		// nMode
#define	VGPIO_OFF		2		// nMode
#define	VGPIO_SYNC_INV	4		// nMode
//int	R_video_set_shutter_timing(int nTurnOnTiming, int nTurnOffTiming);
//int	R_video_set_syncout_timing(int nSignal, int nTurnOnTiming, int nTurnOffTiming);

/*!
 * @brief R_video_gpio_set_syncout_delay
 * @param nBit
 * @param nTurnOnDelay		micro second
 * @param nTurnOffDelay		micro second
 * @retval	OK
 * @retval	ERROR 
 */
int	R_video_gpio_set_syncout_delay(int nBit, int nTurnOnDelay, int nTurnOffDelay);

int	R_video_set_scan_line(int nScanLine);
int	R_video_get_scan_line(void);
int	R_video_set_scan_rate(int nNanoSec);
int	R_video_get_scan_rate(void);

int	R_video_set_camera_gain(int nGain);
int	R_video_get_camera_gain(void);
int	R_video_camera_max_gain(void);	// under construction
int	R_video_camera_min_gain(void);	// under construction

int	R_video_camera_type(void);

/*!
 * @brief R_video_camera_fval
 *	Camera Link FVAL status
 *	Only support CO-CZ-43/44
 * @retval	!0	active
 * @retval	0	inactive
 */
int	R_video_camera_fval(void);

/*!
 * @brief R_video_trigger_input_is_on
 *	External trigger input status
 *	Only support CO-CZ-43/44
 * @retval	!0	input status is 'on'
 * @retval	0	input status is 'off'
 */
int R_video_trigger_input_is_on(void);

int	R_video_initiator_page(void);

int	R_video_set_display_mode(int nMode);
int	R_video_get_display_mode(void);
#define	DM_GRAY8	0x001
#define	DM_COLOR32	0x100

int	R_video_set_trigger_input_delay(int nMicroSec);
int	R_video_get_trigger_input_delay(void);

int	R_video_set_partial_scan_mode(int nMode, int nStart, int nWidth);
#define	PARTIAL_SCAN_CENTER (-1)
// nMode : ON or OFF (ON,OFF -> R_stdio.h)

int	R_video_check_camera_vsync(void);

int	R_video_set_capture_vram_offset(unsigned long offset0, unsigned long offset1);

#ifdef __GNUC__
int	R_video_dma_to_vram(int nPage, R_RECT* pRect);
int	R_video_dma_end_check(void);
#endif

int	R_video_sio_clisbee_write(const char* command);
void	r_video_sio_clisbee_debug(int on_off);

int	R_video_init_look_up_table(const unsigned char ucLut[]);	// CO-CZ-43 only

typedef enum {
	R_VIDEO_BAYER_CONV_DEFAULT = 0,
	R_VIDEO_BAYER_CONV_0RGB_1,
	R_VIDEO_BAYER_CONV_0R0B_1
} R_VIDEO_BAYER_CONV_MODE;

/*!
 * @brief	ベイヤー変換モード設定@n
 * @param	mode	R_VIDEO_BAYER_CONV_MODE
 * @retval	OK		正常終了
 * @retval	OK以外	異常終了
 */
int	R_video_set_bayer_conv_mode(R_VIDEO_BAYER_CONV_MODE mode);

/*!
 * @brief	ベイヤー変換モード取得@n
 * @retval	mode	R_VIDEO_BAYER_CONV_MODE
 */
R_VIDEO_BAYER_CONV_MODE	R_video_get_bayer_conv_mode(void);

/*!
 * @brief	取り込み開始ピクセル位置設定@n
 * @param	start	取り込み開始ピクセル位置
 * @retval	OK		正常終了
 * @retval	OK以外	異常終了
 */
int	R_video_set_capture_start_pixel(int start);

/*!
 * @brief	取り込み開始ピクセル位置取得@n
 * @retval	0以上	開始ピクセル位置
 * @retval	負の値	異常終了
 */
int	R_video_get_capture_start_pixel(void);

// 内部関数
int	r_video_sio_init(int bps, int len, int parity, int stops, int flow);	// CO-CZ-43 only
int	r_video_sio_putc(int);	// CO-CZ-43 only
int	r_video_sio_getc(void);	// CO-CZ-43 only

typedef enum {
	R_CAMERA_INTERFACE_IS_NOT_DEFINED = 0,
	R_CAMERA_INTERFACE_IS_ANALOG,
	R_CAMERA_INTERFACE_IS_USB3_VISION
} R_CAMERA_INTERFACE_TYPE;

// この構造体の仕様は非公開です。直接参照しないでください。
struct cocz33_t {
	int				nID;
	int				nBoard;
	int				nRevision;
	void*			lpGrayAddress;
	void*			lpBinAddress;
	void*			lpDrawAddress;
	unsigned long	nS5933Port;
	uintptr_t		nRegisterPort;
	int				nIrq;
	int				nOutData0;
	int				nOutData1;
	int				nOutData2;
	int				nOutData3;
	int				nOutData4;
	int				nOutData5;
	int				nOutData6;
	int				nOutData7;
	
	int				nOutPort0;
	int				nOutPort1;
	int				nOutPort2;
	int				nOutPort3;
	int				nOutPort4;
	unsigned long	nOutPort5;
	unsigned long	nOutPort6;
	unsigned long	nOutPort7;
	
	int				nInPort0;
	int				nInPort1;
	int				nInPort2;
	int				nInPort3;
	
	int				nVideoMode;
	int				nVideoOutMode;
	int				nFxSize;
	int				nFySize;
	int				nMxSize;
	int				nMySize;
	int				nCaptureLine;
	int				nCameraMode;
	int				nCameraType;
	int				nDrawMode;
	int				nDispLine;
	int				nDispColumn;
	int				nGraphPage;
	
	int				nShutterSpeed;
	
	int				nAutoVideoMode;			// 01/05/14 by ohshima
	RTIME			nAutoVideoTime;			// 01/05/14 by ohshima
	int				nAutoVideoCaptureTime;	// 04/10/06 by araki
	int				nAutoVideoOutMode;		// 01/06/01 by ohshima
	int				nAutoVideoPlane;		// 01/06/13 by ohshima
	
	void*			lpGrayImage;
	void*			lpBinImage;
	void*			lpImageBuffer;
	
	int				nStatus;
	int				nStatus1;					// 2006/11/29
	int				nStatus2;					// 2006/11/29
	int				nStatus3;					// 2006/11/29
	
	int				nInitiatorPage;
	int				nInitiatorOffset;			// 2017.09.07
	
	int				nChannel;
	
	int				nMuxCameraMode[2];			// 2005/02/16
	int				nMuxCameraType[2];			// 2005/02/16
	int				nMuxShutterSpeed[2];		// 2005/02/16
	
	int				nAdcReference[4];
	
	R_PLANE*		pPlaneDraw;
	R_PLANE*		pPlaneUser;
	R_PLANE*		pPlaneTitle;
	R_PLANE*		pPlaneSide;
	
	int				nBytesOfBinPlane;			// 2011/06/29
	void*			pBinPage[2];				// 2007/01/06
	int				nCaptureTimeLimit;			// 2007/02/07
	int				nWaitVsyncLimit;			// 2008/03/18
	int				nLinePeriod;				// 2008/03/26
	
	int				nBitAssignment;				// 2007/02/15
	int				nDataBaseAdr;				// 2007/02/15
	int				nBinBaseAdr;				// 2007/02/15
	
	int				nCameraSpec;				// 2007/02/26
	
	int				nBoardSpec;					// 2007/03/15
	unsigned long	ulVramBaseAdr;				// 2007/03/15
	unsigned long	ulVramByteSize;				// 2007/03/15
	
	void*			pFileBuffer;				// 2007/03/27
	int				nFileSize;					// 2007/03/27
	
	int				nVideoModeAssignment;		// 2008/05/09
	int				nCameraLinkType;			// 2008/07/07
	int				nGain;						// 2008/07/09
	int				nGainMax;					// 2011/09/14
	int				nGainMin;					// 2011/09/14
	int				nGainData[4];				// 2011/09/14
	
	int				nPixelClock[2];				// 2012/01/18
	
	unsigned long	ulCaptureBaseAdr[2];		// 2008/09/10
	
	volatile int*	pStatus;					// 2007/06/04 for LINUX
	
	R_PLANE*		pPlaneImageVram;			// 2008/02/04 for LINUX
	R_PLANE*		pPlaneBinaryVram;			// 2008/02/04 for LINUX
	R_PLANE*		pPlaneInitiator;			// 2008/02/04 for LINUX
	
	struct r_display_t*	pDisplay;				// 2008/02/22 for LINUX
	int				nCamLnkType;				// 2008/02/22 for LINUX
	
	RTIME			timeGetVideo[16];			// 2009/01/15
	int				nGetVideoMode[16];			// 2009/02/26
	int				nOverlapStartTiming;		// 2009/01/16
	int				nOverlapTriggerRise;		// 2009/01/16
	int				nOverlapTriggerFall;		// 2009/01/16
	int				nOverlapStrobeStart;		// 2009/01/19
	int				nOverlapStrobeEnd;			// 2009/01/19
	int				nHSyncPeriod;				// 2009/01/16
	int				nStrobeDelay;				// 2009/01/19
	int				nOverlapLimitLine;			// 2009/01/16
	
	int				nTriggerInputDelay;			// 2009/02/26
	
	int				nAutoCaptureMode;			// 2009/10/14
	int				nStatusEnableFlag;			// 2009/10/14
	int				bIsLineScanCamera;			// 2009/10/14
	
	int				nCaptureType;				// 2013.04.01
	int				nBayerType;					// 2013.08.13
	R_VIDEO_BAYER_CONV_MODE	nBayerConvMode;		// 2017.08.18
	
	int				nSoftwareShrinkMode;		// 2018.11.06
	int				bShrinkIsRequired;			// 2018.11.06
	int				nFilterMode;				// 2014.07.28
	int				bFilterIsRequired;			// 2014.07.28
	
	int				nResizeMode;				// 2014.08.05
	int				bResizeIsRequired;			// 2014.08.05
	int				nResizeBufferPage;			// 2014.08.05
	int				nBufferWidth;				// 2014.08.05
	int				nBufferHeight;				// 2014.08.05
	int				nBufferSize;				// 2015.04.07
	struct {									// 2014.08.05
		int			left;
		int			width;
	} XResizeArea[4];
	int				nPolarityOfTrigger;			// 2014.09.16
	struct {									// 2016.03.25
		int			invert_logic;
		int			turn_on_delay;
		int			turn_off_delay;
	} sync_out[2];
	int				nCopyPageToVram;			// 2012.12.04
#ifdef __GNUC__
	int				handle;						// 2011/10/27
	const void*		header;						// 2017.02.17
	int				nMemmgrOffset;				// 2011/10/28
	void*			pDmaDescriptor;
	int				nDmaDescriptorSize;
	uint64_t		nDmaDescriptorBusAdr;
	uint32_t*		pDmaLastByteCount;
	RTIME			timeDmaTimer;
	int				nDmaChannel0Command;
#endif
	uint32_t		physicalCaptureBuffer;		// 2012.12.21
#ifdef __GNUC__
	struct r_memmgr*	pCaptureBuffer;			// 2013.03.22
	unsigned long	ulBoardVramBaseAdr;			// 2013.04.02
	
	pthread_t		thread_id;					// 2013.08.12
	int				interpolation_finished;		// 2013.08.12
	R_PLANE*		interpolation_plane;		// 2013.08.12
	int				interpolation_type;			// 2013.08.12
	int				interpolation_step;			// 2013.08.12
	int				white_balance;				// 2013.08.13
	
	const char*		device_name;				// 2015.07.01
	int				idVendor;					// 2015.07.02
	int				idProduct;					// 2015.07.02
	
	struct {
		void*		start;						// 2015.07.03
		int			length;						// 2015.07.03
	} buffer[4];
#endif
	void*	funcObject;							// 2015.10.22
	int		(*funcGetVideo)(struct cocz33_t*, int, int);			// 2015.10.22
	int		(*funcGetVideoAuto)(struct cocz33_t*, int, int);		// 2015.10.22
	int		(*funcGetVideoAutoEndCheck)(struct cocz33_t*);			// 2015.10.22
	int		(*funcGetVideoAutoShtterEndCheck)(struct cocz33_t*);	// 2015.10.22
	int		(*funcSetShutterSpeed)(struct cocz33_t*, int);			// 2015.10.22
	int		(*funcGpioControl)(struct cocz33_t*, int, int);			// 2015.10.28
	int		(*funcSetCameraGain)(struct cocz33_t*, int);			// 2015.10.30
	
	R_CAMERA_INTERFACE_TYPE	camera_interface;						// 2015.11.26
	
	int		bOnOverlapCaptureMode;				// 2016.02.03
	int		isVirtualCamera;					// 2017.02.06
	int		nLatestCaptureID;					// 2017.09.19
	
	uint32_t	physical_address_of_status;		// 2020.04.06
};

// nVideoModeAssignment
#define	VMA_BINNING_BIT				0x01		// 2008/05/09
#define	VMA_HALF_RESOLUTION_BIT		0x10		// 2008/05/09

/*!
 * @brief The r_display_t struct@n
 * この構造体の仕様は非公開です。直接参照しないでください。
 */
struct r_display_t {
	unsigned long		nRegPort;
	int					nFxSize;
	int					nFySize;
	int					nPage;
	int					nSelect[2];
	struct cocz33_t*	pCocz33[2];
	R_POINT				pointOffset[2];
	int					nWinMode[2];
};

extern struct cocz33_t	Cocz33Resource[VIDEO_CARD_MAX];
extern struct cocz33_t*	lpCurrentCocz33;
extern struct r_display_t*	pCurrentDisplay;

#define		DRAW_PLANE_RESOURCE_MAX		4
extern R_PLANE*		RDrawPlaneResourcePtr[DRAW_PLANE_RESOURCE_MAX][3];
extern R_PLANE*		pCurrentDrawPlane;
extern R_PLANE*		pCurrentDrawTitlePlane;




// ライブラリ内部関数（CO-CZ-43用）
int	r_display_set_offset(int offset);
int	r_display_get_offset(void);
#define		DISP_OFFSET_0	(int)0
#define		DISP_OFFSET_31	(int)0x001F001F

int	r_display_set_win_page(int win, int page);			// page = 0(camera0), 1(camera1)
int	r_display_get_win_page(int win);

int	r_display_set_win_rect(int win, R_RECT* pRect);
int	r_display_get_win_rect(int win, R_RECT* pRect);

int	r_display_set_win_offset(int win, R_POINT* pPoint);
int	r_display_get_win_offset(int win, R_POINT* pPoint);

int	r_display_set_win_zoom(int win, int zoom);
int	r_display_get_win_zoom(int win);
#define		DISP_WIN_ZOOM_EQUAL		0x0000
#define		DISP_WIN_ZOOM_HALF		0x0101
#define		DISP_WIN_ZOOM_DOUBLE	0x0202
#define		DISP_WIN_ZOOM_BINNING	0x0200
#define		DISP_WIN_ZOOM_QUARTER	0x0303

int	r_display_set_win_switch(int win, int on_off);
int	r_display_get_win_switch(int win);
//#define	OFF	0	-> R_stdio.h
//#define	ON	1	-> R_stdio.h

int	r_display_set_win_mx_size(int win, int mx_size);

int	r_display_fx_size(void);
int	r_display_fy_size(void);

int	r_display_on_physical_draw_plane(void);

int	r_video_dma_gray_memory(int nSrcNum, int nDstNum, R_RECT* pRect);

int	r_video_boards_max_size_is_512_480(void);
int	r_video_get_frame_size(int frame_size, int* width, int* height);




/*!
 * @brief	カメラ接続確認@n
 * 				キャプチャ基板にカメラが接続されているかどうか確認します。@n
 * 				CO-CZ-39A、39B、43、44で使用可能。@n
 * @retval	!0	カメラが接続されている。
 * @retval	0	カメラが接続されていない。
 */
int	R_video_camera_is_present(void);

int	r_video_pixel_clock(void);

int	R_video_set_display_card(int card);

/*!
 * @brief	ベイヤーパターン取得@n
 * 				ベイヤーカメラの場合、取り込んだ画像の左上の配列パターンを取得します。@n
 * 				CO-CZ-43、44で使用可能。@n
 * @retval	R_BAYER_ARRAY_RGGB
 * @retval	R_BAYER_ARRAY_GRBG
 * @retval	R_BAYER_ARRAY_GBRG
 * @retval	R_BAYER_ARRAY_BGGR
 * @retval	-1	ベイヤーカメラではない。
 */
int	R_video_bayer_type(void);	// 2013.08.13

int	R_video_draw_plane_is_present(void);	// 2015.09.10

int	r_video_check_option(const char* opt);
int	r_video_scan_usb_camera(int num);

R_CAMERA_INTERFACE_TYPE R_video_camera_interface_type(void);

#ifdef __GNUC__
struct r_video_page_info {
	struct r_memmgr*	pmemmgr;
	void*				user_adr;
	unsigned long		bus_adr;
	unsigned long		size;
	unsigned long		offset;
};
struct	r_video_page_info* r_video_gray_memory_info(int);

int	r_video_set_initiator_offset(int bytes);

int	R_video_auto_bayer_color_interpolation(R_PLANE* plane, int interpolation, int white_balance);	// 2013.08.12
int	R_video_interpolate(int src, int dst);	// 2013.10.11
#endif	// __GNUC__

int	R_video_set_horizontal_trimming_offset(int num, int offset);

/*
				フィールドメモリーマップ（グレーメモリー）

		c00000                       c000ff                   c001ff
		+-------------------------------+............................
c00200	|								|							.
c00400	|								|							.
		|								|							.
		|								|							.
		|								|							.
		|								|							.
		|		２５６×２４０			|							.
		|								|							.
		|								|							.
		|								|							.
		|								|							.
		|								|							.
		|								|							.
c1fe00	+-------------------------------+							.
		.							 c1feff							.
		.															.
		.						５１２×４８０						.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
c3fe00	.............................................................
															   c3ffff


				ビットマップメモリーマップ（バイナリーメモリー）

		c400000                       c40001f                    c40003f
		+-------------------------------+............................
c400040	|								|							.
c400080	|								|							.
		|								|							.
		|								|							.
		|								|							.
		|								|							.
		|		２５６×２４０			|							.
		|								|							.
		|								|							.
		|								|							.
		|								|							.
		|								|							.
		|								|							.
c43ffc0	+-------------------------------+							.
		.							 c43ffdf						.
		.															.
		.						５１２×４８０						.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
		.															.
c47ffc0	.............................................................	c47ffff

*/

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class RCamera
{
public:
	RCamera() {}
	virtual ~RCamera() {}
};
#endif

#endif	// R_VIDEO_H_
