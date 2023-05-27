/******************************************************************************
	R_SMBIOS.H / R_SMBIOS.C
	
	�ύX����
	Ver 3.44	2004/10/13	���������[�X
	Ver 3.45	2004/10/20	�����֐�GetString�o�O�t�B�N�X
	Ver 3.45	2005/02/15	�֐������ǉ�
	Ver 3.49	2006/03/13	�e�[�u���`�F�b�N���@�ύX
	Ver 3.50	2008/08/19	nBiosCharacteristics�ǉ�
	Ver 3.50	2009/04/07	DOS & LINUX �\�[�X����
	Ver 4.01	2009/09/16	x86_64 �\�[�X����
	Ver 4.01	2016/09/23	1MB�𒴂����̈�ɑΉ�
******************************************************************************/

#ifndef	R_SMBIOS_H_
#define	R_SMBIOS_H_

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bios_info_t {
	int		nLength;
	int		nVender;
	int		nBiosVersion;
	int		nBiosReleaseDate;
	uint64_t	nBiosCharacteristics;
	const char*	strVender;
	const char*	strBiosVersion;
	const char*	strBiosReleaseDate;
};

struct system_info_t {
	int		nLength;
	int		nManufacturer;
	int		nProductName;
	int		nVersion;
	int		nSerialNumber;
	const char*	strManufacturer;
	const char*	strProductName;
	const char*	strVersion;
	const char*	strSerialNumber;
};

struct base_board_info_t {
	int		nLength;
	int		nManufacturer;
	int		nProductName;
	int		nVersion;
	int		nSerialNumber;
	int		nAssetTag;
	const char*	strManufacturer;
	const char*	strProductName;
	const char*	strVersion;
	const char*	strSerialNumber;
	const char*	strAssetTag;
};

struct system_enclosure_t {
	int		nLength;
	int		nManufacturer;
	int		nVersion;
	int		nSerialNumber;
	int		nAssetTagNumber;
	const char*	strManufacturer;
	const char*	strVersion;
	const char*	strSerialNumber;
	const char*	strAssetTagNumber;
};

struct processor_info_t {
	int		nLength;
	int		nSocketDesignation;
	int		nProcessorFamily;
	int		nProcessorManufacturer;
	int		nProcessorVersion;
	int		nExternalClock;
	int		nMaxSpeed;
	int		nCurrentSpeed;
	int		nSerialNumber;
	int		nAssetTag;
	int		nPartNumber;
	const char*	strSocketDesignation;
	const char*	strProcessorManufacturer;
	const char*	strProcessorVersion;
	const char*	strSerialNumber;
	const char*	strAssetTag;
	const char*	strPartNumber;
};

struct smbios_t {
	int		nEntryPointLength;
	int		nMajorVersion;
	int		nMinorVersion;
	int		nMaximumStructureSize;
	int		nEntryPointVersion;
	int		nStructureTableLength;
	uint32_t	nStructureTableAddress;
	int		nNumberOfSmbiosStructures;
	int		nSmbiosBcdRevision;
	struct bios_info_t			BiosInfo;
	struct system_info_t		SystemInfo;
	struct base_board_info_t	BaseBoardInfo;
	struct system_enclosure_t	SystemEnclosure;
	struct processor_info_t		ProcessorInfo;
};

struct smbios_t*	R_smbios_struct(void);

void	R_smbios_init(void);
int		R_smbios_find(struct smbios_t*);

#ifdef __cplusplus
}
#endif

#endif	// R_SMBIOS_H_
