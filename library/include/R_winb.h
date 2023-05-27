/******************************************************************************
	R_WINB.H / R_WINB.C
	
	変更履歴
		Ver 3.39	2003/06/06	正式リリース
		Ver 3.41	2004/01/10	R_winb_parameter
								R_winb_parameter_with_mess追加
		Ver 3.42	2004/02/26	R_swinb系対応
					2004/04/22	R_draw_winb系対応
		Ver 3.50	2009/02/12	DOS & LINUX ソース統合
		Ver.4.01	2012/07/25	R_winb_callback関数追加
******************************************************************************/

#ifndef	R_WINB_H_
#define	R_WINB_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WINB_NON_CLOSE	0
#define WINB_CLOSE	1

#define WINB_WAIT	0
#define WINB_NOWAIT	0x10

struct winb_title_t {
	char	*j;							/*　タイトル(日本語)*/
	char	*e;							/*　タイトル(英語)	*/
};
struct winb_item_t {
	char	*j;							/*　項目(日本語)				*/
	char	*e;							/*　項目(英語)					*/
	int		(*func)(int);				/*　呼出関数					*/
	struct	winb_t	*winb;				/*　ウインドウメニュー構造体	*/
};
struct winb_t {
	struct	winb_title_t winb_title;	/*　タイトル		*/
	int		xlen;						/*　ｘ方向文字数	*/
	int		ylen;						/*　ｙ方向文字数	*/
	int		n;							/*　選択項目数		*/
	struct	winb_item_t winb_item[];	/*　文字列			*/
};
struct winb_para_t {
	char	*j;							/*　パラメータ文字列(日本語)	*/
	char	*e;							/*　パラメータ文字列(英語）		*/
	int		*para_value;				/*　パラメータ					*/
	int		para_vmax;					/*　上限値						*/
	int		para_vmin;					/*　下限値						*/
	int		para_step;					/*　変更ステップ				*/
};
//　パラメータ設定構造体(メッセージ付き）	
struct winb_para_menu_t {					
	char	*j;							/*　メニュー文字列(日本語)		*/
	char	*e;							/*　メニュー文字列(英語）		*/
};
struct winb_para_tm {						
	char	*j;									/*　パラメータ文字列(日本語)	*/
	char	*e;									/*　パラメータ文字列(英語）		*/
	int		*para_value;						/*　パラメータ					*/
	int		para_vmax;							/*　上限値						*/
	int		para_vmin;							/*　下限値						*/
	int		para_step;							/*　変更ステップ				*/
	struct	winb_para_menu_t *winb_para_menu;	/*　メニュー文字列				*/
};

//#if R_WINB | R_SWINB | RWINB
#if defined(R_WINB) || defined(R_SWINB) || defined(RWINB)	// 2009/07/28
static struct {
	struct winb_t *winb;
	int		cx;
	int		cy;
	int		top;
	int		xs;
	int		ys;
} Menu_bilingual[ WIN_NEST_MAX ];
#endif

/* コメントアウト 2009/07/28
//　ウィンドウメッセイジ用
struct winb_mes_t {
	int		xlen;
	int		ylen;
	char	*str[];
};
//　ファンクション設定用
#ifdef	R_WINB
struct winb_mes_t winb_mes = { 14, 1, "function" };
#endif
//#if R_SWINB | RWINB
#if defined(R_SWINB) || defined(RWINB)	// 2009/07/28
extern struct winb_mes_t winb_mes;
#endif
*/

#define	WINB_STEP_ATTR_MASK	0xFF000000
#define	WINB_STEP_ATTR_DEC	0x00000000
#define	WINB_STEP_ATTR_HEX	0x80000000

//　パラメータ変更関数
int	R_winb_parameter( struct winb_para_t [], int, char *, char * );
int	R_winb_parameter_with_mess( struct winb_para_tm [], char *, char * );

/**
*	@brief	ポップアップｗｉｎｄｏｗ
*	@param	int	x					最初のウィンドウ左上ｘ座標
*			int	y					最初のウィンドウ左上ｙ座標
*			int	disp_mode			ウィンドウ処理
*				WINB_NON_CLOSE	新しいウィンドウを開いても前のウィンドウは残す
*				WINB_CLOSE   	新しいウィンドウを開いたら前のウィンドウは消す
*				WINB_WAIT		キャンセルで終了するときマウスSWが離れるのを待って関数を抜ける
*				WINB_NOWAIT		チェックしない
*			struct winb_t*	winb	ウインドウメニュー構造体
*	@return	int
*			0～		最初のウィンドウでの選択番号（呼出関数とメニュー構造体が共にNULLの項目を選択した場合）
*			-2		最初のウィンドウでキャンセル
*			ERROR3	関数の戻り値がERRORで終了又は２番目以降のウィンドウで終了
*/
int R_winb_menu(int x, int y, int disp_mode, struct winb_t* winb);

// 2012.07.25
void	R_winb_callback(int (*func)(int,int));

#ifdef __cplusplus
}
#endif

#endif	// R_WINB_H_
