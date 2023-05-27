/******************************************************************************
	R_INFO.H / R_INFO.CPP
	
	変更履歴
	Ver 3.49	2006/09/07	暫定ソース
	Ver 4.01	2011/03/24	LIB_TIME更新
	Ver 5.045	2021/06/15	R_info_full_path_to_executable関数追加
******************************************************************************/

#ifndef	R_INFO_H_
#define	R_INFO_H_

#include <time.h>

#ifdef __cplusplus
#include <string>
#include <vector>
const std::vector<std::string>& RInfoCommandLineArguments();
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define	R_LOG_INFO		0
#define	R_LOG_WARN		1
#define	R_LOG_ERROR		2

int	R_info_open(int nSize);
int	R_info_close(void);
int	R_info_set_info(int nLevel, int nCode, char* pMessage);
int	R_info_get_last_info(int nLevel);
int	R_info_get_last_error(void);

int		R_info_init_boot_time(time_t time);
time_t	R_info_boot_time(time_t* pTime);
void	R_info_past_time(int* pHour, int* pMinute, int* pSecond);
int		R_info_command_line_argc(void);
const char*	R_info_command_line_argv(int num);
const char*	R_info_full_path_to_executable(void);

void	r_info_func_is_not_supported(const char* func_name);
void	r_info_set_command_line_arguments(int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif	// R_INFO_H_
