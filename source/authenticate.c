// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>			// Math functions
#ifdef __DOS__
	#include <conio.h>
	#include <sys\stat.h>
	#include <fcntl.h>
	#include <io.h>
	#include <dos.h>
#elif __GNUC__
	#include "R_conio.h"
	#include <sys/stat.h>
	#include <time.h>
#endif
#include <termio.h>
#include <unistd.h>

// TSC-9301 library
#include "R_stdio.h"
#include "R_stdlib.h"
#include "R_video.h"
#include "R_graph.h"
#include "R_dgraph.h"
#include "R_menu.h"
#include "R_winb.h"
#include "R_gpio.h"
#include "R_time.h"
#include "R_dmenu.h"
#include "R_filter.h"
#include "R_bgray.h"
#include "R_edge.h"
#include "R_meas.h"			// ２値画像処理ライブラリ用ヘッダ
#include "R_robust.h"
#include "R_cursor.h"
#include "R_gray.h"
#include "R_search.h"
#include "R_bitmap.h"
#include "R_incy56.h"		// ＩＮーＣＹー５６（差動ドライバーインターフェース）ライブラリー
#include "R_camlnk.h"
#include "R_drx124.h"
#include "R_system.h"

#include "R_binrec.h"
#include "R_trans.h"		// Ｒ＿ｔｒａｎｓ関数を使用する
#include "R_math.h"


// common library
#include "R_cs.h"
//#include "R_cdraw.h"
#include "R_led.h"
#include "R_cs_fil.h"
#include "R_cs_sav.h"
#include "R_cs_lan.h"
int		KeyLogDataWrite( char *UsbKeyUser, int SChange );
static int UsbKeyCertification( char *UsbKeyInformation, int SChange  );

#define USBKEY_MAX 10
char	UsbKeyInformation[1024];

