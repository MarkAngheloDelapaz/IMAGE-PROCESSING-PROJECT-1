/******************************************************************************

	R_CS_CMD.H
	
	IN-CY-84�R�}���h�ʐM�w�b�_
	�o�[�W�����A�b�v���́A�K�������̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		Date			Name	Comment
				2004.03.15.000
				2004.04.23.001			DrawPlane�`��֐�(R_draw�n)��g�ݍ���
				2004.05.11.002			�n���h�V�F�C�N���s���́A��M�����R�}���h��
										�S�Ĕj�����āA��蒼���悤�d�l�ύX
				2004.06.11.003			�o�C�i���̃X�e�[�^�X�\���s��C��
				2005.09.05.004			R_CS_CommandBusyOn,Off,ReadyOn,Off�ǉ�
				2009.05.08				R_CS_CommndSetGpioBoard(),
										R_CS_CommandSetWaitTime(),R_CS_CommandSetWaitTime()�̒ǉ�
	ver1.07b1.6	2009.05.27		saito	#define __CMD_GPIO__
										#define __CMD_DLIO__
										#define __CMD_SELECTABLE__
										�ɂ��؂蕪���p�~
										�ȍ~ __CMD_DLIO__ �̓�����f�t�H���g�Ƃ���
										�ؑւ�R_CS_SetCommunicationMode()�ɂĐݒ肷��
										int R_CS_InitialCommand(int nBoardType);	���p�~
										int R_CS_InitialCommand();					��������ɓ���
	1.07b4.4	2012.08.23				R_CS_CommandTransmit�Ń��[�U�֐��R�[���o�b�N�ǉ�
	1.07b4.8	2013.04.09				DP-RAM�ʐM�ǉ�
	1.07b4.8	2013.04.10				COMM_TIMEOUT��ϐ���
******************************************************************************/

// --- define ---
// ver1.07b1.6	2009.05.27 saito 	#define�ɂ��؂蕪���p�~
// 
//#define __CMD_GPIO__			// GPIO�n
//#define __CMD_DLIO__			// DLIO�n
//#define __CMD_SELECTABLE__		// GPIO/DLIO�\�t�g�E�F�A�؂�ւ�

#define COMMAND_BASE	0x20
#define COMMAND_MAX		0x80
//#define COMMAND_BUFF	256					// �σp�����[�^��  2007.11.16


// �r�b�g�ݒ�
#define RX_ENABLE		0x01				// <in > �R�}���h��M
#define RX_COMPLETE		0x01				// <out> �R�}���h��M�m�F
#define TX_ENABLE		0x02				// <out> �R�}���h���M
#define TX_COMPLETE		0x02				// <in > �R�}���h���M�m�F

#define CMD_EXT_IN1		0x01				// <in>  EXT IN 1   <RX_ENABLE>
#define CMD_EXT_IN2		0x02				// <in>  EXT IN 2   <TX_COMPLETE>
#define CMD_EXT_IN3		0x04				// <in>  EXT IN 3
#define CMD_EXT_IN4		0x08				// <in>  EXT IN 4
#define CMD_EXT_OUT1	0x01				// <out> EXT OUT 1  <RX_COMPLETE>
#define CMD_EXT_OUT2	0x02				// <out> EXT OUT 2  <TX_ENABLE>
#define CMD_EXT_OUT3	0x04				// <out> EXT OUT 3
#define CMD_EXT_OUT4	0x08				// <out> EXT OUT 4

#define CMD_BUSY		0x04				// <out> 9301BUSY
#define CMD_READY		0x08				// <out> 9301����������


typedef struct {
	char	*pCommand;				// �R�}���h��(����������)
	int		(*pFunction)(void);		// ���s�֐��ւ̃|�C���^
	int		nCode;					// �I�[�g����R�[�h
	int		nCommandLength;			// �R�}���h��(�����C�u�����Ŏg�p)
} FUNCTION_TABLE;


// --- variable ---
extern int COMMAND_BUFF;									// �σp�����[�^��  2007.11.16
extern char *R_CS_CommandBuffRx;							// �R�}���h��M�o�b�t�@  �σT�C�Y��  2007.11.16
extern char *R_CS_CommandBuffTx;							// �R�}���h���M�o�b�t�@  "
//extern char R_CS_CommandBuffRx[COMMAND_BUFF];							// �R�}���h��M�o�b�t�@
//extern char R_CS_CommandBuffTx[COMMAND_BUFF];							// �R�}���h���M�o�b�t�@

// --- prototype ---
//
// ver1.07b1.6	2009.05.27 saito 	�ʐM���@�̐ݒ�֐���R_CS_CMD.c/h��
int R_CS_CommandGetCommunicationBoardId( void );
int R_CS_CommandGetCommunicationMode( void );
int R_CS_CommandSetCommunicationMode( int nMode, int nBoardId );

// ver1.07b1.6	2009.05.27 saito 	#define�ɂ��؂蕪���p�~
//#ifdef __CMD_SELECTABLE__
//int R_CS_InitialCommand(int nBoardType);
//#else
int R_CS_InitialCommand(void);
//#endif
int R_CS_CommandBufferAllocate(int nSize);
int R_CS_CommandBufferClear(void);				// 2007.11.16
int R_CS_CommndSetGpioBoard(int nRdsw,int nDataPort,int nControlPort);		//<1.07b2.7>

int R_CS_CommandCheck(void);
//int R_CS_CommandExecute(void);
int R_CS_CommandTransmit(char *command);
int R_CS_CommandTxChar(char command);
//void R_CS_CommandIOCheck(void);
int R_CS_CommandBusyOn(void);
int R_CS_CommandBusyOff(void);
int R_CS_CommandReadyOn(void);
int R_CS_CommandReadyOff(void);
int R_CS_CommandWrite(int nPort, int nData);
int R_CS_CommandRead(int nPort);
int R_CS_CommandWriteExtOut(int nData);
int R_CS_CommandReadExtIn(void);
int R_CS_CommandExtOut3On(void);
int R_CS_CommandExtOut3Off(void);
int R_CS_CommandExtOut4On(void);
int R_CS_CommandExtOut4Off(void);

int R_CS_CommandDisplayStatusMode(int mode);
void R_CS_CommandDisplayStatus(void);

void R_CS_CommandSetWaitTime(int nTime);
int R_CS_CommandGetWaitTime(void);

// ver1.07b4.8	2013.04.10	�V�K�ǉ� �ʐM�^�C���A�E�g���Ԑݒ�
int	R_CS_CommandSetCommunicationTimeLimit(int usec);
int	R_CS_CommandGetCommunicationTimeLimit(void);
