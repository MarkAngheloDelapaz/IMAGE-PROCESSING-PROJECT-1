/******************************************************************************
	R_lj.h / R_lj.c

	�ύX����
	Ver 4.01	2013.06.07	�V�K�ǉ�
	Ver 4.01	2016.11.14	R_LJ_V7000S0�Ή�
	Ver 5.012	2019.04.26	R_lj_profile_raw_data_save/load�o�O�t�B�N�X
******************************************************************************/

/*
�o�O���
R_lj_profile_raw_data_save��R_lj_profile_raw_data_load�̃o�O�iVer.4.01b200�Ŕ����j
	�EVer.4.01b199�ȑO�̃��C�u�����ŃZ�[�u�����t�@�C����b200�ȍ~�Ń��[�h����ƃG���[����������B
		�f�[�^�̓��[�h�ł��Ă���悤�Ɍ����邪�P��f�Y���Ă���B
		���̖���Ver.5.012�ŏC���ς݂ł��B
		Ver.5.012�ȑO�̃t�@�C���S�Ă𐳏�Ƀ��[�h�\�ł��B
	�EVer.5.012�ȍ~�̃��C�u�����ŃZ�[�u�����t�@�C����Ver.4.01b200�`Ver.5.011�Ń��[�h����ƃG���[����������B
		�\���󂠂�܂���BVer.5.012�ȍ~�Ń��[�h���Ă��������B
	�EVer.4.01b200�`Ver.5.011�̃��C�u�����ŃZ�[�u�����t�@�C����b199�ȑO�Ń��[�h����Ɛ������邪�P��f�Y���Ă���B
		���̖��̓G���[���������Ȃ��̂ŋC�����ĉ������B
*/

#ifndef R_LJ_H_
#define R_LJ_H_

#include <stdint.h>

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

int R_lj_open(int type);

#define R_LJ_V7000		1	//!< LJ-V7000
#define R_LJ_G5000		2	//!< LJ-G5000
#define R_LJ_V7000S0	3	//!< LJ-V7000S0

/**
*	@brief	���[�U�ψʌv�̃I�[�v��
*	
*	@param	type	���[�U�ψʌv�̎��<BR>
*					R_LJ_V7000 LJ-V7000���g�p<BR>
*					R_LJ_G5000 LJ-G5000���g�p�i�Ή��\��j
*	@return			���[�U�ψʌv�̃n���h��<BR>
*					�G���[���͕��̒l
*/
int R_lj_connect(int handle, const char* ip_address);

/*!
@brief				���[�U�ψʌv�Ƃ̃C�[�T�l�b�g�ڑ�
*/
int R_lj_get_error(int handle);
int R_lj_clear_error(int handle, int code);

int R_lj_change_active_program(int handle, int program_number);

int R_ljv7000_prestart(int handle);
int R_ljv7000_start(int handle);
int R_ljv7000_stop(int handle);
int R_ljv7000_data_conv(int handle, int num);

int R_ljv7000_get_setting(int handle);

#pragma pack(1)
typedef struct {
	uint32_t file_type;				//!< �t�@�C���^�C�v "R_LJ"
	uint32_t version;				//!< �t�@�C���o�[�W����
	uint32_t bytes_of_file;			//!< �t�@�C���T�C�Y
	uint32_t bytes_of_header;		//!< �w�b�_�T�C�Y
	
	uint32_t profile_data_count;	//!< �P�v���t�@�C��������̃f�[�^��
	uint32_t number_of_profiles;	//!< �v���t�@�C����
	uint32_t profile_data_unit;		//!< �����f�[�^�P��
	uint32_t profile_data_pitch;	//!< X�����s�b�`
	uint32_t bytes_of_comment;		//!< �R�����g���̈�T�C�Y
	uint32_t bytes_of_data;			//!< �f�[�^�̈�T�C�Y
	// �ȉ��̓t�@�C���Z�[�u���Ȃ��B�|�C���^�Ȃ̂�32bit����64���ŃT�C�Y���قȂ�B
	char* comment;					//!< �R�����g���ւ̃|�C���^
	int32_t* data;					//!< 32bit�v���t�@�C���f�[�^�ւ̃|�C���^
	uint16_t*	image;				//!<
} R_LJ_PROFILE_RAW_DATA;
#pragma pack()

