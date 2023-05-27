/************************************************

�@�G�b�W���o�֐�
		1994. 3.25

	history
	94. 6. 2				��������ŁA���Z�G���[���������Ă����̂��C��
	94. 7. 1				���߂̋���
	94. 8. 5				��������Q�l�G�b�W�T�[�`�Ƀo�O�����B�C��
	99.09.18				640x480�Ή�  by matsu
	00.07.11				Es_work�̈�m�ۂ�R_main�ɕύX(�σT�C�Y��)
							Se_dx==0&&Se_dy==0���̃G���[�����ǉ�	by ohshima
	01.01.26				search_edge���o�O�C��	by ohshima
	01.08.20				get_bin_edge()�̒��Arc/1000+1 -> rc+1 �ɕύX�Bby shintani
	08.04.24	Ver.3.50	Es_work�̈�𒴂��ăT�[�`���s���Ƃ��ɃG���[��Ԃ��悤�ɏC��
							Es_work�̈�m�ۊ֐���R_edge.c�Ɉړ��A���I�ȃT�C�Y�ύX���\�ɂ���
							R_es_set_length()�ɖ߂�l�ǉ�
	09.05.26	Ver.3.50	R_es_close�C��

**************************************************/

#ifndef	R_EDGE_H_
#define	R_EDGE_H_

#define	ES_BLACK		0x01
#define	ES_WHITE		0x02

#ifdef	R_EDGE
	static int	Se_sx;
	static int	Se_sy;
	static int	Se_ns = 2;
	static int	Se_pg = 0;
	static int	Se_dx;
	static int	Se_dy;
	static int	Se_av = 1;
	static int	Se_ct;
	static int	Se_st;
	static int	Se_sl = 512;
	int			*Es_work=NULL;
	int			Se_sl_max = 0;					// 2008.04.24
	static int	Slice_level = 500;
	static int	Se_level = 50;
	static int	Se_debug = OFF;
#else
	extern int* Es_work;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/************************************************
�@�֐��v���g�^�C�v�錾
************************************************/
//	�O���[�o��
void R_es_set_start( int, int );
int R_es_set_degree( int );
int R_es_set_vector( int, int );
int R_es_set_sincos( double, double );
void R_es_set_noise( int );
void R_es_set_page( int );
int R_es_set_length( int );				// �߂�l�ǉ�  2008.04.24
void R_es_set_debug( int );
void R_es_set_level( int );
void R_es_set_average( int );
void R_es_set_slice( int );
int R_es_search( int *, int *, int * );
int R_es_bin_search( int *, int *, int * );

int R_es_open( int nMaxLength );		// �ǉ�  2008.04.24
int R_es_close( void );					// �ǉ�  2008.04.24
int R_es_get_max_length( void );		// �ǉ�  2008.04.24

//	���[�J��
#ifdef	R_EDGE
	int get_work_area(void);
	void cal_sincos( void );
	int get_edge_y( int *, int *, int * );
	int get_edge_x( int *, int *, int * );
	int get_edge_45( int *, int *, int * );
	int get_edge_135( int *, int *, int * );
	int cal_edge( int, int * );
	void disp_debug( int );
#endif

#ifdef __cplusplus
}
#endif
#endif	// R_EDGE_H_
