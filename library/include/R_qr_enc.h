/******************************************************************************
	R_qr_enc.cpp / R_qr_enc.h
	
	変更履歴
	Ver 4.01	2017/10/13	新規作成
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief QRエンコード構造体のポインタ
 *	ユーザソースからは構造体のメンバにアクセスしないでください。
 */
typedef struct {
	int	init;
	
	int	casesensitive;
	int	eightbit;
	int	version;
	int	margin;
	int	micro;
	
	int	level;
	int	color0;
	int	color1;
} R_QR_ENCODE;

/*!
 * @brief QRコード作成パラメータ初期化関数
 *	デフォルト値で初期化されたQRエンコード構造体を返します。
 * @retval	初期化されたQRエンコード構造体
 */
R_QR_ENCODE	R_qr_encode_struct(void);

/*!
 * @brief QRコード表示関数
 *	グレープレーンにQRコードを表示します。
 * @param	qr	QRエンコード構造体のポインタ
 * @param	page	表示先グレーメモリのページ番号
 * @param	x		表示位置、QRコードの左座標
 * @param	y		表示位置、QRコードの上座標
 * @param	size	１セルあたりのピクセル数
 * @param	string	エンコードする文字列
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_string_on_gray_plane(const R_QR_ENCODE* qr, int page, int x, int y, int size, const char* string);

int	R_qr_encode_string_on_binary_plane(const R_QR_ENCODE* qr, int page, int x, int y, int size, const char* string);

int	R_qr_encode_string_on_draw_plane(const R_QR_ENCODE* qr, int x, int y, int size, const char* string);

/*!
 * @brief マージン（余白）の設定
 *	QRコードのまわりの余白を設定します。
 *	デフォルトは4ですが、変更したい時に使用してください。
 *	QRコード規格のでは4以上必要です。
 * @param	qr	QRエンコード構造体のポインタ
 * @param	cells	マージンのセル数
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_set_margin(R_QR_ENCODE* qr, int cells);

/*!
 * @brief QRコード表示色の設定
 *	デフォルトは、color0=0、color1=0x00FFFFFFです。
 *	変更したい時に使用してください。
 * @param	qr	QRエンコード構造体のポインタ
 * @param	color0	セルの表示色（通常は黒色）
 * @param	color1	背景の表示色（通常は白色）
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_set_color(R_QR_ENCODE* qr, int color0, int color1);

/*!
 * @brief 誤り訂正レベルの設定
 *	QRコードの誤り訂正レベル（L,M,Q,H）を設定します。
 *	デフォルトは0ですが、変更したい時に使用してください。
 * @param	qr	QRエンコード構造体のポインタ
 * @param	level	0:L、1:M、2:Q、3:H
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_set_error_correction_level(R_QR_ENCODE* qr, int level);

/*!
 * @brief バージョンの設定
 *	QRコードのバージョンを設定します。
 *	version=1でセル数は21x21、version=40でセル数は177x177になります。
 *	version=0に設定した場合は、文字数によって自動的にバージョンを決定します。
 *	デフォルトは0ですが、変更したい時に使用してください。
 * @param	qr	QRエンコード構造体のポインタ
 * @param	version	0:auto、1～40
 * @retval	OK
 * @retval	ERROR
 */
int	R_qr_encode_set_symbol_version(R_QR_ENCODE* qr, int version);

/*!
 * @brief セル数の取得
 *	QRコードの表示セル数を取得します。
 *	マージンは含みません。
 * @param	qr	QRエンコード構造体のポインタ
 * @param	string	エンコードする文字列
 * @retval	セル数
 * @retval	0	エラー
 */
int	R_qr_encode_cells(R_QR_ENCODE* qr, const char* string);

#ifdef __cplusplus
}
#endif
