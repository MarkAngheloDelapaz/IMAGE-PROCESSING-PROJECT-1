/******************************************************************************
	R_PCI.H / R_PCI.C
	
	変更履歴
	Ver 3.03	1998/10/15	CO-CY-34対応
	Ver 3.04	1999/01/13	CO-CZ-33複数枚対応
	Ver 3.20	1999/12/12	CO-CY-36サポート
	Ver 3.278	2000/11/21	IN-CY-92サポート
	Ver 3.30	2002/01/09	DR-CY-82,DR-CY-83サポート
	Ver 3.33	2002/12/25	DR-CY-90サポート
	Ver 3.35	2003/02/20	DR-CY-88対応
	Ver 3.41	2003/10/14	CO-CZ-39用パラメータ追加
	Ver 3.42	2004/04/05	DR-CY-95対応
	Ver 3.45	2004/11/01	CO-CY-40対応
	Ver 3.45	2005/03/22	CO-CY-42対応
	Ver 3.46	2005/08/04	DR-CY-101対応
	Ver 3.48	2006/07/27	CO-CZ-41、CO-CZ-43用ＩＤ追加
	Ver 3.49	2007/02/22	DR-CY-105対応
	Ver 3.50	2008/03/28	CO-CY-45対応
	Ver 3.50	2009/02/06	DOS & LINUX ソース統合
	Ver 4.01	2009/08/31	DR-X-124,125用 ＩＤ追加
	Ver 4.01	2010/01/15	DR-X-127用     ＩＤ追加
	Ver 4.01	2011/03/25	IN-CY-116・ID追加
	Ver 4.01	2011/05/30	DR-CY-132・ID追加
	Ver 4.01	2011/06/05	ME-CY-32・ID追加
	Ver 4.01	2012/04/21	DR-X-136用     ＩＤ追加
	Ver 4.01	2014/04/21	DR-CY-140・ID追加
	Ver 4.01	2014/11/26	DR-CY-143・ID追加
	Ver 4.01	2015/05/07	DR-X-147・ID追加
	Ver 4.01	2015/09/25	IN-CY-119・ID追加
	Ver 4.01	2015/12/18	IN-CY-122・ID追加
	Ver 5.004	2018/04/20	DR-CY-144・ID追加
	Ver 5.006	2018/09/12	DR-CY-149 & ME-CY-35・ID追加
	Ver 5.028	2020/05/28	CO-CZ-47・ID追加
******************************************************************************/

#ifndef	R_PCI_H_
#define	R_PCI_H_

#define	CONFIG_ADDRESS		0x0cf8
#define	CONFIG_DATA			0x0cfc

