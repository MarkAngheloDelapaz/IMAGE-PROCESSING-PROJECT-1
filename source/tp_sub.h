/******************************************************************************

	�s�o��p�̊֐�

	system name		�s�o��p�֐�
	program name	"TP_SUB.C"

	����
	history
	<version>		<date>			<name>			<comment>
	Ver. X.01		2008-02-14		M.Takahashi		new


******************************************************************************/


// --- define ---

typedef struct{
	int		nRateX;						// �w��������\(1000�{�l)
	int		nRateY;						// �x��������\(1000�{�l)
	int		nShutterSpeed;				// �V���b�^�X�s�[�h(us)
	int		nCaptureTimer;				// ��荞�ݑO�^�C�}(us)
	int		nCameraName;				// �J������	2017.07.11  tanaka BASLER acA2000-340km-TRIMVGA suport
} MACHINE_PARAMETER;



typedef struct{
	char	Date[128];
	char	Vendor[128];
	char	Serial[128];
}USB_KEY_INFO;

USB_KEY_INFO UsbKeyInfo;			//2017.06.13

//�I�v�V�����R�[�h
#define NO_DRCY80		0x0001
#define NO_INCY83		0x0002
#define NO_DRCY84		0x0004
#define NO_INCY76		0x0008			//2010.01.07 by saito
#define NO_INCY119		0x0080			//<z1> 2021.06.10<shio>
#define NO_USBKEY		0x0010			//2017.04.14 by tanaka
#define IO_INCY122		0x0020			//2017.07.11 by tanaka for NWT
#define NO_DRX124		0x0040			//2017/07/11 by tanaka
#define DEBUG_MODE		0x1000
#define BACKINSP_MODE	0x2000			//���ʌ������[�h
#define MECHA_CHECK		0x4000			//�ƎҒ����p���[�h�i�����㔼���������Ȃ��j
#define ADMIN_MODE		0x8000
#define REPI            0x0100          // REPI barcode

// I/O
#define START			0x01			// input
#define LOT_END			0x02			// "
#define ALARM_RESET		0x04			// "
#define JUDGE			0x01			// "  <GO:low, NG:high>
#define BUSY			0x02			// output
#define LOCK			0x04			// "
#define ALARM			0x08			// "  <active high>
#define SAVEBUSY		0x10			// "

#define BUSY_TR			0x01			// output
#define JUDGE_TR		0x04			// "  <GO:low, NG:high>

#define SPEC_SET		0x10			// "
// I/O
#define INSP_START		0x01				// Insp. start
#define POK_SIG		    0x02				// Cancle Poket  //2012.04.19
#define	RESISTANCE_NG	0x01				// ��R�l���O count sig.
#define	NINSHIKI_NG		0x02				// �O�ό���NG count sig.
#define	PILOT_MODE		0x04				// Pilot mode sig.
#define	JUDGE_READ		0x08				// Judge data read sig.
#define	SPEC_SET		0x10				// Spec set sig.

#define NumMarkChar     12

