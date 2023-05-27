#ifndef RDATAITEM_H
#define RDATAITEM_H

#include <stdlib.h>

#include "R_stdlib.h"
#include "R_dfile.h"
#include "R_winb.h"
#ifdef DEBUG_RINSP
	#include <dlfcn.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif

/**
  権限モード
  */
typedef enum{
	AUTH_FLAGS_NOEDIT	=	0,				///<	編集なし
	AUTH_FLAGS_EVERYONE	=	1 << 0,			///<	全員編集可（デフォルト値）
	AUTH_FLAGS_SETTING	=	1 << 1,			///<	セッティングモード
	AUTH_FLAGS_TS		=	1 << 2,			///<	製品標準値（寸法など）
	AUTH_FLAGS_ALL		=	0xFFFFFFFF		///<	全権限モード（NOEDIT指定は変更不可）
}AUTH_FLAGS;

#define UNUSED(X)	{X=X;}
#ifdef DEBUG_RINSP
	#define DEBUG_OUT			{printf("[%s::%s,%d]\n",__FILE__,__FUNCTION__,__LINE__);	fflush(stdout);	fflush(stderr);}
	#define DEBUG_OUT_DETAIL	{Dl_info info;dladdr(__builtin_return_address(0), &info);printf("[%s]=>[%s::%s,%d]\n",info.dli_sname,__FILE__,__FUNCTION__,__LINE__);	fflush(stdout);	fflush(stderr);}
	#define DEBUG_OUT_INT(X)	{printf("[%s::%s,%d] %d\n",__FILE__,__FUNCTION__,__LINE__,X);	fflush(stdout);	fflush(stderr);}
#else
	#define DEBUG_OUT			{}
	#define DEBUG_OUT_DETAIL	{}
	#define DEBUG_OUT_INT(X)	{X=X}
#endif


#ifdef __cplusplus
}


#include <string>
#include <vector>
#include <typeinfo>
#include <limits>


#ifdef QT_CORE_LIB
	#include <QSettings>
	#include <QWidget>
#endif


using std::string;
using std::vector;
#ifdef __GNUC__
	using std::type_info;
#endif

#ifdef QT_CORE_LIB
class RDataItem : public QObject
{
	Q_OBJECT
#else
class RDataItem
{
#endif
//public:
//	static const int FLAG_NOEDIT;
//	static const int FLAG_ALL;
//	static const int FLAG_SETTING;
//	static const int FLAG_EVERYONE;

public:
	//　テンプレート関数のためヘッダーで定義するメンバ

	~RDataItem();

	/*!
	 * \brief RDataItem::RDataItem
	 * \param key		パラメータ名
	 * \param value		パラメータポインタ
	 * \param def		デフォルト値
	 * \param flags		権限レベル
	 */
	explicit RDataItem(string key, AUTH_FLAGS flags = AUTH_FLAGS_EVERYONE);

	/**
	  対応しているデータ型
	  */
	typedef enum{
		TYPE_INT,
		TYPE_DOUBLE,
		TYPE_STRING,
		TYPE_RECT,
		TYPE_ARRAY,
		TYPE_ARRAY_STRING,
		TYPE_ARRAY_CHAR,
		TYPE_POINT,
		TYPE_UNKNOWN,
		NUM_ALLOWED_TYPE
	}ALLOWED_TYPE;

	/**
	  データ数カウント時のカウント方法
	  */
	typedef enum{
		COUNT_R_DATAFILE_ITEM,
		COUNT_WINB_PARA,
		COUNT_ALL
	}ITEM_COUNT_MODE;

	/**
	 * @brief MakeChild
	 * @param key		設定項目の表示文字列(重複不可)
	 * @param value		設定項目のデータアドレス
	 * @param def		初期値
	 * @param flags		書き込み権限
	 *					0:	書き込み不可
	 *					1~:	書き込み権限フラグ
	 */
	template<typename T>
	void MakeChild(string key, T *value, T def, AUTH_FLAGS flags)
	{
		this->AppendChild(new RDataItem(key,value,def,flags));
	}

	template<typename T>
	void MakeChild(string key, T *value, const int length, T def, AUTH_FLAGS flags)
	{
		this->AppendChild(new RDataItem(key,value,length,def,flags));
	}

	template<typename T>
	void MakeChild(string key, T *value, T def, R_RECT limit, AUTH_FLAGS flags)
	{
		this->AppendChild(new RDataItem(key,value,def,limit,flags));
	}


public:
	//	通常のクラス内メンバ
	void MakeChild(string key, int *value, int def, int base, int lc, int uc, int ll, int ul, AUTH_FLAGS flags);
	void MakeChild(string key, int *value, int def, vector<string> list, AUTH_FLAGS flags);
	void MakeChild(string key, char *value, const char *def, int length, AUTH_FLAGS flags);
	void AppendChild(RDataItem *child);
	void AppendChild(vector<RDataItem *> children);

	void RemoveChild(RDataItem *child);

	RDataItem *GetChild(int row);
	RDataItem *GetChild(string key);
	int GetCount(ITEM_COUNT_MODE mode = COUNT_R_DATAFILE_ITEM, AUTH_FLAGS flags = AUTH_FLAGS_EVERYONE);
	int GetChildCount() const;
	int GetColumnCount() const;
	int GetRow() const;
	RDataItem *GetParent();
	string GetKey() const;
	char *GetKeyByChar();
	string GetFullKey();
	char *GetFullKeyByChar();
	string GetIndentKey();
	char *GetIndentKeyByChar();
	vector<string>  GetEnum();
	struct winb_para_menu_t *GetEnumMenu();
	int *GetLimit(int i);
	int *GetControl(int i);
	int *GetBase();
	void *GetTarget();
	void *GetDefault();
	ALLOWED_TYPE GetType();
	string GetTypeName();
	int Find(const RDataItem *target);

