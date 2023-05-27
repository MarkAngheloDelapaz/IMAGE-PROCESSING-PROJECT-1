/******************************************************************************

	検査プログラム
	メインヘッダ

									Authered. by	Kazuya Matsunaga
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version

******************************************************************************/

// --- define ---


//ＧＲＡＹアドレス定義
#define BASE_GRAYADDRESS			4
#define BASE2_GRAYADDRESS			5
#define FAIL_GRAYADDRESS			6
#define SAVE_GRAYADDRESS			7
#define BASE3_GRAYADDRESS			8
#define BASE4_GRAYADDRESS			9
#define MANUAL_GRAYADDRESS			10

#define MAX_POINT					1024				//最大座標点数

// I/O
#define	RESISTANCE_NG	0x01				// 抵抗値％外 count sig.
#define	NINSHIKI_NG		0x02				// 外観検査NG count sig.
#define	PILOT_MODE		0x04				// Pilot mode sig.
#define	JUDGE_READ		0x08				// Judge data read sig.
#define	SPEC_SET		0x10				// Spec set sig.

// 判定値構造体
typedef struct {
	int			nXsize;						// 判定値 X size
	int			nYsize;						// 判定値 Y size
	int			nCode;						// NG No.
}	JUDGE_SIZE;
typedef struct {
	int			nData1;						// 判定値 Data1
	int			nData2;						// 判定値 Data2
	int			nCode;						// NG No.
}	JUDGE_DATA;