//NG�R�[�h
enum	NG_CODE {
//												//	�R�[�h1�͗\��ς݂Ȃ̂Ŏg�p�֎~(TWL.H�Ő錾�ς�)
					// Insp.
//						NG_NO_WORK		= 1,	//  1: ���[�N�Ȃ�
						NG_CANT_INSP	= 2,	//  2: Can't Insp.
						NG_INDEX_POS,			//  3: Index pos. Error
						NG_TAPE_POS,			//  4: Tape pos. Error
						NG_REVERSE,				//  5: Chip Reverse
						NG_FOCUS,				//  6: Focus Error
					// G2 Insp.
						NG_G2_X_SIZE	= 11,	// 11: G2 X size
						NG_G2_Y_SIZE,			// 12: G2 Y size
						NG_G2_KAKE_X,			// 13: G2 Kake X
						NG_G2_KAKE_Y,			// 14: G2 Kake Y
						NG_G2_POS_X,			// 15: G2 position X Error
						NG_G2_POS_Y,			// 16: G2 position Y Error
						NG_OCOAT_X,				// 17: Overcoat kake X
						NG_OCOAT_Y,				// 18: Overcoat kake Y
						NG_OCOAT_XY,			// 19: Overcoat kake Y
					// Chip(Appearence) Insp.
						NG_CHIP_X_SIZE	= 21,	// 21: CHIP X size
						NG_CHIP_Y_SIZE,			// 22: CHIP Y size
						NG_CHIP_KAKE_X,			// 23: CHIP Kake X
						NG_CHIP_KAKE_Y,			// 24: CHIP Kake Y
						NG_CHIP_CORNER,			// 25: CHIP Kake corner
					// Plating adhesion
						NG_PL_ADHESION	= 31,	// 31: Plating adhesion Error
					// Marking Insp.
						NG_MARKING		= 41,	// 41: Marking Error
						NG_MARK_SEGMENT,		// 42: Mark segment Error
					// Electrode Insp.
						NG_ELEC_X_SIZE	= 51,	// 51: Elec. X size
						NG_ELEC_Y_SIZE,			// 52: Elec. Y size
						NG_ELEC_EDGE_X,			// 53: Elec. edge Kake
						NG_ELEC_EDGE_Y,			// 54: Elec. edge Kake
						NG_ELEC_KAKE,			// 55: Elec. Kake
						NG_ELEC_BLACK,			// 56: Elec. black Error
						NG_SIDE_ELEC,			// 57: Side Elec. X size
						NG_GLASS_IBUTSU,		// 58: Glass IBUTSU
					// M/C NG box
						MC_RESISTANCE	= 81,	// 81: M/C NG box Resistance
                        MC_NINSHIKI 			// 82: M/C NG box Ninshiki
	};
//�R�[�h98,99�͗\��ς݂Ȃ̂Ŏg�p�֎~(TWL.H�Ő錾�ς�)
//#define NG_LIGHTING			97		//�Ɩ��G���[
//#define NG_CAPTURE			98		//�摜��荞�݃G���[
//#define NG_INSP				99		//�F���G���[

#define COMMAND_BUFF	256				//�R�}���h����M�o�b�t�@�T�C�Y


#define GPIO_DSW				_BOARD_RDSW_0							// �Ɩ����DSW�ԍ�
#define OutputBusyOn()			R_gpio_on( GPIO_DSW|CNa_OUT_port_1, BUSY)
#define OutputBusyOff()			R_gpio_off(GPIO_DSW|CNa_OUT_port_1, BUSY)
#define OutputLockOn()			R_gpio_on( GPIO_DSW|CNa_OUT_port_1, LOCK)
#define OutputLockOff()			R_gpio_off(GPIO_DSW|CNa_OUT_port_1, LOCK)
#define OutputJudgeGo()			R_gpio_on( GPIO_DSW|CNa_OUT_port_1, JUDGE)
#define OutputJudgeNg()			R_gpio_off(GPIO_DSW|CNa_OUT_port_1, JUDGE)
#define OutputAlarmOn()			R_gpio_on( GPIO_DSW|CNa_OUT_port_1, ALARM)
#define OutputAlarmOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_1, ALARM)
#define OutputSaveBusyOn()		R_gpio_on( GPIO_DSW|CNa_OUT_port_1, SAVEBUSY)
#define OutputSaveBusyOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_1, SAVEBUSY)

#define OutputJudgeGoTR()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, JUDGE_TR)
#define OutputJudgeNgTR()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, JUDGE_TR)
#define OutputBusyOnTR()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, BUSY_TR)
#define OutputBusyOffTR()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, BUSY_TR)

#define OutputPilotOn()			R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x01)
#define OutputPilotOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x01)
#define OutputNGAlarmOn()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x02)
#define OutputNGAlarmOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x02)
#define OutputGLModeOn()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x04)
#define OutputGLModeOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x04)
#define OutputReverseOn()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x10)
#define OutputReverseOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x10)
#define OutputNoworkOn76()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x20)		//<shio>
#define OutputNoworkOff76()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x20)		//<shio>

#define OutputNoworkOn119()		R_gpio_on( Incy119Ch|CNa_OUT_port_1, 0x04) //<z1>
#define OutputNoworkOff119()	R_gpio_off(Incy119Ch|CNa_OUT_port_1, 0x04) //<z1>


