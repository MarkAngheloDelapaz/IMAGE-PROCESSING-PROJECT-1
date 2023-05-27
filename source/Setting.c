#include "R_stdlib.h"
#include "R_video.h"
#include "R_trans.h"
#include "R_filter.h"
#include "R_dgraph.h"
#include "R_bgray.h"
#include "R_cs.h"
#include "R_cdraw.h"
#include "tmcr00i.h"
#include "tp_sub.h"
#include "mcr03.h"
#include "mcr10.h"
#include "mcr18.h"
#include "esr25.h"
#include "ltr50.h"
#include "ltr18.h"															//v2.20

extern  int             G2_pos[4];											//Ｇ２位置検出データ( lex, rix, upy, loy )
extern  int             Ap_pos[6];											//外形位置検出データ( lex, rix, leuy, lely, riuy, rily )
extern  int             ElecPosRough[3][4];									// 電極位置検出データ[side][pos]
extern KAIKI		ue, shita, migi, hidari, w, l, Ap_up, Ap_lo, Ap_ri, Ap_le, Ap_w, Ap_l;
extern MEANSD		up_y, lo_y, le_x, ri_x, Ap_up_y, Ap_lo_y, Ap_le_x, Ap_ri_x;
extern int g2level;
extern int bIsNitto;
extern int Index_pos[];													//インデックステーブル　エッジ位置( uy, dy, ly )

// --- Gray to Bin level ---
extern BIN_LEVEL_PARAMETER	G2EsLevel;

int SetInspSearchIndex(int mode)
{
	int res = 0;
	R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
	R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

	int insp_mode = R_CS_GetInspectMode();
	R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

	// --- Inspection
	InspBinPn	= BinP1;

	//インデックステーブルエッジ検出
	Index_pos[0]=0       +20;
	Index_pos[1]=FySize-1-20;
	IGuidePos[DataX]=600;	//FxSize-1;			// Nitto
	IGuidePos[1]    =0;					// Weld
	TWA_Index_search();

	R_CS_SetInspectMode(insp_mode);

	return res;
}

int SetInspSearchGuide(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
    R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

    int insp_mode = R_CS_GetInspectMode();
    R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

    // Index guide 位置サーチ
    InspBinPn	= BinP1;
    TP_SearchGuide();

    R_CS_SetInspectMode(insp_mode);

    return res;
}


int SetInspTP_IndexTableBright(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
    R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // Index guide 位置サーチ
    InspBinPn	= BinP1;
	if(!bIsNitto){
		TP_SearchGuide();
	} else {
		IGuidePos[DataX]=FxSize-1;
	}

    R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

    int insp_mode = R_CS_GetInspectMode();
    R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

    // Index table level
    TP_IndexTableBright();

    R_CS_SetInspectMode(insp_mode);

    return res;
}

int SetInspTPCheckBright(int mode)
{
	int res = 0;
	R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
	R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	// Index guide 位置サーチ
	InspBinPn	= BinP1;
	if(!bIsNitto){
		TP_SearchGuide();
	} else {
		IGuidePos[DataX]=FxSize-1;
	}

	R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

	int insp_mode = R_CS_GetInspectMode();
	R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

	// Index table level
	TP_CheckBright();
	R_CS_SetInspectMode(insp_mode);

	return res;
}

int SetInspSearchElect(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
    R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

    // Index guide 位置サーチ
    InspBinPn	= BinP1;
	if(!bIsNitto){
		TP_SearchGuide();
	} else {
		IGuidePos[DataX]=FxSize-1;
	}
	// Index table level
    TP_IndexTableBright();

    int insp_mode = R_CS_GetInspectMode();
    R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

    // 電極位置検出
    InspBinPn	= BinP1;
    TP_SearchElect();

    R_CS_SetInspectMode(insp_mode);

    return res;
}

