#ifndef RCSINSPECT_H
#define RCSINSPECT_H

#include "R_cs_res.h"
#include "R_cdraw.h"

#include <RInspect/RInspect.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
	R_INSPECT_HANDLE h;
	void *Result;
} R_CS_INSPECT_HANDLE;


R_INSPECT_HANDLE RCsInspectGetRInsp(R_CS_INSPECT_HANDLE handle);

/**
 * @brief RCsInspectAppendResult
 *  R_CS_Result�p�\���̂̎擾�֐�
 *  Create�����s�\
 *  Terget�̃������m�ۂ̓��[�U���Ŏ��s���邱��
 * @param handle
 * @param pTerget
 * @return
 */
int RCsInspectAppendResult(R_CS_INSPECT_HANDLE handle, R_CS_RESULT_MONITOR_ITEM **pTerget);



#ifdef __cplusplus
}




using std::string;

class RInspect;

int RCsInspectOpen(R_CS_INSPECT_HANDLE *cshandle);

#ifdef QT_CORE_LIB

static R_CS_INSPECT_HANDLE *csHandle;

//	�e���v���[�g�֐��̂��߃w�b�_�[�ɋL��
//static int Res;
static void _RCsInspectCreateSub()
{
	Res = RCsInspectOpen(csHandle);
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
R_CS_INSPECT_HANDLE RCsInspectCreate(const char *inspname, T **insp)
{
	R_CS_INSPECT_HANDLE h = {{NULL,NULL,NULL,NULL},NULL};

	T **pInsp = (T **)&h.h.p;
	if(*pInsp == NULL)		*pInsp = new T(string(inspname));

	*insp = *pInsp;

#ifdef QT_CORE_LIB
	csHandle = &h;
	R_system_callback(_RCsInspectCreateSub);
#else
	//_RInspectCreateSub();
	RCsInspectOpen(&h);
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
T* RCsInspectGetInsp(R_CS_INSPECT_HANDLE handle, T **insp){
	*insp = (T*)(handle.h.p);
	return *insp;
}



#include "RCsResult/RCsResultItem.h"

class RCsInspect : public RInspect
{
public:
	explicit RCsInspect(string inspname);

	int AppendResultItem(R_CS_RESULT_MONITOR_ITEM **pTarget);

protected:
	RCsResultItem*	_csresults;				///<	���ʃ��X�g
	//RCsResultItem *	_parentcsresults;			///<	�e�N���X�̌��ʃ��X�g

public:
	//int OpenCs(RDataItem *parentsettings, RCsResultItem *parentresults, RInspectPlain *scene);
	virtual int OpenCs();
	virtual int OpenCs(RDataItem *parentsettings, RDataItem *parentresults, RInspectPlain *scene);
	virtual int OpenCs(RDataItem *parentsettings, RCsResultItem  *parentresults, RInspectPlain *scene);
	virtual void InitResult();

};


#endif

#endif // RCSINSPECT_H
