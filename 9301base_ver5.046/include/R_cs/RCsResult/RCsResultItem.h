#ifndef RCSRESULTITEM_H
#define RCSRESULTITEM_H

#include "R_cs_res.h"
#include "RDataList/RDataItem.h"


class RCsResultItem : public RDataItem
{
public:
	RCsResultItem(string key);
	RCsResultItem(string key, int *value, int def, int base, int lc, int uc, int ll, int ul, AUTH_FLAGS flags);

	int GetCount();
	void Clear();

	int AppendResultMonitorItem(R_CS_RESULT_MONITOR_ITEM **pTarget);
	int AppendResultMonitorItem(R_CS_RESULT_MONITOR_ITEM **pTarget, int start);

protected:
	int _AppendResultMonitorItem(R_CS_RESULT_MONITOR_ITEM **pTarget, RDataItem *src, int start);
};

#endif // RCSRESULTITEM_H
