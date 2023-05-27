/******************************************************************************

	�����v���O����
	���C���w�b�_

									Authered. by	Kazuya Matsunaga
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version

******************************************************************************/

// --- define ---


//�f�q�`�x�A�h���X��`
#define BASE_GRAYADDRESS			4
#define BASE2_GRAYADDRESS			5
#define FAIL_GRAYADDRESS			6
#define SAVE_GRAYADDRESS			7
#define BASE3_GRAYADDRESS			8
#define BASE4_GRAYADDRESS			9
#define MANUAL_GRAYADDRESS			10

#define MAX_POINT					1024				//�ő���W�_��

// I/O
#define	RESISTANCE_NG	0x01				// ��R�l���O count sig.
#define	NINSHIKI_NG		0x02				// �O�ό���NG count sig.
#define	PILOT_MODE		0x04				// Pilot mode sig.
#define	JUDGE_READ		0x08				// Judge data read sig.
#define	SPEC_SET		0x10				// Spec set sig.

// ����l�\����
typedef struct {
	int			nXsize;						// ����l X size
	int			nYsize;						// ����l Y size
	int			nCode;						// NG No.
}	JUDGE_SIZE;
typedef struct {
	int			nData1;						// ����l Data1
	int			nData2;						// ����l Data2
	int			nCode;						// NG No.
}	JUDGE_DATA;