// --- Product name
extern char			ProductName[32];				// ���i��
// --- Inspection name
extern char			InspectionName[32];				// ������
// --- Program version ---
extern char			ProgramVer[32];					// Program Version
//�@��No.
extern char			MachineName[32];				// �@�䖼




// --- Bin level set ---
typedef struct{
	int		nGrayPn;						// ����Gray memory No.
	int		nXmin;							// �v��Area X min
	int		nYmin;							// �v��Area Y min
	int		nXmax;							// �v��Area X max
	int		nYmax;							// �v��Area Y max
	int		nStartLevel;					// �q�X�g�O�����̌v�Z�͈͂̊J�n�_(start>=0)
	int		nEndLevel;						// �q�X�g�O�����̌v�Z�͈͂̏I���_
	int		nMethod;						// �������l�I��̕��@
	int		nLevel;							// �Q�l�����x��
}	THRESHOLD_PARAMETER;

// --- Gray to Bin level ---
typedef struct{
	int		nData;							// Bin level data
	int		nOffset;						// Bin level offset
	int		nLower;							// Bin level lower limit
	int		nUpper;							// Bin level upper limit
}	BIN_LEVEL_PARAMETER;

// --- Edge search set ---
typedef struct{
	int		nGrayPn;						// ����Gray memory No.
	int		nXmin;							// �v��Area X min
	int		nYmin;							// �v��Area Y min
	int		nXmax;							// �v��Area X max
	int		nYmax;							// �v��Area Y max
	int		nLvelLo;						// ���Â̍��̍Œ�l
	int		nAv;							// ���ω���
	int		nNoise;							// �m�C�Y��
	int		nSlice;							// �X���C�X���x��
	int		nDeg;							// ���o����
	int		nWidth;							// ���o��
	int		nContrast;						// �G�b�W�̎�ށi�Â������邢�FES_BLACK�A���邢���Â�:ES_WHITE�j
	int		nPos[700][3];					// ���o�G�b�W�f�[�^( X,Y,temp )
	int		nFreq;							// �ŕp�l
}	EDGE_SEARCH_PARAMETER;

// --- Bin Filter condition ---
typedef struct{
	int		nBinPn;							// ����bin memory No.
	int		nXmin;							// �����͈� X min
	int		nYmin;							// �����͈� Y min
	int		nXmax;							// �����͈� X max
	int		nYmax;							// �����͈� Y max
}	BIN_FILTER_PARAMETER;

// --- �Q�l�v�������i �����ʉ��Z�@���Z�����w�� �j ---
typedef struct{
	int		nMode;							// ���Z���[�h( 0:RM_NORMAL �ʏ퉉�Z  1:RM_RUN ���胉���T�C�Y�������Z  2:RM_AREA ����ʐψȉ��������Z )
	int		nRun;							// �ҏW�Ώۃ����T�C�Y
	int		nArea;							// �����Ώۖʐϒl
	int		nColor;							// �����ΏېF
}	BIN_MEASURE_PARAMETER;

// --- �W�󌟍� ---
typedef struct{
	int		marking_xsize;					// �W�󉡃T�C�Y
	int		marking_ysize;					// �W��c�T�C�Y
} INSPECT_MARK_PARAMETER;

// --- �O���[�T�[�`�p�p�����[�^�\���� ---
typedef struct {
	int		pat_no;							// �p�^�[������
	int		middle_lower;
	int		last_lower;
	int		complex;
	int		resolution;
	R_BOX	search_window;					// �T�[�`�G���A
} GS_PARAMETER;


enum	GrayPlaneNo {
						GrayP0		= 0,		// GRAY_PLANE0	�O���[�O�y�[�W�i�\���������[�j
						GrayP1,
						GrayP2,
						GrayP3,
						GrayP4,
						GrayP5,
						GrayP6,
						GrayP7,
						GrayP8,
                        GrayP9
};

enum	BinPlaneNo {
						BinP0		= 0,		// BIN_PLANE0	�o�C�i���O�y�[�W�i�\���������[�j
						BinP1,
						BinP2,
						BinP3,
						BinP4,
						BinP5,
						BinP6,
						BinP7,
						BinP8,
                        BinP9
};

#define TWA_TAPE		0
#define TWA_INDEX		1

#define ImageCamera		0
#define ImageMemory		1
#define ImageFail		2
#define	ImageFile		3

