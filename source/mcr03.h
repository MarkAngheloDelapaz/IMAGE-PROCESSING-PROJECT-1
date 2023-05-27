int		TWA_kensa_m03( void );
void    TWA_mark_set_m03( void );

int		TWA_katamuki( void );
void	TWA_Mark_error_check( int pt[], int *nst, int *nre );
void	TWA_point_cls( void );

int		KAIKI_keisuu( int mode );
void	TWA_meansd( void );
int		TWA_G2_ES_search( int mode );
void	TWA_contrct2( int mem_no, int x_size, int y_size, int pt[] );
void	TWA_expand2( int mem_no, int x_size, int y_size, int pt[] );
void	TWA_Insp_white_or( int src_mem_no, int dst_mem_no, int xs, int ys, int xe, int ye, int l_hi );
int		TWA_smoothing( int src_mem_no, int dst_mem_no, int pt[] );
void	TWA_gray_fill( int x1, int y1, int x2, int y2, int level, int p );
void	TWA_surf_board( int src_mem_no1, int src_mem_no2, int dst_mem_no, int pt[], int level_min );


int		TWA_InspC2Xsize( void );
int		TWA_Mezu_Area( void );
void MaskMarkingMCR03(int binPn);

int		TWA_kensa_zure_w( void );
int		TWA_kensa_zure_l( void );
int		TWA_Insp_Ap( int mode );

int		G2StdEdgeSet( void );
int		G2_Judge_w_SDl( void );
int		G2_Judge_w_APl( void );
void	ElecDataSet(int side,int elec);

void	TWA_Mark_Size( int page, int pt[], int *ls, int *le );
void	TWA_Mark_div_check( int pt[], int *nst, int *nre, int *sum );
int		TWA_Index_search( void );
int		TWA_es_bin_search( int p, int *rx, int *ry, int deg, int l, int flag );


extern unsigned int	G2_data_u[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）上
extern unsigned int	G2_data_d[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）下
