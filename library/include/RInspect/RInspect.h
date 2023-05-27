//*******************************************************************************************************************************
/**
 * @file RInspect.h
 * @brief
	GUIモジュール基幹クラス


**	Initial内 @n
		RInspectCreate @n
			- RInspect実態生成（RInspectコンストラクタ）@n
			- セッティング、リザルト生成（RinspectOpen)@n

		RInspectAppendDatafile	←パラメータ構造体作成		*リストは数字が保存がマスト！もろもろ設定する前に設定を読み込まないといけない！@n
		RInspectAppendResult	←結果構造体作成@n

		R_CS_SetParameterFileSystem(SystemFileData, SystemFileName) @n
		R_CS_SetParameterFilePackage(PackageFileData, SystemPara.sPackageFileName) @n

		RInspectInit@n
			- ユーザ定義関数@n
			- リスト系のパラメータ作成・登録してる@n

	↓===パラメータ読み込み完了後===@n


	InitialUserOnR_CS内@n
		===カメラ初期化===@n

		SetGrayPlane
			- 画像プレーン登録メニューで画像プレーン選択する場合データ読み込み前に登録しないと…@n
		SetBinPlane@n
		SetDrawPane
			- VideoInit後である必要あり@n

		SetScene @n


	InitialUserOnR_CSLast内 @n
		OpenDialog(new) @n
			設定ダイアログ(_dlg)初期化		←GUIスレッドで生成する必要あり(Invoke) @n

 * @author Hiroki Komatsu
 * @date 2018/10
 */
//*******************************************************************************************************************************

#ifndef RINSPECT_H
#define RINSPECT_H

#include "R_dfile.h"
//#include "R_cs_res.h"
#include "R_video.h"

#include "RDataList/RDataItem.h"



