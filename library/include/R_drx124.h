/******************************************************************************
	R_drx124.h / R_drx124.c
	
	�ύX����
	Ver 4.01	2009/08/06	�V�K
	Ver 4.01	2009/08/20	�}���`�X���b�h�Ή�
	Ver 4.01	2009/09/01	DRX125(2A�Z�p���X���샂�[�h�j�ǉ�		by tanaka
	Ver 4.01	2010/01/20	DRX127(16ch�h���C�o�j�����ǉ�			by tanaka
	Ver 4.01	2012/04/21	DRX136(���^�Œǉ�)						by tanaka
	Ver 4.01	2015/05/07	DR-X-147�Ή�
	Ver 4.01	2015/07/10	DR-X-147�L�����u���[�V�������P
	Ver 4.01	2015/09/02	DR-X-147�L�����u���[�V�������P
	Ver 4.01	2016/02/24	DR-X-147�L�����u���[�V�����\���ǉ�
	Ver 4.01	2017/07/06	R_drx147_measure_supply_voltage�֐��ǉ�
	Ver 4.01	2017/07/06	R_drx147_measure_load_voltage�֐��ǉ�
    Ver 5.01    2020/09/23  R_drx124_set_port�֐��ǉ�               by tanaka
	Ver 5.042	2021/03/24	DR-X-147��ID�Փˎ��̃��b�Z�[�W��ύX
******************************************************************************/

#ifndef	R_DRX124_H_
#define	R_DRX124_H_

#include "R_stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

//
#define	_BOARD_JPSW0	0
#define	_BOARD_JPSW1	1
#define	_BOARD_JPSW2	2
#define	_BOARD_JPSW3	3

#define	_DRX124_CH_0	1
#define	_DRX124_CH_1	2
#define	_DRX124_CH_2	4
#define	_DRX124_CH_3	8
#define	_DRX124_CH_4	16
#define	_DRX124_CH_5	32
#define	_DRX125_CH_0	1
#define	_DRX125_CH_1	2
#define	_DRX125_CH_2	4
#define	_DRX125_CH_3	8
#define	_DRX125_CH_4	16
#define	_DRX125_CH_5	32
#define	_DRX127_CH_0	1
#define	_DRX127_CH_1	2
#define	_DRX127_CH_2	4
#define	_DRX127_CH_3	8
#define	_DRX127_CH_4	16
#define	_DRX127_CH_5	32
#define	_DRX127_CH_6	64
#define	_DRX127_CH_7	128
#define	_DRX127_CH_8	256
#define	_DRX127_CH_9	512
#define	_DRX127_CH_10	1024
#define	_DRX127_CH_11	2048
#define	_DRX127_CH_12	4096
#define	_DRX127_CH_13	8192
#define	_DRX127_CH_14	16384
#define	_DRX127_CH_15	32768
#define	_DRX136_CH_0	1
#define	_DRX136_CH_1	2
#define	_DRX136_CH_2	4
#define	_DRX136_CH_3	8
#define	_DRX136_CH_4	16
#define	_DRX136_CH_5	32

int		R_drx124_open(int nID);
int		R_drx124_restart(int nHandle);
void	R_drx124_dac_close(int nHandle);
int 	r_drx124_dac_current_initial(int nHandle);
int		R_drx124_set_current(int nHandle,int port,int nTurn,int nData);
int		R_drx124_dac_set(int nHandle, int nChannel, int nTurn, int nData);
int		R_drx124_set_time(int nHandle, int nTurn, int nTime);
int		R_drx124_set_pattern(int nHandle, int nTurn, int nPattern);
int 	R_drx124_set_turn(int nHandle, int nTurn, int nWait);
int 	R_drx124_set_turn_endcheck(int nHandle );
int		R_drx124_out_pulse(int nHandle);
int		R_drx124_get_nowturn(int nHandle);
int		R_drx124_get_maxturn(int nHandle);
int		R_drx124_getw_eeprom(int nHandle,int reg);
int		R_drx124_putw_eeprom(int nHandle,int reg,int data);
int		R_drx124_get_errorcount(int nHandle );
int		R_drx124_current_limit(int nHandle, int nChannel);
int		R_drx124_set_port(int nHandle, int nPattern);
#define R_drx124_close( nHandle )                            R_drx124_dac_close( nHandle )


