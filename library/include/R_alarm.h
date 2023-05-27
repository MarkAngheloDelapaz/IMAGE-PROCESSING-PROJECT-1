/******************************************************************************
	R_alarm.h
	
	変更履歴
	Ver 4.01	2014/10/30	新規作成
******************************************************************************/

#ifndef	R_ALARM_H_
#define	R_ALARM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int		code;
	int		class_code;			// アラームのクラス
	int		priority;			// アラームの優先順位
	const char*	title_jp;		// アラームのメッセージ
	const char*	title_en;		// アラームのメッセージ

	int		total_counter;		// アラーム延べ回数カウンタ
	int		prior_counter;		// アラームの回数カウンタ
} R_ALARM_ITEM;

typedef struct {
	int		code;
	const char*	title_jp;		// アラームのメッセージ
	const char*	title_en;		// アラームのメッセージ
} R_ALARM_CLASS;

typedef struct {
//	int		prior_number;		// 最終決定のアラーム番号
//	int
	int		prior_code;
	int		current_priority;

	int		total_of_alarm;		// アラーム同時発生数
/*
	int*	total_counter;		// アラーム延べ回数カウンタ
	int*	prior_counter;		// アラームの回数カウンタ
	
	int*	priority;			// アラームの優先順位
	const char**	message;	// アラームのメッセージ
	int*	class;				// アラームのクラス
*/
	R_ALARM_ITEM*	item;		//
	R_ALARM_CLASS*	class_item;
	
	int		num_of_code;			// アラームコードの最大数
	int		num_of_class;
	
	int		bytes_of_flags;			//
	int*	flags;					// 発生アラームフラグ
	int*	backup_of_flags;		// 発生アラームフラグの復帰用バックアップ
	int		backup_of_prior_code;	// 最終決定アラーム番号の復帰用バックアップ
} R_ALARM;

R_ALARM*	R_alarm_open(int max_code);
void	R_alarm_close(R_ALARM* alarm);
int	R_alarm_counter_init(R_ALARM* alarm);
int	R_alarm_set(R_ALARM* alarm, int alarm_number);
int	R_alarm_decide(R_ALARM* alarm, int mode);
int	R_alarm_resume(R_ALARM* alarm);
int	R_alarm_load(R_ALARM* alarm, const char* filename);

// カウンタ分類構造体
typedef struct {
	int nClass;						// カウンタ分類番号
	char* sMessJp;					// 分類名称(日本語)
	char* sMessEn;					// 分類名称(英語)
//} R_CS_COUNTER_CLASS;
} R_ALARM_COUNTER_CLASS;

// カウンタＮＧ番号構造体
typedef struct {
	int   nCode;					// ＮＧ番号
	char* sMessJp;					// ＮＧ名称(日本語)
	char* sMessEn;					// ＮＧ名称(英語)
	int   nClass;					// 分類番号
	int		priority;
//} R_CS_COUNTER_NG_CODE;
} R_ALARM_COUNTER_NG_CODE;

int R_alarm_init_by_struct(R_ALARM* alarm, R_ALARM_COUNTER_NG_CODE* ng_code, R_ALARM_COUNTER_CLASS* class_code);

#ifdef __cplusplus
}
#endif

#endif	// R_ALARM_H_
