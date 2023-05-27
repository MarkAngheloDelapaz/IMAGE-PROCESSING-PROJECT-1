#ifndef R_CS_LAN_H
#define R_CS_LAN_H

#ifdef __cplusplus
extern "C"
{
#endif
// --- define ---
#define	COMMUNICATION_COMMAND_NO_READY				0x00			// �R�}���h�Ȃ�(����M)
#define	COMMUNICATION_COMMAND_LOT_START				0x01			// ���b�g�X�^�[�g�@�R�}���h
#define	COMMUNICATION_COMMAND_LOT_END				0x02			// ���b�g�G���h�@�R�}���h
#define	COMMUNICATION_COMMAND_DATA_REFLESH			0x03			// �f�[�^�X�V�v��
#define	COMMUNICATION_COMMAND_DATA_RECEIVED			0x04			// �f�[�^��M����
#define	COMMUNICATION_COMMAND_RESULT_REQUEST		0x05			// Result Request
#define	COMMUNICATION_COMMAND_USER					0x06			// ���[�U�J�X�^���R�}���h

#define COMMUNICATION_STATUS_ACK		0x00		// �X�e�[�^�X�@ACK		2013/05/30 by sassa
#define COMMUNICATION_STATUS_NACK		0x01		// �X�e�[�^�X�@NACK		2013/05/30 by sassa

#define R_CS_LAN_MAX_SUBSTRING		8
#define R_CS_LAN_MAX_TYPELEN		128
#define R_CS_LAN_MAX_IOBYTE			8				// IO�|�[�g��(�o�C�g��)
#define	R_CS_LEN_MAX_MARK			32				// �W�󕶎���(�o�C�g��)

#define R_CS_LAN_COUNTERCLEAR_TYPE1		1			///<	���b�g�X�^�[�g�O�ɃJ�E���^�N���A
#define R_CS_LAN_COUNTERCLEAR_TYPE2		2			///<	���b�g�G���h��ɃJ�E���^�N���A


typedef struct
{
	int						nLanEnable;									// LAN�ʐM���I�[�v�����邩(0:OFF,1:ON)
	int						nIsServer;									// �T�[�o�ŃI�[�v�����邩�N���C�A���g�ŃI�[�v�����邩�H
	int						nServerPort;								// �T�[�o�[�̃|�[�g�ԍ�
	int						nInterval;									// �|�[�����O�C���^�[�o��
	int						nReconnectTimer;							// �Đڑ����̃^�C�}
	char					sServerIpAddress[32];						// �T�[�o�[��IP�A�h���X
	char					sClientLocation[32];						// �N���C�A���g�̃��P�[�V������
	int						nAutoSend;									// �ڑ��������ŏ�����Ԃ𑗂�

	int						nTypeStart[R_CS_LAN_MAX_SUBSTRING];			// ��QR�R�[�h����̐؂�o���X�^�[�g
	int						nTypeLen[R_CS_LAN_MAX_SUBSTRING];			// ��QR�R�[�h����̐؂�o������

	int						nMarkStart;									// ��QR�R�[�h����̐؂�o���X�^�[�g
	int						nMarkLen;									// ��QR�R�[�h����̐؂�o������

	int						nTrimSpace;

	int						nAutoCounterClear;							// QR��M���I�[�g�J�E���^�N���A

	int						nKeepAliveEnable;							// �\�P�b�g�ʐM��KeepAlive�@�\���g�p���邩�ǂ���
} R_CS_LANINFO;

typedef struct{
	void *comstruct;
} R_CS_LAN_HANDLE;

// --- variable ---

// --- prototype ---
R_CS_LAN_HANDLE R_CS_Lan_Open(char *sIpAddress, int nPort, char *sLocation);		// ���b�g�f�[�^�ʐM���W���[���̃I�[�v��(�̈�m��)
R_CS_LAN_HANDLE R_CS_Lan_OpenServer(char *sIpAddress, int nPort, char *sLocation);	// ���b�g�f�[�^�ʐM���W���[�����T�[�o�[�ŃI�[�v��(�̈�m��)
int R_CS_Lan_Close(R_CS_LAN_HANDLE handle);											// ���b�g�f�[�^�ʐM���W���[���̃N���[�Y(�̈���)
int R_CS_Lan_Connect(R_CS_LAN_HANDLE handle, int nTimer);							// ���b�g�f�[�^�ʐM�̐ڑ�
int R_CS_Lan_Connect_NoMsg(R_CS_LAN_HANDLE handle, int nTimer);						// ���b�g�f�[�^�ʐM�̐ڑ�(���b�Z�[�W�\���Ȃ�)
int R_CS_Lan_Disconnect(R_CS_LAN_HANDLE handle);									// ���b�g�f�[�^�ʐM�̐ؒf
int R_CS_Lan_ReceiveLotInfo(R_CS_LAN_HANDLE handle, const R_CS_LANINFO *info);		// ���b�g�f�[�^��M
int R_CS_Lan_SendLotInfo(R_CS_LAN_HANDLE handle, int nStatus);						// ���b�g�f�[�^���M
int R_CS_Lan_SendDataRefleshRequest(R_CS_LAN_HANDLE handle);
int R_CS_Lan_SendDataRefleshReceived(R_CS_LAN_HANDLE handle);
int R_CS_Lan_Receive(R_CS_LAN_HANDLE handle);										// �f�[�^��M
int R_CS_Lan_SetKeepAlive(R_CS_LAN_HANDLE handle, int idle, int intvl, int cnt);
char* R_CS_Lan_GetQr(R_CS_LAN_HANDLE handle);										// QR�R�[�h�@������̎擾
char* R_CS_Lan_GetLotNo(R_CS_LAN_HANDLE handle);									// LotNo     ������̎擾
char* R_CS_Lan_GetLocation(R_CS_LAN_HANDLE handle);									// Location  ������̎擾
char* R_CS_Lan_GetType(R_CS_LAN_HANDLE handle);										// Type      ������̎擾
char* R_CS_Lan_GetCommand(R_CS_LAN_HANDLE handle);									// Command   ������̎擾
char* R_CS_Lan_GetMarkChar(R_CS_LAN_HANDLE handle);

void R_CS_LAN_SetDataLen(R_CS_LAN_HANDLE handle, int len);
//void R_CS_LAN_SetDataLen(int len);

int R_CS_LAN_GetIO(R_CS_LAN_HANDLE handle, int channel);
int R_CS_LAN_GetOutputStatus(R_CS_LAN_HANDLE handle);
//int R_CS_LAN_GetIO(int channel);

void R_CS_LAN_IO_Off(R_CS_LAN_HANDLE handle, int channel, int bit);
void R_CS_LAN_IO_On(R_CS_LAN_HANDLE handle, int channel, int bit);
void R_CS_LAN_IO_SetAutoReflesh(int flag);

/**
 * @brief R_CS_LAN_SetUserCommand
 * @param command
 */
void R_CS_LAN_SetUserCommand(R_CS_LAN_HANDLE handle, const char* command, int (*pFunc)(char *msg));

#ifdef __cplusplus
}
#endif
#endif // R_CS_LAN_H