int SetInspTWA_G2_ES_search(int mode)
{
	int res = 0;
	R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
	R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

	// Index guide 位置サーチ
	InspBinPn	= BinP1;
	if(!bIsNitto){
		TP_SearchGuide();
	} else {
		IGuidePos[DataX]=FxSize-1;
	}
	// Index table level
	TP_IndexTableBright();

	// 電極位置検出
	TP_SearchElect();
	//v2.20	if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		//NORMAL
		G2_pos[0]	= ElecPosRough[DataLeft][2];
		G2_pos[1]	= ElecPosRough[DataRight][0];
		G2_pos[2]   = (ElecPosRough[DataLeft][PosYmin]+ElecPosRough[DataRight][PosYmin])/2;
		G2_pos[3]   = (ElecPosRough[DataLeft][PosYmax]+ElecPosRough[DataRight][PosYmax])/2;

		Ap_pos[0]	= ElecPosRough[DataLR][PosXmin];
		Ap_pos[1]	= ElecPosRough[DataLR][PosXmax];
		Ap_pos[2]	= ElecPosRough[DataLeft][PosYmin];
		Ap_pos[3]	= ElecPosRough[DataLeft][PosYmax];
		Ap_pos[4]	= ElecPosRough[DataRight][PosYmin];
		Ap_pos[5]	= ElecPosRough[DataRight][PosYmax];
	} else {
		// LTR
		G2_pos[0]	= (ElecPosRough[DataUpper][PosXmin]+ElecPosRough[DataLower][PosXmin])/2;
		G2_pos[1]	= (ElecPosRough[DataUpper][PosXmax]+ElecPosRough[DataLower][PosXmax])/2;
		G2_pos[2]   =  ElecPosRough[DataUpper][PosYmax];
		G2_pos[3]   =  ElecPosRough[DataLower][PosYmin];

		Ap_pos[0]	= ElecPosRough[DataUpper][PosXmin];
		Ap_pos[1]	= ElecPosRough[DataUpper][PosXmax];
		Ap_pos[2]	= ElecPosRough[DataLower][PosXmin];
		Ap_pos[3]	= ElecPosRough[DataLower][PosXmax];
		Ap_pos[4]	= ElecPosRough[DataUL][PosYmin];
		Ap_pos[5]	= ElecPosRough[DataUL][PosYmax];
	}

	int insp_mode = R_CS_GetInspectMode();
	R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

	InspBinPn	= BinP2;
	// v2.20	if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		G2EsLevelSet(0);
		g2level = G2EsLevel.nData;