R_LJ_PROFILE_RAW_DATA* R_lj_profile_raw_data_alloc(int x, int y);
int R_lj_profile_raw_data_free(const R_LJ_PROFILE_RAW_DATA* data);
int R_lj_profile_raw_data_save(const R_LJ_PROFILE_RAW_DATA* data, const char* file_name);

typedef enum {
	R_LJ_PROFILE_LOAD_FIX_SIZE = 0,
	R_LJ_PROFILE_LOAD_LIMIT = 1,
	R_LJ_PROFILE_LOAD_RESIZE = 2,
	R_LJ_PROFILE_LOAD_EXPAND = 3
} R_LJ_PROFILE_LOAD_MODE;
int R_lj_profile_raw_data_load(R_LJ_PROFILE_RAW_DATA* data, const char* file_name, R_LJ_PROFILE_LOAD_MODE mode);

int	R_lj_get_profile(int handle, R_LJ_PROFILE_RAW_DATA* data_a, R_LJ_PROFILE_RAW_DATA* data_b, int y);
int R_lj_get_profiles(int handle, int count, R_LJ_PROFILE_RAW_DATA* data_a, R_LJ_PROFILE_RAW_DATA* data_b);

int	R_lj_get_all_settings_to_file(int handle, const char* file_name);
int	R_lj_set_all_settings_from_file(int handle, const char* file_name);

int	R_lj_get_profiles(int handle, int number_of_profiles, R_LJ_PROFILE_RAW_DATA* data_a, R_LJ_PROFILE_RAW_DATA* data_b);
int	R_lj_get_profiles_start(int handle, int number_of_profiles, R_LJ_PROFILE_RAW_DATA* data_a, R_LJ_PROFILE_RAW_DATA* data_b);
int R_lj_get_profiles_end_check(int handle);
int	R_lj_get_profiles_number(int handle);

int R_lj_conv_profile_raw_to_gray(const R_LJ_PROFILE_RAW_DATA* raw, R_RECT* rect_entry, int gray_page, R_POINT* point_entry, int reso, int offset, int base);
#define R_LJ_RESO_51200	0x80000009	// 51.2um/digit
#define R_LJ_RESO_25600	0x80000008	// 25.6um/digit
#define R_LJ_RESO_12800	0x80000007	// 12.8um/digit
#define R_LJ_RESO_6400	0x80000006	// 6.4um/digit
#define R_LJ_RESO_3200	0x80000005	// 3.2um/digit
#define R_LJ_RESO_1600	0x80000004
#define R_LJ_RESO_800	0x80000003
#define R_LJ_BASE_CENTER	0
#define R_LJ_BASE_BOTTOM	1

int R_lj_gray_resolution(int parameter);

int R_lj_profile_invalid_data_interpolation(R_LJ_PROFILE_RAW_DATA* src, R_LJ_PROFILE_RAW_DATA* dst, int count);
int R_lj_profile_invalid_data_processing(R_LJ_PROFILE_RAW_DATA* src, R_LJ_PROFILE_RAW_DATA* dst, int processing_times, int resume_times);

// 2016.11.17 �b��֐� by araki
int	R_lj_conv_image_raw_to_gray(const R_LJ_PROFILE_RAW_DATA* raw, R_RECT* rect_entry, int gray_page, R_POINT* point_entry, int mode);

struct ljv7000_Environment_settings {
	char Device_name[32];								// 00
	uint32_t Operation_at_next_power_on;				// 01
	uint32_t Highspeed_communication_band_restriction;	// 02
	uint32_t MTU_during_highspeed_communication;		// 03
	uint32_t IP_address;								// 04
	uint32_t Subnet_mask;								// 05
	uint32_t Gateway;									// 06
	uint32_t TCP_command_port_number;					// 07
	uint32_t TCP_highspeed_power_number;				// 08
	uint32_t UDP_port_number;							// 09
	uint32_t Baud_rate;									// 0A
	uint32_t Parity;									// 0B
};

