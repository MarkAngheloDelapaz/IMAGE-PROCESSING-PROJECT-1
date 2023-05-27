/******************************************************************************
	R_EPCS.H / R_EPCS.C
	
	•ÏX—š—ğ
	Ver 4.01	2011/07/25	V‹K’Ç‰Á
******************************************************************************/

#ifndef	R_EPCS_H_
#define	R_EPCS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define	R_EPCS4_ID		0x12
#define	R_EPCS16_ID		0x14
#define	R_EPCS128_ID	0x18

int		r_epcs_open(void* ptr, int delay);
void	r_epcs_close(void);

void	r_epcs_write_enable(void);
void	r_epcs_write_disable(void);
int		r_epcs_read_status(void);
void	r_epcs_read_bytes(int adr, uint8_t* data, int num);
int		r_epcs_read_silicon_id(void);
void	r_epcs_write_bytes(int adr, const uint8_t* data, int num);
void	r_epcs_erase_bulk(void);
int		r_epcs_read_device_identification(void);

#ifdef __cplusplus
}
#endif

#endif	// R_EPCS_H_
