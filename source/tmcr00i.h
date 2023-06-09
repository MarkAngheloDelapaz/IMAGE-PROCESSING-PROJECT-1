/******************************************************************************

	ΈvO
	Cwb_

									Authered. by	Kazuya Matsunaga
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	π
	Version

******************************************************************************/

// --- define ---


//fq`xAhXθ`
#define BASE_GRAYADDRESS			4
#define BASE2_GRAYADDRESS			5
#define FAIL_GRAYADDRESS			6
#define SAVE_GRAYADDRESS			7
#define BASE3_GRAYADDRESS			8
#define BASE4_GRAYADDRESS			9
#define MANUAL_GRAYADDRESS			10

#define MAX_POINT					1024				//ΕεΐW_

// I/O
#define	RESISTANCE_NG	0x01				// οRlO count sig.
#define	NINSHIKI_NG		0x02				// OΟΈNG count sig.
#define	PILOT_MODE		0x04				// Pilot mode sig.
#define	JUDGE_READ		0x08				// Judge data read sig.
#define	SPEC_SET		0x10				// Spec set sig.

// »θl\’Μ
typedef struct {
	int			nXsize;						// »θl X size
	int			nYsize;						// »θl Y size
	int			nCode;						// NG No.
}	JUDGE_SIZE;
typedef struct {
	int			nData1;						// »θl Data1
	int			nData2;						// »θl Data2
	int			nCode;						// NG No.
}	JUDGE_DATA;

//Έp[^
#define nPSpec	5
typedef struct {
	// --- G2 Insp. »θl
	JUDGE_SIZE		G2Xmin[nPSpec];				// G2 X size Ε¬l[X,Y]
	JUDGE_SIZE		G2Xmax[nPSpec];				// G2 X size Εεl[X,Y]
	JUDGE_SIZE		G2Xrange[nPSpec];			// G2 X max-min[X,Y]
	JUDGE_SIZE		G2Xedge[nPSpec];			// G2 X GbWJPΈ »θl[X,Y]
	JUDGE_SIZE		G2ZureXmin[nPSpec];			// G2 X YΈ »θl[X,Y]
	JUDGE_SIZE		G2ZureXmax[nPSpec];			// G2 X YΈ »θl[X,Y]
	JUDGE_SIZE		G2Ymin[nPSpec];				// G2 Y size Ε¬l[X,Y]
	JUDGE_SIZE		G2Ymax[nPSpec];				// G2 Y size Εεl[X,Y]
	JUDGE_SIZE		G2Yrange[nPSpec];			// G2 Y max-min[X,Y]
	JUDGE_SIZE		G2Yedge[nPSpec];			// G2 Y GbWJPΈ »θl[X,Y]
	JUDGE_SIZE		G2ZureYmin[nPSpec];			// G2 Y YΈ »θl[X,Y]
	JUDGE_SIZE		G2ZureYmax[nPSpec];			// G2 Y YΈ »θl[X,Y]
	JUDGE_SIZE		G2YSdl[nPSpec];				// G2 Y NijimiΈ »θl[X,Y]
	JUDGE_SIZE		G2YApl[nPSpec];				// G2 Y NijimiΈ »θl[X,Y]
	JUDGE_SIZE		OCoatXsize[nPSpec];			// Overcoat X JPΈ »θl[X,Y]
	JUDGE_SIZE		OCoatYsize[nPSpec];			// Overcoat Y JPΈ »θl[X,Y]
	JUDGE_SIZE		OCoatXYsize[nPSpec];		// Overcoat XYJPΈ »θl[X,Y]
	// --- G2 position
	JUDGE_SIZE		G2PosXmin[nPSpec];			// G2 X position min
	JUDGE_SIZE		G2PosXmax[nPSpec];			// G2 X position max
	JUDGE_SIZE		G2PosYmin[nPSpec];			// G2 Y position min
	JUDGE_SIZE		G2PosYmax[nPSpec];			// G2 Y position max
	// --- Plating adhesion
	JUDGE_SIZE		Plating[nPSpec];			// Plating adhesion size[X,Y]
	// --- Chip(Appearence) Insp. »θl
	JUDGE_SIZE		ChipXmin[nPSpec];			// Chip X size Ε¬l[X,Y]
	JUDGE_SIZE		ChipXmax[nPSpec];			// Chip X size Εεl[X,Y]
	JUDGE_SIZE		ChipXrange[nPSpec];			// Chip X max-min[X,Y]
	JUDGE_SIZE		ChipXedge[nPSpec];			// Chip X GbWJPΈ »θl[X,Y]
	JUDGE_SIZE		ChipYmin[nPSpec];			// Chip Y size Ε¬l[X,Y]
	JUDGE_SIZE		ChipYmax[nPSpec];			// Chip Y size Εεl[X,Y]
	JUDGE_SIZE		ChipYrange[nPSpec];			// Chip Y max-min[X,Y]
	JUDGE_SIZE		ChipYedge[nPSpec];			// Chip Y GbWJPΈ »θl[X,Y]
	// --- Electrode Insp.
	JUDGE_SIZE		ElecXmin[nPSpec];			// dΙ‘@ XΕ¬l[X,Y]
	JUDGE_SIZE		ElecXmax[nPSpec];			// dΙ‘@ XΕεl[X,Y]
	JUDGE_SIZE		ElecXrange[nPSpec];			// dΙ‘@ X max-min[X,Y]
	JUDGE_SIZE		ElecXedge[nPSpec];			// dΙ‘@ X GbWJPΈ »θl[X,Y]
	JUDGE_SIZE		ElecYmin[nPSpec];			// dΙ‘@ YΕ¬l[X,Y]
	JUDGE_SIZE		ElecYmax[nPSpec];			// dΙ‘@ YΕεl[X,Y]
	JUDGE_SIZE		ElecXsub[nPSpec];			// dΙΆE‘@·[X,Y]
	JUDGE_SIZE		ElecYrange[nPSpec];			// dΙ‘@ Y max-min[X,Y]
	JUDGE_SIZE		ElecYedge[nPSpec];			// dΙ‘@ Y GbWJPΈ »θl[X,Y]
	JUDGE_SIZE		ElecKake[nPSpec];			// dΙJPΈ»θl
  JUDGE_SIZE		ElecKasure[nPSpec];			// dΙJPΈ»θl
	JUDGE_DATA		ElecBlack[nPSpec];			// dΙΟF»θl(%)
	JUDGE_SIZE		ElecC2Xmin[nPSpec];			// C2¬[X,Y]
	JUDGE_SIZE		ElecC1Mezumari[nPSpec];		// C1Ϊlάθ[X,Y]
	JUDGE_SIZE		SideElecXmin[nPSpec];		// €ΚdΙνθέ‘@ XΕ¬l[X,Y]
	JUDGE_DATA		MarkLevel[nPSpec];			// Wσox
	JUDGE_DATA		MarkShift[nPSpec];			// WσΖ
	JUDGE_SIZE		IbutsuXsize[nPSpec];		// Glass Ibutsu  X Έ »θl[X,Y]
	JUDGE_SIZE		IbutsuYsize[nPSpec];		// Glass Ibutsu  Y Έ »θl[X,Y]
	// --- system
	JUDGE_DATA		Focus;						// YΕ_
	JUDGE_DATA		Bright;						// ΖxΗ
}	INSPECT_PARAMETER;


R_TRANS_DATA			IndexTrans;											// R_TRANS_DATA\’Μ

// --- prototype ---
extern  int				Ma_pos[4];											//WσΚuof[^( lex, rix, upy, loy )
extern  int             TWA_R_n;                    						//οRlΆ F/J


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