//DR-X-125
#define	R_drx125_open( nID )                                 R_drx124_open( nID )
#define	R_drx125_restart( nHandle )                          R_drx124_restart( nHandle )
#define	R_drx125_dac_close( nHandle )                        R_drx124_dac_close( nHandle )
#define	R_drx125_set_current( nHandle, nPort, nTurn, nData ) R_drx124_set_current( nHandle, nPort, nTurn, nData )
#define	R_drx125_dac_set( nHandle, nChannel, nTurn, nData )  R_drx124_dac_set( nHandle, nChannel, nTurn, nData )
#define	R_drx125_set_time( nHandle, nTurn, nTime )           R_drx124_set_time( nHandle, nTurn, nTime )
#define	R_drx125_set_pattern( nHandle, nTurn, nPattern )     R_drx124_set_pattern( nHandle, nTurn, nPattern )
#define	R_drx125_set_turn( nHandle, nTurn, nWait )           R_drx124_set_turn( nHandle, nTurn, nWait )
#define	R_drx125_set_turn_endcheck( nHandle )                R_drx124_set_turn_endcheck( nHandle )
#define	R_drx125_out_pulse( nHandle )                        R_drx124_out_pulse( nHandle )
#define	R_drx125_get_nowturn( nHandle )                      R_drx124_get_nowturn( nHandle )
#define	R_drx125_getw_eeprom( nHandle, reg )                 R_drx124_getw_eeprom( nHandle, reg )
#define	R_drx125_putw_eeprom( nHandle, reg, data )           R_drx124_putw_eeprom( nHandle, reg, data )
#define	R_drx125_get_errorcount( nHandle )                   R_drx124_get_errorcount( nHandle )
#define R_drx125_close( nHandle )                            R_drx124_dac_close( nHandle )

//DR-X-136
#define	R_drx136_open( nID )                                 R_drx124_open( nID )
#define	R_drx136_restart( nHandle )                          R_drx124_restart( nHandle )
#define	R_drx136_dac_close( nHandle )                        R_drx124_dac_close( nHandle )
#define	R_drx136_set_current( nHandle, nPort, nTurn, nData ) R_drx124_set_current( nHandle, nPort, nTurn, nData )
#define	R_drx136_dac_set( nHandle, nChannel, nTurn, nData )  R_drx124_dac_set( nHandle, nChannel, nTurn, nData )
#define	R_drx136_set_time( nHandle, nTurn, nTime )           R_drx124_set_time( nHandle, nTurn, nTime )
#define	R_drx136_set_pattern( nHandle, nTurn, nPattern )     R_drx124_set_pattern( nHandle, nTurn, nPattern )
#define	R_drx136_set_turn( nHandle, nTurn, nWait )           R_drx124_set_turn( nHandle, nTurn, nWait )
#define	R_drx136_set_turn_endcheck( nHandle )                R_drx124_set_turn_endcheck( nHandle )
#define	R_drx136_out_pulse( nHandle )                        R_drx124_out_pulse( nHandle )
#define	R_drx136_get_nowturn( nHandle )                      R_drx124_get_nowturn( nHandle )
#define	R_drx136_getw_eeprom( nHandle, reg )                 R_drx124_getw_eeprom( nHandle, reg )
#define	R_drx136_putw_eeprom( nHandle, reg, data )           R_drx124_putw_eeprom( nHandle, reg, data )
#define	R_drx136_get_errorcount( nHandle )                   R_drx124_get_errorcount( nHandle )
#define R_drx136_close( nHandle )                            R_drx124_dac_close( nHandle )



