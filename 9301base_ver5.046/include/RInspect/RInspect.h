//*******************************************************************************************************************************
/**
 * @file RInspect.h
 * @brief
	GUI���W���[����N���X


**	Initial�� @n
		RInspectCreate @n
			- RInspect���Ԑ����iRInspect�R���X�g���N�^�j@n
			- �Z�b�e�B���O�A���U���g�����iRinspectOpen)@n

		RInspectAppendDatafile	���p�����[�^�\���̍쐬		*���X�g�͐������ۑ����}�X�g�I�������ݒ肷��O�ɐݒ��ǂݍ��܂Ȃ��Ƃ����Ȃ��I@n
		RInspectAppendResult	�����ʍ\���̍쐬@n

		R_CS_SetParameterFileSystem(SystemFileData, SystemFileName) @n
		R_CS_SetParameterFilePackage(PackageFileData, SystemPara.sPackageFileName) @n

		RInspectInit@n
			- ���[�U��`�֐�@n
			- ���X�g�n�̃p�����[�^�쐬�E�o�^���Ă�@n

	��===�p�����[�^�ǂݍ��݊�����===@n


	InitialUserOnR_CS��@n
		===�J����������===@n

		SetGrayPlane
			- �摜�v���[���o�^���j���[�ŉ摜�v���[���I������ꍇ�f�[�^�ǂݍ��ݑO�ɓo�^���Ȃ��Ɓc@n
		SetBinPlane@n
		SetDrawPane
			- VideoInit��ł���K�v����@n

		SetScene @n


	InitialUserOnR_CSLast�� @n
		OpenDialog(new) @n
			�ݒ�_�C�A���O(_dlg)������		��GUI�X���b�h�Ő�������K�v����(Invoke) @n

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
 *  �������֐�	<br>
 *  Create��Ɏ��{����
 * @param handle
 * @param x
 * @param y
 */
void RInspectSetRatio(R_INSPECT_HANDLE handle, int x, int y);

void RInspectSetParentName(R_INSPECT_HANDLE handle, const char *parentname);

/**
 * @brief RInspectInit
 *  �������֐�	<br>
 *  Create��Ɏ��{����
 * @param handle
 * @return
 */