static int UsbKeyCertification( char *UsbKeyInformation, int SChange )
{
	int		i;
	int		UsbKeySerial;
	int 	UsbKeyVendor;
	FILE	*fpListVendor;
	FILE	*fpListSerial;
	FILE	*fpTmpVendor;
	FILE	*fpTmpSerial;
	FILE	*fpListUsersNames;
	char	ListSerial[USBKEY_MAX][128];
	char	ListVendor[USBKEY_MAX][128];
	char  ListUsers[USBKEY_MAX][128];
//	char	ComVendor[] = "cat /proc/bus/usb/devices | grep Vendor > usbvendor.txt";
//	char	ComSerial[] = "cat /proc/bus/usb/devices | grep Serial > usbserial.txt";
	char	ComVendor[] = "cat /sys/kernel/debug/usb/devices | grep Vendor > usbvendor.txt";
	char	ComSerial[] = "cat /sys/kernel/debug/usb/devices | grep Serial > usbserial.txt";

	int		list_no;
	int		list_count;
	char	buffer[128];
	int		FlagSerial = 0;
	int		FlagVendor = 0;
	char	InfoSerial[128];
	char	InfoVendor[128];
	int 	UsbKeySerialID;

	//USB Serial Numberの認証
	if( NULL == (fpListSerial=fopen("usbserial.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbserial.dat)" );
		return ERROR;
	}
	else{
		list_no = 0;
		while(NULL != fgets(buffer, 128, fpListSerial )){
			if( list_no > USBKEY_MAX ){
				fclose( fpListSerial );
				//登録Listが多すぎる場合はエラー
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (too many list of usbserial.dat)" );
				fclose( fpListSerial );
				return ERROR;
			}
			if( 1>=strlen(buffer) ){
				//登録Listがゼロの場合はエラー
				if( 0 == list_no ){
					R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (no list of usbserial.dat)" );
					fclose( fpListSerial );
					return ERROR;
				}
			}

			if( 10 < strlen(buffer) ){
				buffer[strlen(buffer)-1] = '\0';
				strcpy( ListSerial[list_no], buffer );
				//printf("ListSerial[%d]:[%s]\n",list_no,ListSerial[list_no] );
				list_no++;
			}
		}
		fclose( fpListSerial );
		list_count = list_no;

		printf("--- USB KEY LIST ---\n");
		for( i=0; i<list_count; i++){
			printf("LIST SERIAL[%2d]:[%s]\n",i, ListSerial[i] );
		}

		system ( ComSerial );

		if( NULL == (fpTmpSerial=fopen("usbserial.txt","rt")) ){
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbserial.txt)" );
			return ERROR;
		}
		else{
			list_no = 0;
			while(NULL != fgets(buffer, 128, fpTmpSerial )){
				buffer[strlen(buffer)-1] = '\0';
				for( i=0; i<list_count; i++){
					if( 0==strcmp( buffer, ListSerial[i] ) ){ //success certification of usb key.
						UsbKeySerial = ListSerial[i];
						UsbKeySerialID = i;
						fclose( fpTmpSerial );
						strcpy( InfoSerial, buffer );
						FlagSerial = 1;
						goto SERIAL_SUCCESS;
					}
				}
			}
			fclose( fpTmpSerial );
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION FAIL!! --\n KEY IS NOT FOUND" );
			return ERROR;
		}
	}

SERIAL_SUCCESS:

	if( NULL == (fpListVendor=fopen("usbvendor.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbvendor.dat)" );
		return ERROR;
	}
	else{
		list_no = 0;
		while(NULL != fgets(buffer, 128, fpListVendor )){
			if( list_no > USBKEY_MAX ){
				fclose( fpListVendor );
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (too many list of usbvendor.dat)" );
				fclose( fpListVendor );
				return ERROR;
			}
			if( 1>=strlen(buffer) ){
				if( 0 == list_no ){
					R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (no list of usbvendor.dat)" );
					fclose( fpListVendor );
					return ERROR;
				}
			}

			if( 10 < strlen(buffer) ){
				buffer[strlen(buffer)-1] = '\0';
				strcpy( ListVendor[list_no], buffer );
				//printf("ListVendor[%d]:[%s]\n",list_no,ListVendor[list_no] );
				list_no++;
			}
		}
		fclose( fpListVendor );
		list_count = list_no;

		printf("--- USB KEY LIST ---\n");
		for( i=0; i<list_count; i++){
			printf("LIST VENDOR [%2d]:[%s]\n",i, ListVendor[i] );
		}

		system ( ComVendor );

		if( NULL == (fpTmpVendor=fopen("usbvendor.txt","rt")) ){
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbvendor.txt)" );
			return ERROR;
		}
		else{
			list_no = 0;
			while(NULL != fgets(buffer, 128, fpTmpVendor )){
				buffer[strlen(buffer)-1] = '\0';
				for( i=0; i<list_count; i++){
					if( 0==strcmp( buffer, ListVendor[i] ) ){ //success certification of usb key. //list_no //<z2>
						UsbKeyVendor = ListVendor[i];
						fclose( fpTmpVendor );
						strcpy( InfoVendor, buffer );
						FlagVendor = 1;
						goto VENDOR_SUCCESS;
					}
				}
			}
			fclose( fpTmpVendor );
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION FAIL!! --\n KEY IS NOT FOUND" );
			return ERROR;
		}
	}

VENDOR_SUCCESS:

	if( NULL == (fpListUsersNames=fopen("key_users.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCannot open USB KEY Users data" );
		return ERROR;
	}
	else{
		list_no = 0;
		while(NULL != fgets(buffer, 128, fpListUsersNames )){

			if( 5 < strlen(buffer) ){
				buffer[strlen(buffer)-1] = '\0';
				strcpy( ListUsers[list_no], buffer );
				list_no++;
				}
			}
	}
	fclose( fpListUsersNames );
		printf("---USB KEY USER NAMES ---\n");
		for( i=0; i<list_count; i++){
			printf("KEY USERS[%d]:[%s]\n",i, ListUsers[i] );
		}


	if(OK != KeyLogDataWrite(ListUsers[UsbKeySerialID], SChange)){
		return ERROR;
	}

	R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- Authentication Success --\nKey User : %s", ListUsers[UsbKeySerialID] );

	return OK;
}

int		KeyLogDataWrite( char *UsbKeyUser, int SChange )
	{
		int						mn, fn;
		FILE *fin;
		static char		fname[] = "key_log.csv";
		char buff[100];
		time_t nTime;

switch( SChange ) {	
case 0:
case 1:
case 2:
case 3:
case 4:
case 6:
case 7:
case 8:
case 9:
case 10:
case 11:
case 12:
case 13:
case 14:
			if (UsbKeyUser[0] != 'I'){
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- Access Denied! --" );
				return ERROR;
			}
			break;
			
case 15: //9499
	if (UsbKeyUser[0] == 'I')	instru = 0; //authorized = 0
		
	else	instru = 1; //unauthorized = 1
	
	
break;			
			}		


			if((fin = fopen( fname, "a" )) == NULL){
				fclose( fin );
				return(ERROR);
			}

				nTime = time(NULL);
				strftime(buff, 100, "Date: %Y:%m:%d Time: %H:%M:%S ", localtime(&nTime));
				fprintf( fin, buff);

			switch( SChange ) {
				case 14:
					fprintf( fin, " : Inspection Setting\n", UsbKeyUser);
					break;
				case 15:
					fprintf( fin, " : System Setting\n", UsbKeyUser);		
					break;
				case 16:
					fprintf( fin, " : Light Setting\n", UsbKeyUser);
					break;
				default:	//ERROR
					fprintf( fin, " : ERROR\n" );
					break;
			}
			fclose( fin );
						
			return(OK);
}


