/******************************************************************************

	R_CS_PRO.H
	
	���Y�Ǘ��֐��w�b�_
	�o�[�W�����A�b�v���́A�K��Version[]�̕ύX�A�����̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	<version>	<date>		<name>		<comment>
	000			2004.04.07	sassa		�I���W�i��
	001			2004.06.13	sassa		PRODUCT_DATA�ɐ��Y�J�n���Ԃ�ǉ�
	002			2005.02.17	sassa		�O���[�o���ϐ������̃I�u�W�F�N�g�Ƃ��Ԃ�Ȃ��悤�ɁAstatic��+���O�ύX
	1.07b3.0	2010.11.16	sassa		R_CS_ProductCounterOpen�ǉ�

******************************************************************************/

// --- define ---
//���Y���\����
typedef struct{
	int			nProductNb;			//���Y��
	int			nRpmAverage;		//���ω�]��
	int			nRpmCurrent;		//���߂̉�]��
	int			nChangeTime;		//���b�g�؂�ւ�����
	R_TIME_INFO	tStartTime;			//���Y�J�n���ԏ��
	char		sLotNo[32];			//���b�g��
}R_CS_PRODUCT_DATA;

// --- variable ---

// --- prototype ---
int R_CS_ProductCounterOpen(R_CS_PRODUCT_DATA *pData);		// 2010.11.16 by sassa
int R_CS_ProductCounterClear(R_CS_PRODUCT_DATA *pData);
int R_CS_ProductCounterUp(R_CS_PRODUCT_DATA *pData);
int R_CS_ProductSetCycleNum(int number);