// Class
#define CLASS_D			 0				// D��
#define CLASS_F			 1				// F��
#define CLASS_J			 2				// J��

#define	TWA_Scallop		0
#define	TWA_Rectangle	1

#define	PosXmin			0
#define	PosYmin			1
#define	PosXmax			2
#define	PosYmax			3

#define	DataXum			0
#define	DataYum			1
#define	DataXbit		2
#define	DataYbit		3

#define	DataX			0
#define	DataY			1

#define ColorBlack		0
#define ColorWhite		1

#define	DataLeft		0
#define	DataRight		1
#define	DataLR			2
#define DataUpper		0
#define DataLower       1
#define DataUL			2

#define	PosLup			0
#define	PosLlo			1
#define	PosRup			2
#define	PosRlo			3

#define	PilotFlag		Pilot_mode

#define	SpecGlass		0
#define	SpecNormal		0
#define	SpecResin		1
#define	SpecJPW			2
#define	SpecSpG			3
#define	SpecGlassSp		3
#define	SpecMCL			3
#define	SpecSpJ			4
#define	SpecResinSp		4
//#define	SpecJPWSp		5

#define	TWA_G2			1

#define	Insp_Normal		0
#define	Insp_Hyouri		1
#define	Insp_Pinhole	2
#define	Insp_Pilot		3

#define	Insp_Low		0
#define	Insp_High		1
#define	Insp_High2		2

// ��A�W���E���ϒl�E�W���΍�
typedef struct	{
    int		dataX[700];														// Chip edge search data X
    int		dataY[700];														// Chip edge search data Y
    int		dataR[700];														// Chip edge data result
//	double	Mean;															// ���ϒl
//	double	SD;																// �W���΍�
    double	MeanX;															// �w�f�[�^���ϒl
    double	MeanY;															// �x�f�[�^���ϒl
    double	Rev;															// ��A�W��
    int		Result;															// �v�Z����
} Regression_def;//		ChipEdge[4], ChipXside, ChipYside, ElecEdge[3][8];


typedef struct	{
    int		BlobNo;												// blob No.( bin measure )
    int		CornerPosX[4];										// X pos. corner(pos.)
    int		CornerPosY[4];										// Y pos. corner(pos.)
    int		Pos[4];												// position
}	Overcoat_data;

typedef struct	{
	float	kaiki_ex;														//�w�f�[�^���ϒl
	float	kaiki_ey;														//�x�f�[�^���ϒl
	float	kaiki_b;														//��A�W��
} KAIKI;

typedef struct {
	float	mean;															//���ϒl
	float	sd;																//�W���΍�
	float	mean2;															//���ϒl�i2sd�ȉ��j
	float	sd2;															//�W���΍��i2sd�ȉ��j
} MEANSD;

extern int				Bin_level;
extern int				Channel;
extern int				Vmode;
extern int				TWA_X_size;
extern int				TWA_Y_size;
extern int				TWA_trigger_mode;
extern double			CCD_x;
extern int				Pilot_mode;											//�p�C���b�g���[�h
extern int				Incy76Ch;											// IN-CY-76 DSW data
extern int				Incy119Ch;										// IN-CY-119 DSW data //<z1>
extern int				Drcy80Ch;											// DR-CY-80 DSW data
extern int              Option;

//�d�l ( 0:�ʏ� 1:�\�� 2:�s���z�[������ 3:�p�C���b�g )
extern int				TWA_Insp_mode;                                      //�d�l ( 0:�ʏ� 1:�\�� 2:�s���z�[������ 3:�p�C���b�g )
extern int				TWA_Spec;                                           //���i�d�l( 0:Normal 1:NP3 2:Jpw)
extern int				TWA_Insp_level;                                     //�������[�h�i 0:�ʏ� 1:�ڍ� �j
extern int				TWA_Mark_Insp;                                      //�W�󌟍�

extern int              bIsMCR01;                                           // MCR01����
extern int              bIsMCR03;                                           // MCR03����
extern int              bIsMCR10;                                           // MCR10����
extern int              bIsMCR18;                                           // MCR18����
extern int              bIsESR25;                                           // ESR25����
extern int              bIsSFR25;                                           // SFR25����
extern int				bIsLTR50;											// LTR50����
extern int				bIsLTR18;											// LTR18����									//v2.20
extern int				bIsG2Straight;										// G2�����i�i�O�`�ƐڐG������́j
extern int				bIsG2Separate;										// G2�K���X�i�Ǝ����Ōʕi

