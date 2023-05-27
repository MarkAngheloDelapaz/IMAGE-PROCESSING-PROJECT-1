/********************************************************************
	パラレルポート制御サブルーチン  for PC/AT
		(出展：トラ技1995.10, 1996.6など)

										Program by K.Matsunaga
********************************************************************/

/**********************************************************
	使用方法
		ParallelOpen()					パラレルポートの初期化
		ParallelClose()					パラレルポートの終了
		ParallelMode()					パラレルポートの方向設定
		ParallelBitSet(port,bit,data)	パラレルポートのビットセット
		ParallelGetBit(port,bit)		パラレルポートのビット状態の取得

**********************************************************/

// *** 定義 ***
// ポートアドレス
// LPT-1
//#define PARALLEL_DATA    0x3bc				// データ
//#define PARALLEL_STATUS  (PARALLEL_DATA+1)	// 制御入力
//#define PARALLEL_CONTROL (PARALLEL_DATA+2)	// 制御出力

// LPT-2
#define PARALLEL_DATA    0x378					// データ
#define PARALLEL_STATUS  (PARALLEL_DATA+1)		// 制御入力
#define PARALLEL_CONTROL (PARALLEL_DATA+2)		// 制御出力

// LPT-3
//#define PARALLEL_DATA    0x278				// データ
//#define PARALLEL_STATUS  (PARALLEL_DATA+1)	// 制御入力
//#define PARALLEL_CONTROL (PARALLEL_DATA+2)	// 制御出力

// ステータスポート定義
#define SLCT    0x10		// select
#define PE      0x20		// paper end
#define ACK     0x40		// ack
#define BUSY    0x80		// busy
#define ER		0x08		// error

// コントロールポート定義
#define STROBE  0x01
#define AUTOFD  0x02		// 
#define INIT    0x04		// 
#define SLCTIN  0x08		// 
#define INT     0x10		// 
#define DIR     0x20		// 

// パラレルポートモード定義
#define PARALLEL_IN   0
#define PARALLEL_OUT  1
#define PARALLEL_ON   2
#define PARALLEL_OFF  3

// ステータスポート ビット定義
#define QR_START      ER			// input<0x08>
#define QR_RESERVE_1  SLCT			// input<0x10>
#define QR_RESERVE_2  ACK			// input<0x40>

int ParallelOpen(void);
void ParallelClose(void);
/*
ParallelMode()					パラレルポートの方向設定
ParallelBitSet(port,bit,data)	パラレルポートのビットセット
ParallelGetBit(port,bit)		パラレルポートのビット状態の取得
*/

