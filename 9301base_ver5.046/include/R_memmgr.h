/******************************************************************************
	R_memmgr.h / R_memmgr.c
	
	ïœçXóöó
	Ver 4.01	2010/01/07	ê≥éÆÉäÉäÅ[ÉX
******************************************************************************/

#ifndef	R_MEMMGR_H_
#define	R_MEMMGR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__

struct r_memmgr {
	void*	ptr;
	void*	obj;
	struct r_memmgr*	prev;
	struct r_memmgr*	next;
};

int		r_memmgr_open(int);
void	r_memmgr_close(void);

struct r_memmgr*	r_memmgr_create(int);
int		r_memmgr_destroy(struct r_memmgr*);

struct r_memmgr*	r_memmgr_alloc(int size);
uintptr_t			r_memmgr_ptr(struct r_memmgr*);
unsigned long		r_memmgr_pages(struct r_memmgr*);
unsigned long		r_memmgr_bus_adr(struct r_memmgr*);
unsigned long		r_memmgr_bytes_of_block(struct r_memmgr*);
uint64_t*			r_memmgr_table(struct r_memmgr*);
int					r_memmgr_free(struct r_memmgr*);

int		r_memmgr_open(int);
void	r_memmgr_close(void);
int		r_memmgr_page_init(int page, void* adr, unsigned bus_adr, unsigned size);

void	r_memmgr_free_all(void);

void	r_memmgr_disable_scatter_gather(void);
int		r_memmgr_set_max_memory_block_size(int);
int		r_memmgr_get_max_memory_block_size(void);

void	r_memmgr_disable(void);

#endif	// __GNUC__

#ifdef __cplusplus
}
#endif

#endif	// R_MEMMGR_H_