extern char				ProductName[32];									// ���i��
extern char				MachineName[32];									// �@��No.

extern          char	Comment[2048];										// Comment Buffer
extern          char	Comment1[200];										// Comment Buffer
extern          char	Comment2[200];										// Comment Buffer


// �d�l�ݒ�f�[�^
//char					ProdSpecData[20];									// �d�l�ݒ�Data
//static	char			ProdSpecDataBak[20];								// �d�l�ݒ�Data backup
//static	int				ProdSpecSet			= ERROR;						// �d�l�ݒ�


void	TWA_Zoom_set( int level, int zoom_std );
void	TWA_Focus_set( int bin_l, int Judge_F );
void	TWA_rs_cle( void );
void	TWA_kaiten( int *x, int *y, int deg );

void	TWA_es_initial( int page, int level, int av, int noise );
int		TWA_es_search( int deg, int *xs, int *ys, int l, int contrast );
void	TWA_manu_video_in( int level );
int		TWA_ID_check( void );
int		DrawMessage( char mes[], int color, int mode, int pos );
void	DrawMessageOff( void );
void	BinClsPn( int binPn, int xs, int ys, int xe, int ye );
void	GrayClsPn( int grayPn, int xs, int ys, int xe, int ye );

void	FilterBinContrct( BIN_FILTER_PARAMETER *pFilter, int xsize, int ysize );
void	FilterBinExpand( BIN_FILTER_PARAMETER *pFilter, int xsize, int ysize );
int		SetsubiDataLoadfile( void );
void	SetsubiDataCheck( void );
void	SetCounter( void );
void	SetCounterTape( void );

int		CheckInspStart( void );
int		CheckInspStartNwt( void );
int		CheckResistanceNG( void );
int		CheckNinshikiNG( void );
int		CheckPilotMode( void );
int		CheckJudgeRead( void );
int		CheckSpecSet( void );
//int		CheckPilotSet( void );

void    Print1( void );
int     CanclePok_sig( void );       //2012.04.19
void	WaitMouseClick( void );
void	SetsubiDataPrintBusy( int mode );
int		SetsubiDataPrint( void );
void	SetsubiDataReadBusy( int mode );
void	SetsubiDataRead( int dn );
int		SetsubiDataWrite( void );
int		NinshikiDataWrite( void );
void TWA_mark_set_m03( void );
void TWA_mark_set_m10( void );
void TWA_mark_set_e25( void );


typedef struct{
    int 	nTime[20];
    RTIME	tTime;
    char	cComment[20][30];
}TWA_TIME_RESULT;

extern TWA_TIME_RESULT twaTime[2];

#ifdef __DOS__
    extern struct	dosdate_t		Lot_StartDate;										// ���b�g�J�n Date
    extern struct	dostime_t		Lot_StartTime;										// ���b�g�J�n Time
#elif __GNUC__
    extern time_t					ctLotStart;
    extern struct	tm				Lot_StartDateTime;									// ���b�g�J�n Date,Time
#endif



void twa_TimeStart( TWA_TIME_RESULT *pPara );
void twa_TimeRead( TWA_TIME_RESULT *pPara, int nNumber );
int twa_TimeDisp( TWA_TIME_RESULT *pPara );
int UsbKeyCertification_rist( USB_KEY_INFO *UsbKeyInfo );     //2017.06.13
int UsbKeyCertification( char *UsbKeyInformation, int SChange );
void	TWA_expand2( int mem_no, int x_size, int y_size, int pt[] );
void	TWA_contrct2( int mem_no, int x_size, int y_size, int pt[] );
//int		TWA_es_bin_search_n( int p, int *rx, int *ry, int deg, int l, int flag, int nm );
void	TWA_open_switch( char *str[], int cn );
void	TWA_close_switch( int cn );

void	TWA_gray2bin( int level, int xs, int ys, int xl, int yl, int gn, int bn );
void	TWA_bin_cls( int p, int x1, int y1, int x2, int y2 );