#ifdef __cplusplus
extern "C"
{
#endif

#define RINSPECT_MODE_NORMAL	0
#define RINSPECT_MODE_SETTING	0x01
#define RINSPECT_MODE_STEP		0x10

typedef struct {
	void *p;
	void *Scene;
	void *Setting;
	void *Result;
} R_INSPECT_HANDLE;

/**
 * @brief RInspectSetRatio
 *  初期化関数	<br>
 *  Create後に実施する
 * @param handle
 * @param x
 * @param y
 */
void RInspectSetRatio(R_INSPECT_HANDLE handle, int x, int y);

void RInspectSetParentName(R_INSPECT_HANDLE handle, const char *parentname);

/**
 * @brief RInspectInit
 *  初期化関数	<br>
 *  Create後に実施する
 * @param handle
 * @return
 */
int RInspectInit(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectInitOnLast	<br>
 *  最終初期化後関数	<br>
 *  R_CS_InitialLastでの実行を推奨	<br>
 *  主にパラメータ読み込み後に実施する必要のある処理を記述する	<br>
 * @param handle	処理対象クラスのハンドラ
 * @return			0:	OK		<br>
 *					-1:	ERROR
 */
int RInspectInitOnLast(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectClose
 * - データの破棄
 * - 動的に検査を確保する場合は破棄時にリソースを開放する必要あり
 * - アプリケーション終了時には開放されるため基本的には実施不要
 *
 * @param handle
 */
void RInspectClose(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectSetGrayPlane
 * - 検索クラスに処理対象グレーページを登録する
 * - R_CS_InitialLast移行での実行を推奨
 * - *)登録のタイミングがはやすぎてと画像メモリ確保前に登録しないように注意
 *
 * @param handle
 * @param planename	登録ページ識別名
 * @param graypage	登録ページ番号
 */
void RInspectSetGrayPlane(R_INSPECT_HANDLE handle, const char *planename, const int graypage);

/**
 * @brief RInspectSetBinPlane
 * - 検索クラスに処理対象バイナリページを登録する
 * - R_CS_InitialLast移行での実行を推奨
 * - *)登録のタイミングがはやすぎてと画像メモリ確保前に登録しないように注意
 *
 * @param handle
 * @param planename	登録ページ識別名
 * @param binpage	登録ページ番号
 */
void RInspectSetBinPlane(R_INSPECT_HANDLE handle, const char *planename, const int binpage);

/**
 * @brief RInspectSetDrawPlane
 * - 検索クラスに処理対象DrawPlaneページを登録する
 * - これを実行しないとDrawPlaneはGUI上で表示されない
 * - RInspectSetGrayPlane以降での実行を推奨
 * - *)RInspectSetGrayPlane以前だと画像のサイズがわからない
 * @param handle
 */
void RInspectSetDrawPlane(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectSetAuthority
 *  設定関数の権限フラグを設定する関数
 * @param handle
 * @param flags
 * - AUTH_FLAGS_NOEDIT		=	0,
 * - AUTH_FLAGS_EVERYONE	=	1 << 0,
 * - AUTH_FLAGS_SETTING		=	1 << 1,
 * - AUTH_FLAGS_ALL			=	0xFFFFFFFF
 *
 */
void RInspectSetAuthority(R_INSPECT_HANDLE handle, AUTH_FLAGS flags);

/**
 * @brief RInspectGetDatafileCount
 *	RDatafile構造体の項目数取得関数
 *  Create後実行可能
 * @param handle
 * @return
 */
int RInspectGetDatafileCount(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectAppendDatafile
 *  RDatafile用の構造体取得関数
 *  Create後に実行可能
 *  Tergetのメモリ確保はユーザー側で実施すること
 * @param handle
 * @param pTerget
 * @return
 */
int RInspectAppendDatafile(R_INSPECT_HANDLE handle, R_DATAFILE_ITEM **pTerget);

/**
 * @brief RInspectAppendResult
 *  R_CS_Result用構造体の取得関数
 *  Createご実行可能
 *  Tergetのメモリ確保はユーザ側で実行すること
 * @param handle
 * @param pTerget
 * @return
 */
//int RInspectAppendResult(R_INSPECT_HANDLE handle, R_CS_RESULT_MONITOR_ITEM **pTerget);

/**
 * @brief RInspectGetResultCount	<br>
 *	設定アイテム数取得関数
 *  Create後実行可能
 * @param handle
 * @return
 */
int RInspectGetResultCount(R_INSPECT_HANDLE handle);


/**
 * @brief RInspectSetting	<br>
 *	設定実行用関数
 * @param handle
 * @return
 */
int RInspectSetting(R_INSPECT_HANDLE handle);
/**
 * @brief RInspectExecute	<br>
 *  検査実行用関数
 * @param handle	検査を実行したいRInspetクラスのハンドラ
 * @return
 *	検査結果
 */
int RInspectExecute(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectExecuteMode
 *  実行モード付き検査実行用関数
 * @param handle	検査を実行したいRInspetクラスのハンドラ
 * @param mode		検査関数に引き渡したいパラメータ(通常実行モードなど)
 * @return
 *	検査結果
 */
int RInspectExecuteMode(R_INSPECT_HANDLE handle, int mode);

/**
 * @brief RInspectShowResult
 * @param handle
 * @return
 */
int RInspectShowResult(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectSetMsgArea
 * @param handle
 * @param area
 * @return
 */
void RInspectSetMsgArea(R_INSPECT_HANDLE handle, R_RECT area);

/**
 * @brief RInspectClearResult
 * @param handle
 */
void RInspectInitResult(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectSaveSettings
 * @param handle
 * @param filename
 */
void RInspectSaveSettings(R_INSPECT_HANDLE handle, const char* filename);

/**
 * @brief RInspectLoadSettings
 * @param handle
 * @param filename
 */
void RInspectLoadSettings(R_INSPECT_HANDLE handle, const char *filename);

/**
 * @brief RInspectParameterIsChanged
 * @param handle
 * @return
 */
int RInspectParameterIsChanged(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectParameterRestoreChange
 * @param handle
 */
void RInspectParameterReturnChange(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectParameterFixChange
 * @param handle
 */
void RInspectParameterFixChange(R_INSPECT_HANDLE handle);


#ifdef __cplusplus
}

#include <string.h>

//#include "RCsResult/RCsResultItem.h"
#include "RInspect/RInspectPlane.h"
#include "RInspect/RInspectSettingItem.h"

#ifdef QT_CORE_LIB
	#include <mutex>
	#include "R_system.h"
	#include "RInspect/RDialogSetting.h"
#endif

using std::string;

class RInspect;


//	テンプレート関数のためヘッダーに記載
int RInspectOpen(R_INSPECT_HANDLE *h);


#ifdef QT_CORE_LIB

static int Res;
static R_INSPECT_HANDLE *Handle;

static void _RInspectCreateSub()
{
	Res = RInspectOpen(Handle);
}
#endif


/**
 * @brief RInspectCreate
 * - 検査クラスのインスタンス（実体）生成関数
 * - テンプレート関数のためcでは使用不可。cで使用する場合c++のラッパーを作成すること
 *
 * @param inspname
 * @return
 * @code
 *
 *	//使用例
 *	R_INSPECT_HANDLE CheckAnodeCreate(char *name, MEASDATA *measure, int *intbuf, R_POINT *pointbuffer)
 *	{
 *		R_INSPECT_HANDLE handle = RInspectCreate<chk_anode>(name);
 *		chk_anode *insp = RInspectGetInsp<chk_anode>(handle);
 *
 *		insp->SetMeasure(measure);
 *		insp->SetBuffer(intbuf,pointbuffer);
 *		return handle;
 *	}
 *
 * @endcode
 */
template <typename T>
R_INSPECT_HANDLE RInspectCreate(const char *inspname, T **insp)
{
	R_INSPECT_HANDLE h = {NULL,NULL,NULL,NULL};

	T **pInsp = (T **)&h.p;
	if(*pInsp == NULL)		*pInsp = new T(string(inspname));

	*insp = *pInsp;


#ifdef QT_CORE_LIB
	Handle = &h;
	R_system_callback(_RInspectCreateSub);
#else
	//_RInspectCreateSub();
	RInspectOpen(&h);
#endif

	return h;
}

/**
 * @brief RInspectGetInsp
 * @param handle
 * @param insp
 * @return
 */
template <typename T>
T* RInspectGetInsp(R_INSPECT_HANDLE handle, T **insp){
	*insp = (T*)(handle.p);
	return *insp;
}

/**
 * @brief The RInspect class
 */


#ifdef QT_CORE_LIB
class RInspect : public QObject
{
	Q_OBJECT
#else
class RInspect
{

#endif


public:
	static const string Ver;

	RInspect(string inspname);
	virtual ~RInspect();

	/**
	 * @brief Init
	 * @return
	 */
	virtual int Init();

	/**
	 * @brief Open
	 * @param parentsettings
	 * @param parentresults
	 * @param scene
	 * @return
	 */
	virtual int Open(RDataItem *parentsettings, RDataItem *parentresults , RInspectPlain *scene);

	/**
	 * @brief Open
	 * @return
	 */
	virtual int Open();

	/**
	 * @brief Close
	 * @return
	 */
	virtual int Close();

	/**
	 * @brief Setting
	 * @return
	 */
	virtual int Setting();

	/**
	 * @brief Setting_NonGui
	 * @return
	 */
	virtual int Setting_NonGui();

	/**
	 * @brief Execute
	 * @param mode
	 * @return
	 */
	virtual int Execute(int mode = 0);

	/**
	 * @brief ShowResult
	 * @param mode
	 * @return
	 */
	virtual int ShowResult(int mode = 0);

	/**
	 * @brief ShowResult_NonGui
	 * @param mode
	 * @return
	 */
	virtual int ShowResult_NonGui(int mode = 0);

	/**
	 * @brief ClearResult
	 * @param mode
	 * @return
	 */
	virtual int ClearResult(int mode = 0);

	/**
	 * @brief InitOnLast
	 * @return
	 */
	virtual int InitOnLast();

	/**
	 * @brief Setting_Ready
	 */
	virtual void Setting_Ready(){}

	/**
	 * @brief Setting_User
	 */
	virtual void Setting_User(){}

	/**
	 * @brief Setting_After
	 */
	virtual void Setting_After(int result){result = result;}

	/**
	 * @brief InitResult
	 */
	virtual void InitResult(){}

#ifdef QT_CORE_LIB
	/**
	 * @brief Setting_Gui
	 * @return
	 */
	virtual int Setting_Gui();

	/**
	 * @brief ShowResult_Gui
	 * @param mode
	 * @return
	 */
	virtual int ShowResult_Gui(int mode = 0);
#endif



	void SetAuthority(AUTH_FLAGS flags);
	void SetScene(RInspectPlain *scene);
	void SetRatio(int x, int y);
	void SetMsgArea(R_RECT area);

	void SetGrayPage(const char *name, const int page);
	void SetBinPage(const char *name, const int page);

	int GetDataFileCount();
	int GetResultCount();
	int AppendSettingItem(R_DATAFILE_ITEM **pTarget);
	char *GetInspName();
//	int AppendResultItem(R_CS_RESULT_MONITOR_ITEM **pTarget);

	void DispMessage(string str);
	void ClearMessage();


	void SaveSetting(string filename);
	void LoadSetting(string filename);

	bool ParameterIsChanged();
	void ReturnChange();
	void FixChange();

//	QT表示用
#ifdef QT_CORE_LIB
	int SetTeachItem(R_RECT *rect, int *base_x, int *base_y, QColor color = QColor("blue"), QString comment = "");
	int SetTeachItem(R_RECT *rect, R_POINT *base = NULL, QColor color = QColor("blue"), QString comment = "");
#endif

protected:
	string _inspname;						///<	検査モジュール名
	char *_cinspname;

	RInspectPlain *_scene;					///<	画像管理クラス
	R_RECT _msgarea;						///<	メッセージ表示エリア
	int _msgcaret;							///<	メッセージ表示行(現在)
	//QGraphicsItem items;

	AUTH_FLAGS		_settingflags;			///<	パラメータ設定権限
	RDataItem *		_settings;				///<	設定パラメータリスト
	RDataItem *		_parentsettings;		///<	親クラスのパラメータ
	RDataItem *		_results;				///<	結果リスト
	RDataItem *		_parentresults;			///<	親クラスの結果リスト

	int _ratio[2];							///<	画像解像度

	vector<RInspectSettingItem *> _menu;	///<	個別設定ダイアログ（重要パラメータ）のリスト

	/**
	 * @brief SetSettingItem
	 * - 個別設定ダイアログ（重要パラメータ）の設定関数
	 * @param item
	 */
	void SetSettingItem(RDataItem *item);

	typedef struct {
		string *name;
		int page;
	} PAGE_INDEX;

	vector<PAGE_INDEX> _grayindex;
	vector<PAGE_INDEX> _binindex;

private:
	void _GetLayout(struct button_t *pnt_button, int *posi_para_x, int *posi_para_y, int *posi_res_x, int *posi_res_y, int *cls_x, int *cls_y);

//	QT表示用
#ifdef QT_CORE_LIB
public:
	static std::mutex		_mtmp;
	static RInspect *		_tmpinsp;
	static RDataItem *		_tmpsettings;
	static RDataItem *		_tmpresults;
	static RInspectPlain *	_tmpscene;					///<	画像管理クラス(コールバック用ポインタ)
	static AUTH_FLAGS		_tmpsettingflags;			///<	パラメータ設定権限
	static RDialogSetting *	_tmpdlg;
	static PAGE_INDEX *		_tmpindex;

protected:
	RDialogSetting *_dlg;								///<	設定ダイアログ（QTのみ）
	vector<RInspectViewTeachItem *> _teach;				///<	GUI設定アイテム

	void _initDialog();

private slots:
	void _updatestgitem();
	
#endif


	//static void _CreateSettingDialog();
};


#endif

#endif // RINSPECT_H
