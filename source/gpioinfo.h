#ifndef GPIOINFO_H
#define GPIOINFO_H

struct hls_info {
	unsigned long	ptr;
	union {
		uint64_t	data64[16];
		uint32_t	data32[32];
		uint16_t	data16[64];
		uint8_t		data8[128];
	};
};

struct gpio_dac {
	double	offset;
	double	span;
};

struct gpio_info {
	int	fd;
	int (*initialize)(struct gpio_info*);
	int (*inp)(struct gpio_info*, int channel);
	int (*outp)(struct gpio_info*, int channel, int data);
	int (*inpw)(struct gpio_info*, int channel);
	int (*outpw)(struct gpio_info*, int channel, int data);
	int (*inpd)(struct gpio_info*, int channel);
	int (*outpd)(struct gpio_info*, int channel, int data);
	int	(*set_current)(struct gpio_info*, int channel, int data);
	int	(*set_dac)(struct gpio_info*, int channel, int data);
	int	(*get_vforce)(struct gpio_info*, int channel);
	int	(*get_vsupply)(struct gpio_info*, int channel);
	int	(*get_temperature)(struct gpio_info*, int channel);
	int	(*adjust_dac)(struct gpio_info*, int channel);
	
	const char*	board_name;
	
	int	rdsw;
	int	vendor_id;
	int	device_id;
	int	bus_no;
	int	device_no;
	int	access_mode;
	int	with_gpdac;
	int	num_of_inp;
	int	num_of_outp;
	uint8_t	bits_of_inp[8];
	uint8_t	bits_of_outp[8];
	int	num_of_dac;
	int	driver_version;
	int	fpga_version;
	int	board_version;
	struct gpio_dac	dac[8];
//	double nDacSpan[8];
//	double nDacOffset[8];
	struct hls_info*	hls;
	
	unsigned long	mmio_adr;
	
	struct gpio_info*	next;
};

struct gpio_info*	r_gpio_info_create(void);
struct gpio_info*	r_gpio_info_of_channel(int channel);
struct gpio_info*	r_gpio_info(int rdsw);

#endif // GPIOINFO_H