struct ljv7000_Common_measurement_settings {
	struct {
		uint32_t Operation_mode;						// 00
		uint32_t Memory_allocation;						// 01
		uint32_t Operation_when_memory_full;			// 02
		uint32_t Parallel_imaging;						// 03
		uint32_t Strobe_output_time;					// 04
		uint32_t Head_installation;						// 05
		uint32_t TRG_minimum_input_time;				// 06
		uint32_t ENCODER_minimum_input_time;			// 07
		uint32_t Control_terminal_minimum_input_time;	// 08
		uint32_t Change_program;						// 09
	};
};

struct ljv7000_Trigger_settings {
	uint32_t Standard_setting_advanced_setting;			// 00
	uint32_t Trigger_mode;								// 01
	uint32_t Sampling_period;							// 02
	uint32_t Batch_measurement;							// 03
	uint32_t Inter_trigger_pitch;						// 04
	uint32_t Inter_trigger_pitch_count;					// 05
	uint32_t Mutual_interference_prevention;			// 06
	uint32_t Input_mode;								// 07
	uint32_t Skipping;									// 08
	uint32_t Points_to_skip;							// 09
	uint32_t Batch_count;								// 0A
};

struct ljv7000_Imaging_settings {
	uint32_t Standard_setting_advanced_setting;			// 00
	uint32_t Binning;									// 01
	uint32_t X_direction;								// 02
	uint32_t Z_direction;								// 03
	struct {
		uint32_t Measurement_target;					// 04
		uint32_t CMOS_sensitivity;						// 05
		uint32_t Exposure_time;							// 06
		uint32_t Imaging_mode;							// 07
		uint32_t Multi_emission_light_intensity_optimization_detail;	// 08
		uint32_t Multi_emission_composite_detail;		// 09
		uint8_t Mask_setting[6][16];					// 0A
		uint32_t Control_mode;							// 0B
		uint32_t Upper_limit_value;						// 0C
		uint32_t Lower_limit_value;						// 0D
		uint32_t FB_target_range;						// 0E
		uint32_t Peak_detection_level;					// 0F
		uint32_t Invalid_data_interpolation_count;		// 10
		uint32_t Peak_selection;						// 11
		uint32_t Peak_width_filter;						// 12
		uint8_t Imaging_internal_settings[16];			// 13
	} Head[2];
};

struct ljv7000_Profile_settings {
	uint32_t Standard_setting_advanced_setting;			// 00
	uint32_t Combine_wide;								// 01
	uint32_t Compression_X_axis;						// 02
	uint32_t Compression_time_axis;						// 03
	uint32_t Time_axis_compression_count;				// 04
	uint32_t Dead_zone_processing_enabled_disabled;		// 05
	struct {
		uint32_t Reverse_X;								// 06
		uint32_t Reverse_Z;								// 07
		uint32_t Shift_X;								// 08
		uint32_t Shift_Z;								// 09
		uint32_t Median_X_axis;							// 0A
		uint32_t Smoothing;								// 0B
		uint32_t Median_time_axis;						// 0C
		uint32_t Averaging;								// 0D
		uint32_t Invalid_data_processing_time_axis;		// 0E
		uint8_t Tilt_correction[28];					// 0F
		uint8_t Height_correction[28];					// 10
	} Head[2];
};

struct ljv7000_setting_details {
	struct ljv7000_Environment_settings environment_settings;
	struct ljv7000_Common_measurement_settings common_measurement_settings;
	struct ljv7000_Program {
		struct ljv7000_Trigger_settings trigger_settings;
		struct ljv7000_Profile_settings profile_settings;
	} program[16];
};

#ifdef __cplusplus
}
#endif

#endif // LJV7000_H
