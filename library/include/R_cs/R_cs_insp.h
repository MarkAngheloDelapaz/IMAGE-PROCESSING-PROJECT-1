#ifndef R_CS_INSP_H
#define R_CS_INSP_H

#include "RCsInspect/RCsInspect.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "R_winb.h"
#include "R_dfile.h"

void	R_CS_InspectInitial();
void	R_CS_InspectInitialOnLast();

int		R_CS_inspectExecute(int camera, int mode, int step);

void	R_CS_InspectGetMenu(struct winb_item_t *dist);
int		R_CS_InspectGetMenuCount();

R_DATAFILE_ITEM* R_CS_InspectCreatePkgItem(R_DATAFILE_ITEM* basic_item);

void	R_CS_InspectSetHandle(int cam_no, R_CS_INSPECT_HANDLE handle);
int		R_CS_InspectSetInspectionName(int num, char* j_name, char* e_name);

#ifdef __cplusplus
}
#endif
#endif // R_CS_INSP_H