//検査パラメータ
#define nPSpec	5
typedef struct {
	// --- G2 Insp. 判定値
	JUDGE_SIZE		G2Xmin[nPSpec];				// G2 X size 最小値[X,Y]
	JUDGE_SIZE		G2Xmax[nPSpec];				// G2 X size 最大値[X,Y]
	JUDGE_SIZE		G2Xrange[nPSpec];			// G2 X max-min[X,Y]
	JUDGE_SIZE		G2Xedge[nPSpec];			// G2 X エッジカケ検査 判定値[X,Y]
	JUDGE_SIZE		G2ZureXmin[nPSpec];			// G2 X ズレ検査 判定値[X,Y]
	JUDGE_SIZE		G2ZureXmax[nPSpec];			// G2 X ズレ検査 判定値[X,Y]
	JUDGE_SIZE		G2Ymin[nPSpec];				// G2 Y size 最小値[X,Y]
	JUDGE_SIZE		G2Ymax[nPSpec];				// G2 Y size 最大値[X,Y]
	JUDGE_SIZE		G2Yrange[nPSpec];			// G2 Y max-min[X,Y]
	JUDGE_SIZE		G2Yedge[nPSpec];			// G2 Y エッジカケ検査 判定値[X,Y]
	JUDGE_SIZE		G2ZureYmin[nPSpec];			// G2 Y ズレ検査 判定値[X,Y]
	JUDGE_SIZE		G2ZureYmax[nPSpec];			// G2 Y ズレ検査 判定値[X,Y]
	JUDGE_SIZE		G2YSdl[nPSpec];				// G2 Y Nijimi検査 判定値[X,Y]
	JUDGE_SIZE		G2YApl[nPSpec];				// G2 Y Nijimi検査 判定値[X,Y]
	JUDGE_SIZE		OCoatXsize[nPSpec];			// Overcoat X カケ検査 判定値[X,Y]
	JUDGE_SIZE		OCoatYsize[nPSpec];			// Overcoat Y カケ検査 判定値[X,Y]
	JUDGE_SIZE		OCoatXYsize[nPSpec];		// Overcoat XYカケ検査 判定値[X,Y]
	// --- G2 position
	JUDGE_SIZE		G2PosXmin[nPSpec];			// G2 X position min
	JUDGE_SIZE		G2PosXmax[nPSpec];			// G2 X position max
	JUDGE_SIZE		G2PosYmin[nPSpec];			// G2 Y position min
	JUDGE_SIZE		G2PosYmax[nPSpec];			// G2 Y position max
	// --- Plating adhesion
	JUDGE_SIZE		Plating[nPSpec];			// Plating adhesion size[X,Y]
	// --- Chip(Appearence) Insp. 判定値
	JUDGE_SIZE		ChipXmin[nPSpec];			// Chip X size 最小値[X,Y]
	JUDGE_SIZE		ChipXmax[nPSpec];			// Chip X size 最大値[X,Y]
	JUDGE_SIZE		ChipXrange[nPSpec];			// Chip X max-min[X,Y]
	JUDGE_SIZE		ChipXedge[nPSpec];			// Chip X エッジカケ検査 判定値[X,Y]
	JUDGE_SIZE		ChipYmin[nPSpec];			// Chip Y size 最小値[X,Y]
	JUDGE_SIZE		ChipYmax[nPSpec];			// Chip Y size 最大値[X,Y]
	JUDGE_SIZE		ChipYrange[nPSpec];			// Chip Y max-min[X,Y]
	JUDGE_SIZE		ChipYedge[nPSpec];			// Chip Y エッジカケ検査 判定値[X,Y]
	// --- Electrode Insp.
	JUDGE_SIZE		ElecXmin[nPSpec];			// 電極寸法 X最小値[X,Y]
	JUDGE_SIZE		ElecXmax[nPSpec];			// 電極寸法 X最大値[X,Y]
	JUDGE_SIZE		ElecXrange[nPSpec];			// 電極寸法 X max-min[X,Y]
	JUDGE_SIZE		ElecXedge[nPSpec];			// 電極寸法 X エッジカケ検査 判定値[X,Y]
	JUDGE_SIZE		ElecYmin[nPSpec];			// 電極寸法 Y最小値[X,Y]
	JUDGE_SIZE		ElecYmax[nPSpec];			// 電極寸法 Y最大値[X,Y]
	JUDGE_SIZE		ElecXsub[nPSpec];			// 電極幅左右寸法差[X,Y]
	JUDGE_SIZE		ElecYrange[nPSpec];			// 電極寸法 Y max-min[X,Y]
	JUDGE_SIZE		ElecYedge[nPSpec];			// 電極寸法 Y エッジカケ検査 判定値[X,Y]
	JUDGE_SIZE		ElecKake[nPSpec];			// 電極カケ検査判定値
  JUDGE_SIZE		ElecKasure[nPSpec];			// 電極カケ検査判定値
	JUDGE_DATA		ElecBlack[nPSpec];			// 電極変色判定値(%)
	JUDGE_SIZE		ElecC2Xmin[nPSpec];			// C2小[X,Y]
	JUDGE_SIZE		ElecC1Mezumari[nPSpec];		// C1目詰まり[X,Y]
	JUDGE_SIZE		SideElecXmin[nPSpec];		// 側面電極被り込み寸法 X最小値[X,Y]
	JUDGE_DATA		MarkLevel[nPSpec];			// 標印検出レベル
	JUDGE_DATA		MarkShift[nPSpec];			// 標印と
	JUDGE_SIZE		IbutsuXsize[nPSpec];		// Glass Ibutsu  X 検査 判定値[X,Y]
	JUDGE_SIZE		IbutsuYsize[nPSpec];		// Glass Ibutsu  Y 検査 判定値[X,Y]
	// --- system
	JUDGE_DATA		Focus;						// レンズ焦点
	JUDGE_DATA		Bright;						// 照度管理
}	INSPECT_PARAMETER;


R_TRANS_DATA			IndexTrans;											// R_TRANS_DATA構造体

// --- prototype ---
extern  int				Ma_pos[4];											//標印位置検出データ( lex, rix, upy, loy )
extern  int             TWA_R_n;                    						//抵抗値文字数 F/J


int		InspBinPn;          	// Inspection		bin memory
int		Inspect( R_CS_RESULT_DATA *pCsResult );
int		TP_CheckBright( void );
int		TP_SearchGuide( void );
int		TP_SearchGuideFmRight( void );
int		TP_IndexTableBright( void );
int		TP_SearchElect( void );
int		SearchChipPos( void );
int		InspReverse( void );
int		InspChipSize( void );
int		InspElectrode( void );
int		InspElecKasure( void );
int		InspOvercoat( void );
int		InspPlating( void );
int		Pinhole_insp(void);
int		G2EsLevelSet( int side );
int		InspOvercoatBlack( void );						//v2.11

int nDispMode1; //9499 mark match display
static int instru; //9499
static int ratex; //9499
static int ratey; //9499
static int compare; //9499
extern int             	IGuidePos[2];						// Index guide position
extern int				InspGrayPn;                 	// Inspection		gray memory
