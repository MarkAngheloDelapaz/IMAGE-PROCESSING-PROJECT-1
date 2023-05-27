extern int				Pilot_mode;							//パイロットモード
extern int				InspClass;							// 検査 許容差

extern char             MarkChar[];
extern int				InitializeFlag;						// 初期化完了フラグ
extern int              GLMode;
extern int              GLCount;
// Spec Set
extern char             SpecData[];							// バーコードデータ
extern char             SpecDataBak[];						// バーコードデータ
extern char             GLData[];							// GL data
extern char             SpecType[3];             			// 製品タイプ
extern char             DataClass[4][5];
//static char           PilotDisp[];

int SpecSet( int nMenuNumber );