//�����p�����[�^
#define nPSpec	5
typedef struct {
	// --- G2 Insp. ����l
	JUDGE_SIZE		G2Xmin[nPSpec];				// G2 X size �ŏ��l[X,Y]
	JUDGE_SIZE		G2Xmax[nPSpec];				// G2 X size �ő�l[X,Y]
	JUDGE_SIZE		G2Xrange[nPSpec];			// G2 X max-min[X,Y]
	JUDGE_SIZE		G2Xedge[nPSpec];			// G2 X �G�b�W�J�P���� ����l[X,Y]
	JUDGE_SIZE		G2ZureXmin[nPSpec];			// G2 X �Y������ ����l[X,Y]
	JUDGE_SIZE		G2ZureXmax[nPSpec];			// G2 X �Y������ ����l[X,Y]
	JUDGE_SIZE		G2Ymin[nPSpec];				// G2 Y size �ŏ��l[X,Y]
	JUDGE_SIZE		G2Ymax[nPSpec];				// G2 Y size �ő�l[X,Y]
	JUDGE_SIZE		G2Yrange[nPSpec];			// G2 Y max-min[X,Y]
	JUDGE_SIZE		G2Yedge[nPSpec];			// G2 Y �G�b�W�J�P���� ����l[X,Y]
	JUDGE_SIZE		G2ZureYmin[nPSpec];			// G2 Y �Y������ ����l[X,Y]
	JUDGE_SIZE		G2ZureYmax[nPSpec];			// G2 Y �Y������ ����l[X,Y]
	JUDGE_SIZE		G2YSdl[nPSpec];				// G2 Y Nijimi���� ����l[X,Y]
	JUDGE_SIZE		G2YApl[nPSpec];				// G2 Y Nijimi���� ����l[X,Y]
	JUDGE_SIZE		OCoatXsize[nPSpec];			// Overcoat X �J�P���� ����l[X,Y]
	JUDGE_SIZE		OCoatYsize[nPSpec];			// Overcoat Y �J�P���� ����l[X,Y]
	JUDGE_SIZE		OCoatXYsize[nPSpec];		// Overcoat XY�J�P���� ����l[X,Y]
	// --- G2 position
	JUDGE_SIZE		G2PosXmin[nPSpec];			// G2 X position min
	JUDGE_SIZE		G2PosXmax[nPSpec];			// G2 X position max
	JUDGE_SIZE		G2PosYmin[nPSpec];			// G2 Y position min
	JUDGE_SIZE		G2PosYmax[nPSpec];			// G2 Y position max
	// --- Plating adhesion
	JUDGE_SIZE		Plating[nPSpec];			// Plating adhesion size[X,Y]
	// --- Chip(Appearence) Insp. ����l
	JUDGE_SIZE		ChipXmin[nPSpec];			// Chip X size �ŏ��l[X,Y]
	JUDGE_SIZE		ChipXmax[nPSpec];			// Chip X size �ő�l[X,Y]
	JUDGE_SIZE		ChipXrange[nPSpec];			// Chip X max-min[X,Y]
	JUDGE_SIZE		ChipXedge[nPSpec];			// Chip X �G�b�W�J�P���� ����l[X,Y]
	JUDGE_SIZE		ChipYmin[nPSpec];			// Chip Y size �ŏ��l[X,Y]
	JUDGE_SIZE		ChipYmax[nPSpec];			// Chip Y size �ő�l[X,Y]
	JUDGE_SIZE		ChipYrange[nPSpec];			// Chip Y max-min[X,Y]
	JUDGE_SIZE		ChipYedge[nPSpec];			// Chip Y �G�b�W�J�P���� ����l[X,Y]
	// --- Electrode Insp.
	JUDGE_SIZE		ElecXmin[nPSpec];			// �d�ɐ��@ X�ŏ��l[X,Y]
	JUDGE_SIZE		ElecXmax[nPSpec];			// �d�ɐ��@ X�ő�l[X,Y]
	JUDGE_SIZE		ElecXrange[nPSpec];			// �d�ɐ��@ X max-min[X,Y]
	JUDGE_SIZE		ElecXedge[nPSpec];			// �d�ɐ��@ X �G�b�W�J�P���� ����l[X,Y]
	JUDGE_SIZE		ElecYmin[nPSpec];			// �d�ɐ��@ Y�ŏ��l[X,Y]
	JUDGE_SIZE		ElecYmax[nPSpec];			// �d�ɐ��@ Y�ő�l[X,Y]
	JUDGE_SIZE		ElecXsub[nPSpec];			// �d�ɕ����E���@��[X,Y]
	JUDGE_SIZE		ElecYrange[nPSpec];			// �d�ɐ��@ Y max-min[X,Y]
	JUDGE_SIZE		ElecYedge[nPSpec];			// �d�ɐ��@ Y �G�b�W�J�P���� ����l[X,Y]
	JUDGE_SIZE		ElecKake[nPSpec];			// �d�ɃJ�P��������l
  JUDGE_SIZE		ElecKasure[nPSpec];			// �d�ɃJ�P��������l
	JUDGE_DATA		ElecBlack[nPSpec];			// �d�ɕϐF����l(%)
	JUDGE_SIZE		ElecC2Xmin[nPSpec];			// C2��[X,Y]
	JUDGE_SIZE		ElecC1Mezumari[nPSpec];		// C1�ڋl�܂�[X,Y]
	JUDGE_SIZE		SideElecXmin[nPSpec];		// ���ʓd�ɔ�荞�ݐ��@ X�ŏ��l[X,Y]
	JUDGE_DATA		MarkLevel[nPSpec];			// �W�󌟏o���x��
	JUDGE_DATA		MarkShift[nPSpec];			// �W���
	JUDGE_SIZE		IbutsuXsize[nPSpec];		// Glass Ibutsu  X ���� ����l[X,Y]
	JUDGE_SIZE		IbutsuYsize[nPSpec];		// Glass Ibutsu  Y ���� ����l[X,Y]
	// --- system
	JUDGE_DATA		Focus;						// �����Y�œ_
	JUDGE_DATA		Bright;						// �Ɠx�Ǘ�
}	INSPECT_PARAMETER;


R_TRANS_DATA			IndexTrans;											// R_TRANS_DATA�\����

// --- prototype ---
extern  int				Ma_pos[4];											//�W��ʒu���o�f�[�^( lex, rix, upy, loy )
extern  int             TWA_R_n;                    						//��R�l������ F/J


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
