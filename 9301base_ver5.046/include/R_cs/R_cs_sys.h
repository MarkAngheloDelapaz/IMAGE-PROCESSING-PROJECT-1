#ifndef R_CS_SYS_H
#define R_CS_SYS_H

#ifdef QT32
#ifdef __cplusplus
//#include "Qt32/RMainWindow.h"

#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

void r_cs_system_analyze_show();
void r_cs_system_analyze_set_source(int handle);
void r_cs_system_analyze_countup(int page);
void r_cs_system_analyze_saveresult(const char* workdir);
void r_cs_system_analyze_counterclear();

#ifdef __cplusplus
}
#endif


#endif // R_CS_SYS_H