int RInspectInit(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectInitOnLast	<br>
 *  �ŏI��������֐�	<br>
 *  R_CS_InitialLast�ł̎��s�𐄏�	<br>
 *  ��Ƀp�����[�^�ǂݍ��݌�Ɏ��{����K�v�̂��鏈�����L�q����	<br>
 * @param handle	�����ΏۃN���X�̃n���h��
 * @return			0:	OK		<br>
 *					-1:	ERROR
 */
int RInspectInitOnLast(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectClose
 * - �f�[�^�̔j��
 * - ���I�Ɍ������m�ۂ���ꍇ�͔j�����Ƀ��\�[�X���J������K�v����
 * - �A�v���P�[�V�����I�����ɂ͊J������邽�ߊ�{�I�ɂ͎��{�s�v
 *
 * @param handle
 */
void RInspectClose(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectSetGrayPlane
 * - �����N���X�ɏ����ΏۃO���[�y�[�W��o�^����
 * - R_CS_InitialLast�ڍs�ł̎��s�𐄏�
 * - *)�o�^�̃^�C�~���O���͂₷���ĂƉ摜�������m�ۑO�ɓo�^���Ȃ��悤�ɒ���
 *
 * @param handle
 * @param planename	�o�^�y�[�W���ʖ�
 * @param graypage	�o�^�y�[�W�ԍ�
 */
void RInspectSetGrayPlane(R_INSPECT_HANDLE handle, const char *planename, const int graypage);

/**
 * @brief RInspectSetBinPlane
 * - �����N���X�ɏ����Ώۃo�C�i���y�[�W��o�^����
 * - R_CS_InitialLast�ڍs�ł̎��s�𐄏�
 * - *)�o�^�̃^�C�~���O���͂₷���ĂƉ摜�������m�ۑO�ɓo�^���Ȃ��悤�ɒ���
 *
 * @param handle
 * @param planename	�o�^�y�[�W���ʖ�
 * @param binpage	�o�^�y�[�W�ԍ�
 */
void RInspectSetBinPlane(R_INSPECT_HANDLE handle, const char *planename, const int binpage);

/**
 * @brief RInspectSetDrawPlane
 * - �����N���X�ɏ����Ώ�DrawPlane�y�[�W��o�^����
 * - ��������s���Ȃ���DrawPlane��GUI��ŕ\������Ȃ�
 * - RInspectSetGrayPlane�ȍ~�ł̎��s�𐄏�
 * - *)RInspectSetGrayPlane�ȑO���Ɖ摜�̃T�C�Y���킩��Ȃ�
 * @param handle
 */
void RInspectSetDrawPlane(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectSetAuthority
 *  �ݒ�֐��̌����t���O��ݒ肷��֐�
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
 *	RDatafile�\���̂̍��ڐ��擾�֐�
 *  Create����s�\
 * @param handle
 * @return
 */
int RInspectGetDatafileCount(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectAppendDatafile
 *  RDatafile�p�̍\���̎擾�֐�
 *  Create��Ɏ��s�\
 *  Terget�̃������m�ۂ̓��[�U�[���Ŏ��{���邱��
 * @param handle
 * @param pTerget
 * @return
 */
int RInspectAppendDatafile(R_INSPECT_HANDLE handle, R_DATAFILE_ITEM **pTerget);

/**
 * @brief RInspectAppendResult
 *  R_CS_Result�p�\���̂̎擾�֐�
 *  Create�����s�\
 *  Terget�̃������m�ۂ̓��[�U���Ŏ��s���邱��
 * @param handle
 * @param pTerget
 * @return
 */
//int RInspectAppendResult(R_INSPECT_HANDLE handle, R_CS_RESULT_MONITOR_ITEM **pTerget);

/**
 * @brief RInspectGetResultCount	<br>
 *	�ݒ�A�C�e�����擾�֐�
 *  Create����s�\
 * @param handle
 * @return
 */
int RInspectGetResultCount(R_INSPECT_HANDLE handle);


/**
 * @brief RInspectSetting	<br>
 *	�ݒ���s�p�֐�
 * @param handle
 * @return
 */
int RInspectSetting(R_INSPECT_HANDLE handle);
/**
 * @brief RInspectExecute	<br>
 *  �������s�p�֐�
 * @param handle	���������s������RInspet�N���X�̃n���h��
 * @return
 *	��������
 */
int RInspectExecute(R_INSPECT_HANDLE handle);

/**
 * @brief RInspectExecuteMode
 *  ���s���[�h�t���������s�p�֐�
 * @param handle	���������s������RInspet�N���X�̃n���h��
 * @param mode		�����֐��Ɉ����n�������p�����[�^(�ʏ���s���[�h�Ȃ�)
 * @return
 *	��������
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


//	�e���v���[�g�֐��̂��߃w�b�_�[�ɋL��
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
 * - �����N���X�̃C���X�^���X�i���́j�����֐�
 * - �e���v���[�g�֐��̂���c�ł͎g�p�s�Bc�Ŏg�p����ꍇc++�̃��b�p�[���쐬���邱��
 *
 * @param inspname
 * @return
 * @code
 *
 *	//�g�p��
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

//	QT�\���p
#ifdef QT_CORE_LIB
	int SetTeachItem(R_RECT *rect, int *base_x, int *base_y, QColor color = QColor("blue"), QString comment = "");
	int SetTeachItem(R_RECT *rect, R_POINT *base = NULL, QColor color = QColor("blue"), QString comment = "");
#endif

protected:
	string _inspname;						///<	�������W���[����
	char *_cinspname;

	RInspectPlain *_scene;					///<	�摜�Ǘ��N���X
	R_RECT _msgarea;						///<	���b�Z�[�W�\���G���A
	int _msgcaret;							///<	���b�Z�[�W�\���s(����)
	//QGraphicsItem items;

	AUTH_FLAGS		_settingflags;			///<	�p�����[�^�ݒ茠��
	RDataItem *		_settings;				///<	�ݒ�p�����[�^���X�g
	RDataItem *		_parentsettings;		///<	�e�N���X�̃p�����[�^
	RDataItem *		_results;				///<	���ʃ��X�g
	RDataItem *		_parentresults;			///<	�e�N���X�̌��ʃ��X�g

	int _ratio[2];							///<	�摜�𑜓x

	vector<RInspectSettingItem *> _menu;	///<	�ʐݒ�_�C�A���O�i�d�v�p�����[�^�j�̃��X�g

	/**
	 * @brief SetSettingItem
	 * - �ʐݒ�_�C�A���O�i�d�v�p�����[�^�j�̐ݒ�֐�
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

//	QT�\���p
#ifdef QT_CORE_LIB
public:
	static std::mutex		_mtmp;
	static RInspect *		_tmpinsp;
	static RDataItem *		_tmpsettings;
	static RDataItem *		_tmpresults;
	static RInspectPlain *	_tmpscene;					///<	�摜�Ǘ��N���X(�R�[���o�b�N�p�|�C���^)
	static AUTH_FLAGS		_tmpsettingflags;			///<	�p�����[�^�ݒ茠��
	static RDialogSetting *	_tmpdlg;
	static PAGE_INDEX *		_tmpindex;

protected:
	RDialogSetting *_dlg;								///<	�ݒ�_�C�A���O�iQT�̂݁j
	vector<RInspectViewTeachItem *> _teach;				///<	GUI�ݒ�A�C�e��

	void _initDialog();

private slots:
	void _updatestgitem();
	
#endif


	//static void _CreateSettingDialog();
};


#endif

#endif // RINSPECT_H
