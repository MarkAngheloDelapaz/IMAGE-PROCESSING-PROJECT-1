#ifndef R_VIDEO_BASLER_USB_H
#define R_VIDEO_BASLER_USB_H

#ifdef __cplusplus
extern "C" {
#endif

int	R_video_init_basler_usb(int camera_type, int frame_size, int camera_mode,  int func_bits, const char* serial_number);

#ifdef __cplusplus
}
#endif

#endif // R_VIDEO_BASLER_USB_H
