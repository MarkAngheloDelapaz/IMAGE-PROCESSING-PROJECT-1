#ifndef R_UVC_H
#define R_UVC_H

#include "R_video.h"

#ifdef __cplusplus
extern "C" {
#endif

int	r_uvc_get_video_auto(struct cocz33_t*, int, int);
int	r_uvc_get_video_auto_end_check(struct cocz33_t*);

#ifdef __cplusplus
}
#endif

#endif // R_UVC_H
