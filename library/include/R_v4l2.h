#ifndef R_V4L2_H
#define R_V4L2_H

#ifdef __cplusplus
extern "C" {
#endif

int	r_v4l2_device_idVendor(const char* device_name);
int	r_v4l2_device_idProduct(const char* device_name);

int	r_v4l2_open(int num);

#ifdef __cplusplus
}
#endif

#endif // R_V4L2_H