//		g2level = GetBinG2Rough(InspGrayPn);
//		g2level = 80;
		BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
		R_gray2bin( g2level, 0, 0, IGuidePos[DataX]+1, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	} else {
		G2EsLevelSet(0);
		g2level = G2EsLevel.nData;
		BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
		R_gray2bin( g2level, 0, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
		G2EsLevelSet(1);
		g2level = G2EsLevel.nData;
		R_gray2bin( g2level, FxSize/2, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	}

	//Ｇ２　エッジ検出
	if(!bIsLTR50){	// normal (MCR,ESR,SFR)
		if( TWA_G2_ES_search( 0 ) != OK ){
		//	pCsResult->nCode = NG_CANT_INSP;
			return ERROR;
		}
	} else {		// LTR
		if(bIsLTR18){									//v2.20
			if( TWA_G2_ES_searchLTR_l18( 0 ) != OK ){
				return ERROR;
			}
		}else{
			if( TWA_G2_ES_searchLTR( 0 ) != OK ){
			//	pCsResult->nCode = NG_CANT_INSP;
				return ERROR;
			}
		}
	}

	//回帰係数計算
	KAIKI_keisuu(0);

	//平均値・標準偏差
	TWA_meansd();

	//回帰係数計算
	KAIKI_keisuu(1);

	// G2 std. edge position set
//		if( G2StdEdgeSet() != OK ) return( NG_CANT_INSP );

	G2_pos[0] = hidari.kaiki_ex;
	G2_pos[1] = migi.kaiki_ex;
	G2_pos[2] = ue.kaiki_ey;
	G2_pos[3] = shita.kaiki_ey;

	printf("G2_pos %d,%d,%d,%d\n",G2_pos[0],G2_pos[1],G2_pos[2],G2_pos[3]);
	R_CS_SetInspectMode(insp_mode);

	return res;
}



int SetInspChipPos(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
    R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

    // Index guide 位置サーチ
    InspBinPn	= BinP1;
	if(!bIsNitto){
		TP_SearchGuide();
	} else {
		IGuidePos[DataX]=FxSize-1;
	}
	// Index table level
    TP_IndexTableBright();
    // 電極位置検出
    InspBinPn	= BinP1;
    TP_SearchElect();

	//v2.20	if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		//NORMAL
		G2_pos[0]	= ElecPosRough[DataLeft][2];
		G2_pos[1]	= ElecPosRough[DataRight][0];
		G2_pos[2]   = (ElecPosRough[DataLeft][PosYmin]+ElecPosRough[DataRight][PosYmin])/2;
		G2_pos[3]   = (ElecPosRough[DataLeft][PosYmax]+ElecPosRough[DataRight][PosYmax])/2;

		Ap_pos[0]	= ElecPosRough[DataLR][PosXmin];
		Ap_pos[1]	= ElecPosRough[DataLR][PosXmax];
		Ap_pos[2]	= ElecPosRough[DataLeft][PosYmin];
		Ap_pos[3]	= ElecPosRough[DataLeft][PosYmax];
		Ap_pos[4]	= ElecPosRough[DataRight][PosYmin];
		Ap_pos[5]	= ElecPosRough[DataRight][PosYmax];
	} else {
		// LTR
		G2_pos[0]	= (ElecPosRough[DataUpper][PosXmin]+ElecPosRough[DataLower][PosXmin])/2;
		G2_pos[1]	= (ElecPosRough[DataUpper][PosXmax]+ElecPosRough[DataLower][PosXmax])/2;
		G2_pos[2]   =  ElecPosRough[DataUpper][PosYmax];
		G2_pos[3]   =  ElecPosRough[DataLower][PosYmin];

		Ap_pos[0]	= ElecPosRough[DataUpper][PosXmin];
		Ap_pos[1]	= ElecPosRough[DataUpper][PosXmax];
		Ap_pos[2]	= ElecPosRough[DataLower][PosXmin];
		Ap_pos[3]	= ElecPosRough[DataLower][PosXmax];
		Ap_pos[4]	= ElecPosRough[DataUL][PosYmin];
		Ap_pos[5]	= ElecPosRough[DataUL][PosYmax];
	}

	InspBinPn	= BinP2;
	//v2.20	if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		G2EsLevelSet(0);
		g2level = G2EsLevel.nData;
//		g2level = GetBinG2Rough(InspGrayPn);
//		g2level = 80;
		BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
		R_gray2bin( g2level, 0, 0, IGuidePos[DataX]+1, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	} else {
		G2EsLevelSet(0);
		g2level = G2EsLevel.nData;
		BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
		R_gray2bin( g2level, 0, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
		G2EsLevelSet(1);
		g2level = G2EsLevel.nData;
		R_gray2bin( g2level, FxSize/2, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	}

	//Ｇ２　エッジ検出
	if(!bIsLTR50){	// normal (MCR,ESR,SFR)
		if( TWA_G2_ES_search( 0 ) != OK ){
		//	pCsResult->nCode = NG_CANT_INSP;
			return ERROR;
		}
	} else {		// LTR
		if(bIsLTR18){									//v2.20
			if( TWA_G2_ES_searchLTR_l18( 0 ) != OK ){
				return ERROR;
			}
		}else{
			if( TWA_G2_ES_searchLTR( 0 ) != OK ){
			//	pCsResult->nCode = NG_CANT_INSP;
				return ERROR;
			}
		}
	}

	//回帰係数計算
	KAIKI_keisuu(0);

	//平均値・標準偏差
	TWA_meansd();

	//回帰係数計算
	KAIKI_keisuu(1);

	// G2 std. edge position set
//		if( G2StdEdgeSet() != OK ) return( NG_CANT_INSP );

	G2_pos[0] = hidari.kaiki_ex;
	G2_pos[1] = migi.kaiki_ex;
	G2_pos[2] = ue.kaiki_ey;
	G2_pos[3] = shita.kaiki_ey;

	int insp_mode = R_CS_GetInspectMode();
	R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

	if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25){	//xxxxx
		InspBinPn	= BinP2;
		TWA_Insp_Ap(0);
	} else if( bIsLTR50){
		InspBinPn	= BinP2;
		TWA_Insp_ApLTR(0);
	} else if( bIsLTR18){								//v2.20
		InspBinPn	= BinP2;
		TWA_Insp_ApLTR_l18(0);
	}
	// チップ位置検出
    InspBinPn	= BinP1;
    SearchChipPos();

    R_CS_SetInspectMode(insp_mode);

    return res;
}

int SetInspReverse(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
	R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

	// Index guide 位置サーチ
	InspBinPn	= BinP1;
	if(!bIsNitto){
		TP_SearchGuide();
	} else {
		IGuidePos[DataX]=FxSize-1;
	}
	// Index table level
	TP_IndexTableBright();
	// 電極位置検出
	InspBinPn	= BinP1;
	TP_SearchElect();

	//v2.20	if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		//NORMAL
		G2_pos[0]	= ElecPosRough[DataLeft][2];
		G2_pos[1]	= ElecPosRough[DataRight][0];
		G2_pos[2]   = (ElecPosRough[DataLeft][PosYmin]+ElecPosRough[DataRight][PosYmin])/2;
		G2_pos[3]   = (ElecPosRough[DataLeft][PosYmax]+ElecPosRough[DataRight][PosYmax])/2;

		Ap_pos[0]	= ElecPosRough[DataLR][PosXmin];
		Ap_pos[1]	= ElecPosRough[DataLR][PosXmax];
		Ap_pos[2]	= ElecPosRough[DataLeft][PosYmin];
		Ap_pos[3]	= ElecPosRough[DataLeft][PosYmax];
		Ap_pos[4]	= ElecPosRough[DataRight][PosYmin];
		Ap_pos[5]	= ElecPosRough[DataRight][PosYmax];
	} else {
		// LTR
		G2_pos[0]	= (ElecPosRough[DataUpper][PosXmin]+ElecPosRough[DataLower][PosXmin])/2;
		G2_pos[1]	= (ElecPosRough[DataUpper][PosXmax]+ElecPosRough[DataLower][PosXmax])/2;
		G2_pos[2]   =  ElecPosRough[DataUpper][PosYmax];
		G2_pos[3]   =  ElecPosRough[DataLower][PosYmin];

		Ap_pos[0]	= ElecPosRough[DataUpper][PosXmin];
		Ap_pos[1]	= ElecPosRough[DataUpper][PosXmax];
		Ap_pos[2]	= ElecPosRough[DataLower][PosXmin];
		Ap_pos[3]	= ElecPosRough[DataLower][PosXmax];
		Ap_pos[4]	= ElecPosRough[DataUL][PosYmin];
		Ap_pos[5]	= ElecPosRough[DataUL][PosYmax];
	}

	InspBinPn	= BinP2;
	//v2.20	if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		G2EsLevelSet(0);
		g2level = G2EsLevel.nData;
//		g2level = GetBinG2Rough(InspGrayPn);
//		g2level = 80;
		BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
		R_gray2bin( g2level, 0, 0, IGuidePos[DataX]+1, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	} else {
		G2EsLevelSet(0);
		g2level = G2EsLevel.nData;
		BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
		R_gray2bin( g2level, 0, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
		G2EsLevelSet(1);
		g2level = G2EsLevel.nData;
		R_gray2bin( g2level, FxSize/2, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	}

	//Ｇ２　エッジ検出
	if(!bIsLTR50){	// normal (MCR,ESR,SFR)
		if( TWA_G2_ES_search( 0 ) != OK ){
		//	pCsResult->nCode = NG_CANT_INSP;
			return ERROR;
		}
	} else {		// LTR
		if(bIsLTR18){									//v2.20
			if( TWA_G2_ES_searchLTR_l18( 0 ) != OK ){
				return ERROR;
			}
		}else{
			if( TWA_G2_ES_searchLTR( 0 ) != OK ){
			//	pCsResult->nCode = NG_CANT_INSP;
				return ERROR;
			}
		}
	}

	//回帰係数計算
	KAIKI_keisuu(0);

	//平均値・標準偏差
	TWA_meansd();

	//回帰係数計算
	KAIKI_keisuu(1);

	// G2 std. edge position set
//		if( G2StdEdgeSet() != OK ) return( NG_CANT_INSP );

	G2_pos[0] = hidari.kaiki_ex;
	G2_pos[1] = migi.kaiki_ex;
	G2_pos[2] = ue.kaiki_ey;
	G2_pos[3] = shita.kaiki_ey;

	if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25){	//xxxxx
		InspBinPn	= BinP2;
		TWA_Insp_Ap(0);
	} else if( bIsLTR50){
		InspBinPn	= BinP2;
		TWA_Insp_ApLTR(0);
	} else if( bIsLTR18){								//v2.20
		InspBinPn	= BinP2;
		TWA_Insp_ApLTR_l18(0);
	}
	// チップ位置検出
	InspBinPn	= BinP1;
	SearchChipPos();


	int insp_mode = R_CS_GetInspectMode();
	R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

	// 表裏確認
	InspReverse();
	R_CS_SetInspectMode(insp_mode);


    return res;
}


int SetInspChipSize(int mode)
{
	int res = 0;
	R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
	R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

	// Index guide 位置サーチ
	InspBinPn	= BinP1;
	if(!bIsNitto){
		TP_SearchGuide();
	} else {
		IGuidePos[DataX]=FxSize-1;
	}
	// Index table level
	TP_IndexTableBright();
	// 電極位置検出
	InspBinPn	= BinP1;
	TP_SearchElect();

	// チップ位置検出
	InspBinPn	= BinP1;
	SearchChipPos();

	//v2.20	if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		//NORMAL
		G2_pos[0]	= ElecPosRough[DataLeft][2];
		G2_pos[1]	= ElecPosRough[DataRight][0];
		G2_pos[2]   = (ElecPosRough[DataLeft][PosYmin]+ElecPosRough[DataRight][PosYmin])/2;
		G2_pos[3]   = (ElecPosRough[DataLeft][PosYmax]+ElecPosRough[DataRight][PosYmax])/2;

		Ap_pos[0]	= ElecPosRough[DataLR][PosXmin];
		Ap_pos[1]	= ElecPosRough[DataLR][PosXmax];
		Ap_pos[2]	= ElecPosRough[DataLeft][PosYmin];
		Ap_pos[3]	= ElecPosRough[DataLeft][PosYmax];
		Ap_pos[4]	= ElecPosRough[DataRight][PosYmin];
		Ap_pos[5]	= ElecPosRough[DataRight][PosYmax];
	} else {
		// LTR
		G2_pos[0]	= (ElecPosRough[DataUpper][PosXmin]+ElecPosRough[DataLower][PosXmin])/2;
		G2_pos[1]	= (ElecPosRough[DataUpper][PosXmax]+ElecPosRough[DataLower][PosXmax])/2;
		G2_pos[2]   =  ElecPosRough[DataUpper][PosYmax];
		G2_pos[3]   =  ElecPosRough[DataLower][PosYmin];

		Ap_pos[0]	= ElecPosRough[DataUpper][PosXmin];
		Ap_pos[1]	= ElecPosRough[DataUpper][PosXmax];
		Ap_pos[2]	= ElecPosRough[DataLower][PosXmin];
		Ap_pos[3]	= ElecPosRough[DataLower][PosXmax];
		Ap_pos[4]	= ElecPosRough[DataUL][PosYmin];
		Ap_pos[5]	= ElecPosRough[DataUL][PosYmax];
	}

	InspBinPn	= BinP2;
	//v2.20	if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		G2EsLevelSet(0);
		g2level = G2EsLevel.nData;
//		g2level = GetBinG2Rough(InspGrayPn);
//		g2level = 80;
		BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
		R_gray2bin( g2level, 0, 0, IGuidePos[DataX]+1, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	} else {
		G2EsLevelSet(0);
		g2level = G2EsLevel.nData;
		BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
		R_gray2bin( g2level, 0, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
		G2EsLevelSet(1);
		g2level = G2EsLevel.nData;
		R_gray2bin( g2level, FxSize/2, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	}

	//Ｇ２　エッジ検出
	if(!bIsLTR50){	// normal (MCR,ESR,SFR)
		if( TWA_G2_ES_search( 0 ) != OK ){
		//	pCsResult->nCode = NG_CANT_INSP;
			return ERROR;
		}
	} else {		// LTR
		if(bIsLTR18){									//v2.20
			if( TWA_G2_ES_searchLTR_l18( 0 ) != OK ){
				return ERROR;
			}
		}else{
			if( TWA_G2_ES_searchLTR( 0 ) != OK ){
			//	pCsResult->nCode = NG_CANT_INSP;
				return ERROR;
			}
		}
	}

	//回帰係数計算
	KAIKI_keisuu(0);

	//平均値・標準偏差
	TWA_meansd();

	//回帰係数計算
	KAIKI_keisuu(1);

	// G2 std. edge position set
//		if( G2StdEdgeSet() != OK ) return( NG_CANT_INSP );

	G2_pos[0] = hidari.kaiki_ex;
	G2_pos[1] = migi.kaiki_ex;
	G2_pos[2] = ue.kaiki_ey;
	G2_pos[3] = shita.kaiki_ey;

	if(bIsESR25 || bIsMCR10 || bIsMCR18){	//xxxxx
		InspBinPn	= BinP2;
		TWA_Insp_Ap(0);
	} else if( bIsLTR50){
		InspBinPn	= BinP2;
		TWA_Insp_ApLTR(0);
	} else if( bIsLTR18){					//v2.20
		InspBinPn	= BinP2;
		TWA_Insp_ApLTR_l18(0);
	}
	// チップ位置検出
	InspBinPn	= BinP1;
	SearchChipPos();

	// 表裏確認
	InspReverse();


        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        // Inspection chip size
        InspChipSize();
        R_CS_SetInspectMode(insp_mode);


    return res;
}


int SetInspElectrode(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
        R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // --- 検査開始 ---
//        ProcessTimerStart();

        res = Inspect( &pResult );

    // --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

        R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        // Inspection Electrode( a foreign body [substance].)
        InspBinPn	= BinP1;
        res = InspElectrode();
        R_CS_SetInspectMode(insp_mode);


    return res;
}
int SetInspElecKasure(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
        R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // --- 検査開始 ---
//        ProcessTimerStart();

        res = Inspect( &pResult );

    // --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

        R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        // Inspection Electrode( a foreign body [substance].)
        InspBinPn	= BinP1;
        res = InspElecKasure();
        R_CS_SetInspectMode(insp_mode);


    return res;
}

int SetInspMark(int mode)
{
	int res = 0;
	R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
		R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	// --- 検査開始 ---
//        ProcessTimerStart();

		res = Inspect( &pResult );

	// --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

		G2_pos[2] = ue.kaiki_ey;
		G2_pos[3] = shita.kaiki_ey;

		int insp_mode = R_CS_GetInspectMode();
		R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

		// Inspection Electrode( a foreign body [substance].)
		InspBinPn	= BinP1;
		if(bIsMCR03!=0){
			res = TWA_kensa_m03();
		} else if(bIsMCR10!=0){
			res = TWA_kensa_m10();
		} else if(bIsMCR18!=0){
			res = TWA_kensa_m18();
		} else if(bIsESR25!=0 || bIsSFR25!=0){
			res = TWA_kensa_me25();
		} else if(bIsLTR50!=0){
			res = TWA_kensa_l50();
		} else if(bIsLTR18!=0){
			res = TWA_kensa_l18();									//v2.20
		}
		R_CS_SetInspectMode(insp_mode);


	return res;
}


int SetInspOverCoat(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
        R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // --- 検査開始 ---
//        ProcessTimerStart();

        res = Inspect( &pResult );

    // --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

        R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        InspBinPn	= BinP2;
        res = InspOvercoat();
        R_CS_SetInspectMode(insp_mode);


    return res;
}

int SetInspPlating(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
        R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // --- 検査開始 ---
//        ProcessTimerStart();

        res = Inspect( &pResult );

    // --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

        R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        // Inspection Plating ( CB Side )
        InspBinPn	= BinP3;
        res = InspPlating();
        R_CS_SetInspectMode(insp_mode);


    return res;
}


int SetInspPinhole(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
        R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // --- 検査開始 ---
//        ProcessTimerStart();

        res = Inspect( &pResult );

    // --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

        R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        //Pinhole insp.
        Pinhole_insp();
        R_CS_SetInspectMode(insp_mode);


    return res;
}

int SetInspC2Xsize(int mode)
{
	int res = 0;
	R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
		R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	// --- 検査開始 ---
//        ProcessTimerStart();

		res = Inspect( &pResult );

	// --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

		int insp_mode = R_CS_GetInspectMode();
		R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

		// Inspection Plating ( CB Side )
		InspBinPn	= BinP3;
		if(!bIsLTR50){	//normal
			res = TWA_InspC2Xsize();
		} else {
			if(bIsLTR18){									//v.2.20
				res = TWA_InspC2Ysize_l18();
			}else{
				res = TWA_InspC2Ysize();
			}
		}
		R_CS_SetInspectMode(insp_mode);


	return res;
}

int SetInspMezuArea(int mode)
{
	int res = 0;
	R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
		R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	// --- 検査開始 ---
//        ProcessTimerStart();

		res = Inspect( &pResult );

	// --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

		int insp_mode = R_CS_GetInspectMode();
		R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

		// Inspection Plating ( CB Side )
		InspBinPn	= BinP3;
		res = TWA_Mezu_Area();

		R_CS_SetInspectMode(insp_mode);


	return res;
}

int SetInspSDl(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
        R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // --- 検査開始 ---
//        ProcessTimerStart();

        res = Inspect( &pResult );

    // --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

        R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        // Inspection Plating ( CB Side )
        InspBinPn	= BinP2;

    //Ｇ２エッジデータ判定　Ｗ方向
        res = G2_Judge_w_SDl();
		if(bIsLTR50){
			res = G2_Judge_w_SDlLTR();
		}
		if(bIsLTR18){
			res = G2_Judge_w_SDlLTR_l18();					//v2.20
		}

        R_CS_SetInspectMode(insp_mode);


    return res;
}

int SetInspAPl(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
        R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // --- 検査開始 ---
//        ProcessTimerStart();

        res = Inspect( &pResult );

    // --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

        R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        // Inspection Plating ( CB Side )
        InspBinPn	= BinP2;

		R_move_bin_memory( InspBinPn, BinP0, 0, 0, FxSize-1, FySize-1 );
        while( !R_DrawGetCursor());
        R_DrawCls();
        while( R_DrawGetCursor());

        //Ｇ２エッジデータ判定　Ｗ方向
        res= G2_Judge_w_APl();

        R_CS_SetInspectMode(insp_mode);


    return res;
}

int SetInspZure(int mode)
{
    int res = 0;
    R_CS_RESULT_DATA    pResult;
    // 検査対象Gray Pn
        R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

    // --- 検査開始 ---
//        ProcessTimerStart();

        res = Inspect( &pResult );

    // --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

        R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

        int insp_mode = R_CS_GetInspectMode();
        R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

        // Inspection Plating ( CB Side )
        InspBinPn	= BinP2;

        //Ｇ２ズレ検査
		//v2.20	if(!bIsLTR50){
		if( (!bIsLTR50) && (!bIsLTR18) ){
			res = TWA_kensa_zure_w();
		} else {
			res = TWA_kensa_zure_l();
		}
        R_CS_SetInspectMode(insp_mode);


    return res;
}

int SetInspC1Space(int mode)
{
	int res = 0;
	R_CS_RESULT_DATA    pResult;
	// 検査対象Gray Pn
		R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	// --- 検査開始 ---
//        ProcessTimerStart();

		res = Inspect( &pResult );

	// --- 検査終了 ---
//        ProcessTimerStop();
 //       if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();

		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

		int insp_mode = R_CS_GetInspectMode();
		R_CS_SetInspectMode(insp_mode | INSPECT_STEP);

		// Inspection Plating ( CB Side )
		InspBinPn	= BinP2;

		// C1電極間検査
		if(bIsLTR18){
			res= LTR_Judge_C1Space_l18();								//v2.20
		}else{
			res= LTR_Judge_C1Space();
		}

		R_CS_SetInspectMode(insp_mode);


	return res;
}