	void SetParent(RDataItem *parent);
	void SetKey(string key);
	bool SetEnum(string name, vector<string> list);
	int CheckFlag(AUTH_FLAGS flags);
	bool IsChanged();

	void ReturnChange();
	void FixChange();

	static void FromDatafileItem(R_DATAFILE_ITEM *src, RDataItem *dst, string name = "default");
	int AppendDatafileItem(R_DATAFILE_ITEM **pTarget);

	int AppendWinbPara(struct winb_para_tm **pTarget, AUTH_FLAGS flags = AUTH_FLAGS_EVERYONE);

//Qt表示用
#ifdef QT_CORE_LIB
	bool SetData(const QVariant &value);
	QVariant GetData(int column) const;
	void SaveSettings(QSettings *settings);
	void LoadSettings(QSettings *settings);

signals:
	void datachanged();
#endif

private:
	const static string KEY_VALUE;
	static int _dummy;

protected:
	//	テンプレート関数のためヘッダーで定義するメンバ
	template<typename T>
	RDataItem(string key, T *value, T def, AUTH_FLAGS flags)
	{
		_InitBase(key,flags);
		_InitVal(value,def);
	}

	template<typename T> RDataItem(string key, T *value, int length, T def, AUTH_FLAGS flags)
	{
		_InitBase(key, AUTH_FLAGS_NOEDIT);
		_SetArray(value,length,def,flags);
	}

	template<typename T>
	void _InitVal(T *value, T def){
		_value = (void*) value;

		_oldvalue = (void *)malloc(sizeof(T));
		_def = (void *)malloc(sizeof(T));

		*(T*)_oldvalue = *value;
		*(T*)_def = def;

		_type = _GetType(value);
	}


	template<typename T>
	void _SetArray(T *value, const int length, T def, AUTH_FLAGS flags)
	{
	   for(int i = 0; i < length; i++){
		   this->MakeChild(std::to_string(i) ,value + i, def, flags);
	   }
	}

	template<typename T>
	static ALLOWED_TYPE _GetType(const T *){
		//type_info const & type = typeid(T);
		if (typeid(T) == typeid(int)) {
			return TYPE_INT;
		}else if(typeid(T) == typeid(double)){
			return TYPE_DOUBLE;
		}else if(typeid(T) == typeid(R_RECT)){
			return TYPE_RECT;
		}else if(typeid(T) == typeid(R_POINT)){
			return TYPE_POINT;
		}else if(typeid(T) == typeid(string)){
			return TYPE_STRING;
		}else if(typeid(T) == typeid(char*)){
			return TYPE_ARRAY_CHAR;
		}
		return TYPE_UNKNOWN;
	}

protected:
	//	通常のクラス内参照メンバ
	RDataItem(string key, int *value, int def, int base, int lc, int uc, int ll, int ul, AUTH_FLAGS flags);
	RDataItem(string key, int *value, int def, vector<string> list, AUTH_FLAGS flags);
	RDataItem(string key, char *value, const char *def, int length, AUTH_FLAGS flags);
	RDataItem(string key, R_RECT *value, R_RECT def, AUTH_FLAGS flags);
	RDataItem(string key, R_POINT *value, R_POINT def, AUTH_FLAGS flags);
	RDataItem(string key, R_RECT *value, R_RECT def, R_RECT limit, AUTH_FLAGS flags);
	RDataItem(string key, R_POINT *value, R_POINT def, R_RECT limit, AUTH_FLAGS flags);

	void _InitBase(string key, AUTH_FLAGS flags);
	void _InitLimit(int base, int lc, int uc, int ll, int ul);

	void _Append(vector<RDataItem *> children);
	void _SetRRect(R_RECT *value, R_RECT def, AUTH_FLAGS flags,
					int left = std::numeric_limits<int>::min(),	int top = std::numeric_limits<int>::min(),
					int right = std::numeric_limits<int>::max(), int bottom = std::numeric_limits<int>::max());
	void _SetRPoint(R_POINT *value, R_POINT def, AUTH_FLAGS flags,
					int left = std::numeric_limits<int>::min(),	int top = std::numeric_limits<int>::min(),
					int right = std::numeric_limits<int>::max(), int bottom = std::numeric_limits<int>::max());

	int _AppendDatafileItem(R_DATAFILE_ITEM **pTarget, int start);
	int _AppendWinbPara(struct winb_para_tm **pTarget, AUTH_FLAGS flags, int start);


//Qt表示用
#ifdef QT_CORE_LIB
	QVariant _GetQVarient(void *src) const;
	bool _setQVarient(void *dst, const QVariant &value);
#endif

protected:
	//	通常のクラス内参照メンバ
	vector<RDataItem*> _children;
	RDataItem *_parent;

	string _key;
	string _fullkey;
	void *_value;
	ALLOWED_TYPE _type;
	AUTH_FLAGS _flags;
	char *_keyc;
	char *_fullkeyc;
	char *_indentkey;
//	string _fullkey;
//	string _indentkey;

	void *_oldvalue;
	void *_def;

	int _base;
	int _step;				///<	調整時の幅
	int _control[2];
	int _limit[2];
	vector<string> _enum;
	struct	winb_para_menu_t *_enummenu;


protected:
	//	クラス内静的メンバ
	static struct winb_para_menu_t _enum_invalid[1];

	int _length;
};

#endif

#endif // RDATAITEM_H
