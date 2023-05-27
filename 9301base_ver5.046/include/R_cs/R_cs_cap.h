/******************************************************************************

	R_cs_cap.h

	����
	<version>	<date>		<name>		<comment>
	0.01		2007.09.10	matsu		�V�K�쐬(R_cs.c���番�����J�����ݒ�֐��ǉ�)
	0.02		2007.09.27	sassa		�ȉ��̊֐���ǉ�(�f�o�b�O��)
										R_CS_CaptureMultiN()
										R_CS_CaptureMultiWaitN()
										R_CS_CaptureDisplayImage()
										R_CS_CaptureChangeDisplayId()
				2007.09.28	matsu		�ȉ��̊֐���ǉ�
										R_CS_CaptureSetGain()
										R_CS_CaptureGetGainLimit()
	1.07b1.4	2008.06.21	matsu		�ȉ��̊֐���ǉ�
											R_CS_CaptureSetInitiatorAddress()
											R_CS_CaptureGetInitiatorAddress()
	1.07b1.5	2009.05.22	saito		KPFR31PCL(VGA RAW�o�� COLOR CAMERA�j�Ή�
	1.07b1.9	2009.07.16	matsu		R_CS_CaptureGetCameraType()�ǉ�
	1.07b2.1	2009.08.10	araki		�t�@�C����R_CS_CAP.H��R_cs_cap.h�ɕύX
				2009.08.10	sassa		R_CS_CaptureCameraChange()�C��(���C�u����3.50�ȍ~��
										CO-CZ-43�����g�p���ɃX���[�\���ł��Ȃ��s��C��)
				2010.01.07	sassa		R_CS_Capture�̈���mode��R_CS_CameraCapture�ɓn���ۂɁACAPTURE_WAIT�ȊO��
										BIT��S�ēn���悤�ɏC��(����GV_NO_VRAM��GV_NO_WAIT�݂̂�n���Ă���)
	1.07b2.9	2010.11.02	oshima		R_CS_CaptureSetVideoInitMode(),R_CS_CaptureGetVideoInitMode()�̒ǉ�
										

******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

// --- define ---

extern R_CS_CAMERA 	*R_CS_CameraPara;

// --- prototype ---
int R_CS_CaptureOpen(void);
int R_CS_CaptureClose(void);

// �摜��荞�݊֐�
int R_CS_Capture(int camera_nb, int capture_nb, int mode);											// ��ʎ�荞��
int R_CS_CaptureWait(int camera_nb);																// ��ʎ�荞�ݏI���҂�
int R_CS_CaptureEndCheck(int camera_nb);															// �摜��荞�݊����I���m�F  �ǉ� 2008.01.08  by matsu
int R_CS_CaptureMultiN(int total_nb, int *camera_nb, int *capture_nb, int mode);					// ��ʎ�荞��(�J����������)
int R_CS_CaptureMultiWaitN(int total_nb, int *camera_nb);											// ��ʎ�荞�ݏI���҂�(�J����������)
#define R_CS_CaptureMulti(camera_nb, capture_nb, mode)		R_CS_CaptureMultiN(2, camera_nb, capture_nb, mode)
#define R_CS_CaptureMultiWait(camera_nb)					R_CS_CaptureMultiWaitN(2, camera_nb)

int R_CS_CaptureCameraChange(int camera_nb);														// ��ʎ�荞�݉摜�ؑ�
int R_CS_CaptureDisplayImage(int camera_nb, int capture_nb);										// ��荞�񂾉摜�����j�^�[�ɕ\��
int R_CS_CaptureChangeDisplayId(void);																// �摜�{�[�h��ID�����j�^�[�ɏo�͂��Ă���ԍ��ɐ؂�ւ���

// �J������{�ݒ�֐�
int R_CS_CaptureSetConfiguration(int nCameraNo, int nType, int nBoardId, int nChannel);				// �ڑ��󋵂̐ݒ�
int R_CS_CaptureSetCondition(int nCameraNo, int nNum, int *pShutterSpeed, int *pInitiatorPage);		// ��荞�ݏ����̐ݒ�

// �J�����g���ݒ�֐�
int R_CS_CaptureSetSize(int nCameraNo, int nSize);													// ��荞�݃T�C�Y�̐ݒ�
int R_CS_CaptureSetMode(int nCameraNo, int nMode);													// �J�������[�h�̐ݒ�
int R_CS_CaptureSetSequence(int nCameraNo, int nSequence);											// ��荞�݃��[�h�̐ݒ�
int R_CS_CaptureSetInitiatorMode(int nCameraNo, int nInitiator);									// �C�j�V�G�[�^���[�h�̐ݒ�
int R_CS_CaptureSetPlane(int nCameraNo, int nPlane);												// ��荞�݃v���[���̐ݒ�
int R_CS_CaptureSetBinLevel(int nCameraNo, int nBinLevel);											// ��荞�݂Q�l���x���̐ݒ�

int R_CS_CaptureSetGain(int nCameraNo, int nGain);													// �J�����Q�C���̐ݒ�  2007.09.28  by matsu
int R_CS_CaptureGetGainLimit(int nCameraNo, int *pGainMin, int *pGainMax);							// �J�����Q�C���̎擾  2007.09.28  by matsu

int R_CS_CaptureSetInitiatorAddress(int nCameraNo, int nCaptureNo, unsigned char *pAddress);		//<1.07b1.4> �C�j�V�G�[�^��A�h���X�̎w��  2008.06.21  by matsu
unsigned char *R_CS_CaptureGetInitiatorAddress(int nCameraNo, int nCaptureNo);						//<1.07b1.4> �C�j�V�G�[�^��A�h���X�̎擾  2008.06.21  by matsu

int R_CS_CaptureGetCameraType(int nCameraNo);														//<1.07b1.9>  �J�����^�C�v�̎擾

int R_CS_CaptureSetVideoInitMode(int nCameraNo,int nMode);											//�{�[�h�������ݒ�
int R_CS_CaptureGetVideoInitMode(int nCameraNo);													//�{�[�h�������ݒ�̎擾

int	R_CS_CaptureSetVideoInitShutterMode(int nCameraNo, int mode);
int	R_CS_CaptureGetVideoInitShutterMode(int nCameraNo);

int	R_CS_CaptureSetVideoInitOtherMode(int nCameraNo, int mode);

// ���d�l
int R_CS_SetCameraPara(int camera_type,int video_size);						// �J�����̐ݒ�(�S�J��������)
int R_CS_CaptureSetCameraNumber(int nNum);									// �J�����䐔�̐ݒ�

#ifdef __cplusplus
}
#endif