#define	ROHM_IPG_ID			0x5253		// Vendor ID 'RS'
#define	BROOKTREE_ID		0x109E		// Vendor ID
#define	DEVICE_ID_COCZ24	0x5624		// CO-CZ-24 Device ID 'V' + 0x24
#define	DEVICE_ID_COCZ30	0x5630		// CO-CZ-30 Device ID 'V' + 0x30
#define	DEVICE_ID_COCY32	0x5632		// CO-CY-32 Device ID 'V' + 0x32
#define	DEVICE_ID_COCZ33	0x5633		// CO-CZ-33 Device ID 'V' + 0x33
#define	DEVICE_ID_COCY34	0x0350		// CO-CY-34 Device ID Bt848
#define	DEVICE_ID_COCZ35	0x5635		// CO-CZ-35 Device ID 'V' + 0x35
#define	DEVICE_ID_COCY36	0x5636		// CO-CY-36 Device ID 'V' + 0x36
#define	DEVICE_ID_COCZ39	0x5639		// CO-CZ-39 Device ID 'V' + 0x39
#define	DEVICE_ID_COCY40	0x5640		// CO-CY-40 Device ID 'V' + 0x40
#define	DEVICE_ID_COCZ41	0x5641		// CO-CZ-41 Device ID 'V' + 0x41
#define	DEVICE_ID_COCY42	0x5642		// CO-CY-42 Device ID 'V' + 0x42
#define	DEVICE_ID_COCZ43	0x5643		// CO-CZ-43 Device ID 'V' + 0x43
#define	DEVICE_ID_COCZ43L	0x5623		// CO-CZ-43 Device ID 'V' + 0x23
#define	DEVICE_ID_COCY45	0x5645		// CO-CY-45 Device ID 'V' + 0x45
#define	DEVICE_ID_COCZ47	0x5647		// CO-CZ-47 Device ID 'V' + 0x47
#define	DEVICE_ID_CPCZ47	0x4347		// CP-CZ-47 Device ID 'C' + 0x47
#define	DEVICE_ID_INCY76	0x4976		// IN-CY-76 Device ID 'I' + 0x76
#define	DEVICE_ID_INCY84	0x4984		// IN-CY-84 Device ID 'I' + 0x84
#define	DEVICE_ID_INCY92	0x4992		// IN-CY-92 Device ID 'I' + 0x92
#define	DEVICE_ID_INCY116	0x6974		// IN-CY-116 Device ID 'i' + 0x74(116)
#define	DEVICE_ID_INCY119	0x6977		// IN-CY-119 Device ID 'i' + 0x77(119)
#define	DEVICE_ID_INCY122	0x697A		// IN-CY-122 Device ID 'i' + 0x7A(122)
#define	DEVICE_ID_DRCY72	0x5072		// DR-CY-72 Device ID 'P' + 0x72
#define	DEVICE_ID_DRCY80	0x4480		// DR-CY-80 Device ID 'D' + 0x80	//by ohshima
#define	DEVICE_ID_DRCY82	0x4482		// DR-CY-82 Device ID 'D' + 0x82
#define	DEVICE_ID_DRCY83	0x4483		// DR-CY-83 Device ID 'D' + 0x83
#define	DEVICE_ID_DRCY132	0x6484		// DR-CY-132 Device ID 'd' + 0x84(132)
#define	DEVICE_ID_DRCY140	0x648C		// DR-CY-140 Device ID 'd' + 0x8C(140)
#define	DEVICE_ID_DRCY143	0x648F		// DR-CY-143 Device ID 'd' + 0x8F(143)
#define	DEVICE_ID_DRCY144	0x6490		// DR-CY-144 Device ID 'd' + 0x90(144)
#define	DEVICE_ID_DRCY149	0x6495		// DR-CY-149 Device ID 'd' + 0x95(149)
#define	DEVICE_ID_DRCY88	0x5088		// DR-CY-88 Device ID 'P' + 0x88
#define	DEVICE_ID_DRCY90	0x5090		// DR-CY-83 Device ID 'P' + 0x90
#define	DEVICE_ID_DRCY95	0x4495		// DR-CY-95 Device ID 'D' + 0x95
//100以降の基板は、00より順にIDを割当てる。100番未満のIDと重なった場合は、スキップする。
#define	DEVICE_ID_DRCY101	0x4400		// DR-CY-101 Device ID 'D' + 0x00
#define	DEVICE_ID_DRCY105	0x4401		// DR-CY-105 Device ID 'D' + 0x01
#define	DEVICE_ID_DRCY118	0x4402		// DR-CY-118 Device ID 'D' + 0x02
#define	DEVICE_ID_DRX120	0x4403		// DR-X-120  Device ID 'D' + 0x03
#define	DEVICE_ID_DRX124	0x4404		// DR-X-124  Device ID 'D' + 0x04
#define	DEVICE_ID_DRX125	0x4405		// DR-X-125  Device ID 'D' + 0x05
#define	DEVICE_ID_DRX127	0x4406		// DR-X-127  Device ID 'D' + 0x06
#define	DEVICE_ID_DRX129	0x4407		// DR-X-129  Device ID 'D' + 0x07
#define	DEVICE_ID_DRX136	0x4408		// DR-X-136  Device ID 'D' + 0x08
#define	DEVICE_ID_DRX147	0x440A		// DR-X-147  Device ID 'D' + 0x0A

#define	DEVICE_ID_MECY35	0x4d35		// ME-CY-35 Device ID 'M' + 0x35

#define	VENDOR_ID_MECY32	0x7604		// ME-CY-32 CUnet board
#define	DEVICE_ID_MECY32	0xF001		// ME-CY-32 CUnet board

