#ifndef R_CGEOM_H_
#define R_CGEOM_H_

int R_convex_pickup( int num, int *x, int *y, int *resnum, int *res_x, int *res_y );
int r_convex_check_clockwise( int ax, int ay, int bx, int by, int cx, int cy );

#endif	// R_CGEOM_H_