//DR-X-127
#define	R_drx127_open( nID )                                 R_drx124_open( nID )
#define	R_drx127_restart( nHandle )                          R_drx124_restart( nHandle )
#define	R_drx127_dac_close( nHandle )                        R_drx124_dac_close( nHandle )
//#define	R_drx127_set_current( nHandle, port, turn, data )    R_drx124_set_current( nHandle, port, turn, data )
//#define	R_drx127_dac_set( nHandle, nChannel, nTurn, nData )  R_drx124_dac_set( nHandle, nChannel, nTurn, nData )
//#define	R_drx127_set_time( nHandle, nTurn, nTime )           R_drx124_set_time( nHandle, nTurn, nTime )
//#define	R_drx127_set_pattern( nHandle, nTurn, nPattern )     R_drx124_set_pattern( nHandle, nTurn, nPattern )
//#define	R_drx127_set_turn( nHandle, nTurn, nWait )           R_drx124_set_turn( nHandle, nTurn, nWait )
//#define	R_drx127_set_turn_endcheck( nHandle )                R_drx124_set_turn_endcheck( nHandle )
//#define	R_drx127_out_pulse( hDrx124 )                        R_drx124_out_pulse( hDrx124 )
//#define	R_drx127_get_nowturn( hDrx124 )                      R_drx124_get_nowturn( hDrx124 )
#define	R_drx127_getw_eeprom( hDrx124, reg )                 R_drx124_getw_eeprom( hDrx124, reg )
#define	R_drx127_putw_eeprom( hDrx124, reg, data )           R_drx124_putw_eeprom( hDrx124, reg, data )
#define	R_drx127_get_errorcount( nHandle )                   R_drx124_get_errorcount( nHandle )
#define R_drx127_close( nHandle )                            R_drx124_dac_close( nHandle )

int		R_drx127_set_current( int nHandle, int nTrig, int port    , int turn    , int data  );
int		R_drx127_dac_set(     int nHandle, int nTrig, int nChannel, int nTurn   , int nData );
int		R_drx127_set_time(    int nHandle, int nTrig, int nTurn   , int nTime               );
int		R_drx127_set_pattern( int nHandle, int nTrig, int nTurn   , int nPattern            );
int 	R_drx127_set_turn(    int nHandle, int nTrig, int nTurn   , int nWait               );
int 	R_drx127_set_turn_endcheck(int nHandle , int nTrig);
int		R_drx127_out_pulse(   int nHandle, int nTrig );
int		R_drx127_get_nowturn( int nHandle, int nTrig );

int		R_drx147_init(int hDrx124);
int		R_drx147_init_channel(int hDrx124, int channel);
int		r_drx147_init_debug(int on_off);

// ���C�u���������֐�
void	RDrx124Init(void);
int		RDrx124Open(int num, int device);
int		RDrx125Open(int num, int device);
int		RDrx136Open(int num, int device);
int		RDrx147Open(int num, int device);
int		RDrx127Open(int num, int device);
int		r_drx147_open(const char* device_name, int count);
int		r_drx124_get_revision(int hDrx124);

/**
*	@brief	�d���d���̑���
*			DR-X-147�̒�d���o�͉�H�̓d���d���𑪒肵�܂��B
*
*	@param	int		handle		�n���h���BR_drx124_open�֐��Ŏ擾�B
*			int		channel		�d���o�͉�H�`�����l���i0�`5�j
*
*	@return
*			0.0��菬�����l		�G���[����
*			0.0�ȏ�̒l			�d���l�ivolt�j
*/
double	R_drx147_measure_supply_voltage(int handle, int channel);

/**
*	@brief	���דd���̑���
*			DR-X-147�̒�d���o�͉�H�̕��דd���𑪒肵�܂��B
*
*	@param	int		handle		�n���h���BR_drx124_open�֐��Ŏ擾�B
*			int		channel		�d���o�͉�H�`�����l���i0�`5�j
*			int		current		�o�͓d���l�iuA�j
*			int		interval	�d���o�͂���d���v���܂ł̊Ԋu�iusec�j
*
*	@return
*			0.0��菬�����l		�G���[����
*			0.0�ȏ�̒l			�d���l�ivolt�j
*/
double	R_drx147_measure_load_voltage(int handle, int channel, int current, int interval);

int		r_drx147_set_dac(int handle, int channel, int code);
int		r_drx147_set_gate(int handle, int on_off);

#ifdef __cplusplus
}
#endif

#endif	// R_DRX124_H_