#define	VENDOR_ID_INTERFACE	0x1147		// Vendor ID
#define	DEVICE_ID_PCI2724C	0x0AA4		// PCI-2724C/CL/CM

#define	VENDOR_ID_STEPTECHNICA	0x1683	// Vendor ID
#define	DEVICE_ID_CUBPCI		0x0000	// CUB-PCI

//	ＰＣＩコンフィギュレーションレジスタ
typedef union {
	struct {
		unsigned io_access:1;
		unsigned memory_access:1;
		unsigned bus_master:1;
		unsigned special_cycle:1;
		unsigned memory_wi:1;
		unsigned vga_palette:1;
		unsigned parity_error:1;
		unsigned wait_cycle:1;
		unsigned system_error:1;
		unsigned fast_back_to_back:1;
		unsigned reserved:6;
	} bit;
	unsigned short word;
} PCI_COMMAND;

typedef union {
	struct {
		unsigned reserved:5;
		unsigned speed66:1;
		unsigned user_define:1;
		unsigned fast_back_to_back:1;
		unsigned data_parity:1;
		unsigned device_select_timing:2;
		unsigned target_abort_execute:1;
		unsigned target_abort_receive:1;
		unsigned master_abort_receive:1;
		unsigned system_error:1;
		unsigned parity_error:1;
	} bit;
	unsigned short word;
} PCI_STATUS;

typedef union {
	struct {
		unsigned status:4;
		unsigned reserved:2;
		unsigned start:1;
		unsigned suport:1;
	} bit;
	unsigned char byte;
} PCI_BIST;

typedef struct {
	unsigned short	vender_id;
	unsigned short	device_id;
	PCI_COMMAND	command;
	PCI_STATUS	status;
	unsigned char	revision_id;
	struct {
		unsigned char	programing_interface;
		unsigned char	sub;
		unsigned char	base;
	} cl;
	unsigned char	cashe_line;
	unsigned char	latency_timer;
	unsigned char	header_type;
	PCI_BIST	bist;
} PCI_CONFIG;

#ifdef __DOS__
//	ＤＰＭＩ物理アドレスの配置
unsigned long DPMI_PhysicalAddressMapping(unsigned long address, unsigned long size);
#pragma aux DPMI_PhysicalAddressMapping value{eax} =\
	"mov cx,ax"\
	"shr eax,16"\
	"mov bx,ax"\
	"mov di,dx"\
	"shr edx,16"\
	"mov si,dx"\
	"mov ax,0800h"\
	"int 31h"\
	"mov ax,bx"\
	"shl eax,16"\
	"mov ax,cx"\
	parm [eax] [edx] modify [ebx ecx esi edi];
#endif	// __DOS__

//
//	関数
//

#ifdef __cplusplus
extern "C" {
#endif

void ReadConfigHeader(int bus, int device, PCI_CONFIG *p);
unsigned long ReadPCIRegister(int bus, int device, int function, int reg_no);
#ifdef __DOS__
int MemoryMapping(unsigned long address, unsigned long *new_address);
void WritePCIRegister(int bus, int device, int function, int reg_no, unsigned long wdata);
#endif

#ifdef __cplusplus
}
#endif


int	bus_rd(char *);
int	bus_wr(char *);
int	bus_rdw(char *);
int	bus_wrw(char *);
int	bus_rdwh(char *);
int	bus_wrwh(char *);
int	bus_mov(char *, char *);
int	bus_movd(unsigned long *des, unsigned long *src, unsigned int count);


#define	ReadMIO(p)		(*(( volatile unsigned char *)BoardAddress+p))
// #define	ReadMIO(p)		(*(( volatile unsigned int *)BoardAddress+p))
#define	OnMIO(p,d,m)	m|=d;*((unsigned char *)BoardAddress+p)=m
#define	OffMIO(p,d,m)	m&=~d;*((unsigned char *)BoardAddress+p)=m
#define	SetMIO(p,d)		*((unsigned char *)BoardAddress + p) = d
// #define	SetMIO(p,d)		*((unsigned int *)BoardAddress + p) = d

void	R_VmPriPCI( void );
void	R_VmPriVideo( void );
int		R_GetVmPri( void );
void	R_SetVmPri( int Pri );

#endif	// R_PCI_H_
