/******************************************************************************
	R_GPDAC.H / R_GPDAC.C
	
	�ύX����
		Ver 3.28	2001/02/07	���������[�X
		Ver 3.30	2002/01/10	DR-CY-82�ADR-CY-83�Ή�
		Ver 3.32	2002/09/18	DR-CY-83A/B�Ή��A�������Ή�
		Ver 3.34	2002/12/24	DR-CY-80�Ή�
		Ver 3.34	2003/03/10	r_drcy83_dac_open�����֐���
		Ver 3.35	2003/04/7	R_gpdac_set_cc,R_gpdac_open�ǉ�
		Ver 3.35	2003/04/7	R_set_DAC_drcy88�ǉ�
		Ver 3.36	2003/05/13	DR-CY-80A�Ή�
		Ver 3.39	2003/06/24	R_gpdac_check_board�ǉ�
		Ver 3.46	2004/08/08	DR-CY-101�Ή�
		Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
		Ver 4.01	2014/06/05	DR-CY-140�Ή�
		Ver 4.01	2014/08/25	R_set_DAC_drcy140�֐��ǉ�
		Ver 4.01	2015/09/29	DR-CY-143 DOS�Ή�
		Ver 5.042	2021/03/02	R_gpdac_disable_bias_shifting�֐��ǉ�
******************************************************************************/

#ifndef	R_GPDAC_H_
#define	R_GPDAC_H_

#ifdef __cplusplus
extern "C" {
#endif

struct gpdac_info_t {		//��d����̏��
	int board_type;			//��̎��
	int revision;			//���r�W����
	int channel;			//�`�����l����
	int current_max[8];		//�e�`�����l���̍ő�d���l[uA]
	int voltage_max[8];		//�e�`�����l���̍ő�d���l[mV]
};


int R_gpdac_set_current(int channel,int DacChannel,int data);
int R_gpdac_open(int channel);
int R_gpdac_check_board(int channel);
int R_gpdac_get_info(int channel, struct gpdac_info_t *info );
int R_set_DAC_drcy64(int channel, int ch, int data);
int R_set_DAC_incy67( int nChannel, int nCh, int nData );
int R_set_DAC_drcy82(int channel, int ch, int data);		//shin DRCY82 2001/7/12
void R_set_DAC_drcy67(int channel, int nData0, int nData1, int nData2, int nData3);
void R_set_DAC_drcy80(int channel, int nData0, int nData1, int nData2, int nData3);
void R_set_DAC_drcy83(int channel,int DacChannel,int data);		//shin DRCY83 00/09/20
int R_set_DAC_drcy88(int channel,int DacChannel,int data);
int R_set_DAC_drcy101(int channel,int DacChannel,int data);
int	R_set_DAC_drcy140(int channel, int DacChannel, int data);	// 2014.08.25
int CheckDacDRCY83( int nChannel );
int InitialDacTLV5630(int nChannel);

int	R_gpdac_get_supply_voltage(int channel, int dac);
int	R_gpdac_get_force_voltage(int channel, int dac);
int	R_gpdac_get_temperature(int channel, int dac);

int	R_gpdac_make_own_dacs(int channel, int dacs);

// D/A�R���o�[�^�̃`�����l�������擾����B
// �J�����̂��ߖ��Ή���L��
// �߂�l	0�ȏ�	�`�����l�����BDAC�𓋍ڂ��Ă��Ȃ�I/O���0��Ԃ��B
//			-1		��Ȃ��A�܂��͖��Ή����
/*!
 * @brief R_gpdac_channels_of_dac@n
 *			channels of D/A converter
 * @param	channel of board (ex._BOARD_RDSW_0)
 * @retval	channels@n
 *			0	no dac channel
 *			0<	channels of dac
 *			<0	no board, or the board is not supported in this function.
 */
int	R_gpdac_channels_of_dac(int channel);

/*!
 * @brief R_gpdac_disable_bias_shifting@n
 *			disable bias-shifting for current zero
 * @param	channel of board (ex._BOARD_RDSW_0)
 * @retval	OK
 *			ERROR	no board or not DR-CY-80 board
 *	��d���o�͉�H��BIAS�V�t�g�@�\�𖳌��ɂ���B
 *	2011�N�ȑO�ɐ��삳�ꂽDR-CY-80A/92�̒��ɂ́AR_gpdac_set_current��0��ݒ肵�Ă�LED�����S�ɏ����Ȃ���������B
 *	�x�[�X���C�u����Ver.5.042�ȍ~�ł�BIAS�����p��D/A�R���o�[�^�𐧌䂵�āA0�ݒ莞�̘R��d����h���@�\���ǉ�����Ă���B
 *	������DAC�����30�`50us�����邽�߁A���̋@�\�𖳌��ɂ������ꍇ�͂��̊֐����g�p����B
 *	DR-CY-80A/92�����Â�DR-CY-80����ɂ�DAC�����ڂ���Ă��Ȃ�����BIAS�V�t�g�@�\�͓��삵�܂���B
 */
int	R_gpdac_disable_bias_shifting(int channel);

#define	R_set_DAC_drcy82		R_set_DAC_drcy64

#ifdef __cplusplus
}
#endif

#endif	// R_GPDAC_H_
