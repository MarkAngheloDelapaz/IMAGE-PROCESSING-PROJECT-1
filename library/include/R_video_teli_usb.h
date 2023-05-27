#ifndef R_VIDEO_TELI_USB_H
#define R_VIDEO_TELI_USB_H

#ifdef __cplusplus
extern "C" {
#endif

int	R_video_init_teli_usb(int camera_type, int frame_size, int camera_mode, int mode, const char* serial_number);

#ifdef __cplusplus
}
#endif

#endif // R_VIDEO_TELI_USB_H
