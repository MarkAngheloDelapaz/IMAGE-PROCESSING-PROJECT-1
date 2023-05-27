#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// TSC-9301 library
#include "R_stdio.h"
#include "R_stdlib.h"
#include "R_video.h"
#include "R_dgraph.h"
#include "R_dmenu.h"
#include "R_cursor.h"
#include "R_cs.h"
#include "R_cdraw.h"

#include "tp_sub.h"
#include "mcr03.h"
#include "mcr10.h"
#include "mcr18.h"
#include "esr25.h"
#include "ltr50.h"
#include "ltr18.h"													//v2.20

static char     SpecDataFileName[]	= "TP-S_sp.csv";

int				Pilot_mode			= OFF;							//�p�C���b�g���[�h
int				InspClass			= 0;							// ���� ���e��

char			MarkChar[10]		= { "-----" };
int				InitializeFlag		= ERROR;						// �����������t���O
int             GLMode				= NO;
int             GLCount				= 0;
// Spec Set
char			SpecData[20];										// �o�[�R�[�h�f�[�^
char            SpecDataBak[20];									// �o�[�R�[�h�f�[�^
char			GLData[10];											// GL data
char            SpecType[3]			= { '0', '3', 0 };				// ���i�^�C�v
char            DataClass[4][5]		= { "D", "F", "J", "" };
//static char   PilotDisp[2]        = { ' ', 'J' };

//Bar cord
static	int				TWA_IEC24[27] = { 100, 110, 120, 130, 150, 160, 180, 200, 220, 240, 250, 270, 300,
                                          330, 360, 390, 430, 470, 500, 510, 560, 620, 680, 750, 820, 910 };


#ifdef __DOS__
    #define		SerialCh  1
#elif __GNUC__
    #define		SerialCh  4
#endif

static int              NG_LIMIT=5000;                                     		//2010.09.23

static int              LotSpectotal;                                           //2010.09.23
static int              NGpcs_Hilimit;                                          //2010.09.23
static int              NGpcs_Lolimit;                                          //2010.09.23
static int              Automotive_switch = ON;                                 //2014.03.06
static int              AUTO_flag = 1;                                 	    	//2014.03.06    AUTO_flag[0: Normal Spec    1: Automotive Spec]

extern int				bIsNitto;												// �����@����

/**********************************************************************************
    Type date and time write
**********************************************************************************/

static int		SpecDataWrite( int w_mode )
    {
#ifdef __DOS__
        struct					dosdate_t date;
        struct					dostime_t time;
#elif __GNUC__
        time_t					tTime;
        struct	tm				tmTime;									// ���b�g�J�n Date,Time
#endif
        FILE					*fin;

        //�t�@�C���I�[�v��
            if((fin = fopen( SpecDataFileName, "a" )) == NULL) {
                fclose( fin );
                return ERROR;
            }

#ifdef __DOS__
        //���݂̓��t���擾
            _dos_getdate(&date);
            fprintf( fin, "date:%04d-%02d-%02d ", date.year, date.month, date.day );
        //���݂̎��Ԃ��擾
            _dos_gettime(&time);
            fprintf( fin, "time:%02d:%02d:%02d", time.hour, time.minute, time.second );
#elif __GNUC__
        //���݂̓��t���擾
        //���݂̎��Ԃ��擾
            tTime = time(NULL);
            memcpy(&tmTime , localtime(&tTime) ,sizeof(tmTime));
            fprintf( fin, "date:%04d-%02d-%02d ", tmTime.tm_year+1900, tmTime.tm_mon+1, tmTime.tm_mday );
            fprintf( fin, "time:%02d:%02d:%02d", tmTime.tm_hour     , tmTime.tm_min, tmTime.tm_sec );

#endif
            switch( w_mode ) {
                case 0:		//Type data
                    fprintf( fin, " : %s\n", SpecData );
                    break;
                case 1:		//GL data
                    fprintf( fin, " : %s\n", GLData );
                    break;
                case 2:		//ERROR
                    fprintf( fin, " : ERR. Read Out Bar code data\n" );	//�o�[�R�[�h�f�[�^�[�Ǎ��� ERROR
                    break;
                default:	//ERROR
                    fprintf( fin, " : ERROR\n" );
                    break;
            }
            fclose( fin );

            return OK;

    }	// SpecDataWrite( int w_mode ) end


int		TWA_check_IEC24( void )
    {
        int				m, n, i;

        if((SpecData[4]=='D')||(SpecData[4]=='F')||(SpecData[4]=='C')) {
            if( MarkChar[3] == ' ' ) return(ERROR);
            m = 3;
        } else {
            if( MarkChar[3] != ' ' ) return(ERROR);
            m = 2;
        }

        n=i=0;
        while( i<m ) {
            if( MarkChar[n] != 'R' ) {
                Comment[i] = MarkChar[n];
                i++;
            }
            n++;
        }
        Comment[m] = 0;

        i = atoi( Comment );
        if((SpecData[4]!='D')&&(SpecData[4]!='F')&&(SpecData[4]!='C')) {
            i *= 10;
        }
        for( n=0; n<26; n++ ) {
            if( i == TWA_IEC24[n] ) break;
        }

        if( n == 26 ) return(ERROR);
        return(OK);
    }

void	TWA_m4_m3( void )
    {
        int				n;

        for( n=0; n<4; n++ ) {
            if( MarkChar[n] == 'R' ) break;
        }

        switch( n ) {
            case 0:
                if( MarkChar[3] == '0' ) {
                    MarkChar[3] = ' ';
                }
                break;
            case 1:
                if( MarkChar[3] == '0' ) {
                    MarkChar[3] = ' ';
                }
                break;
            case 2:
                if( MarkChar[3] == '0' ) {
                    MarkChar[2] = '0';
                    MarkChar[3] = ' ';
                }
                break;
            default:
                if( MarkChar[2] == '0' ) {
                    MarkChar[2] = MarkChar[3] + 0x01;
                    MarkChar[3] = ' ';
                }
                break;
        }
    }

//�eclass Marking 3 digit
void	TWA_F_setting( void )
    {
        if( TWA_Spec != SpecJPW ) {
            if((SpecData[4]=='D')||(SpecData[4]=='F')||(SpecData[4]=='C')) {
                if( TWA_check_IEC24() == OK ) TWA_m4_m3();
                else                          TWA_Insp_mode = Insp_Pinhole;		//Pinhole inspection
            }
        }
    }
// from ESR25
/************************************************
�@��ʃX�C�b�`�\���n�m
************************************************/
struct message_t TWA_mes = { 14, 1, "function" };

void	TWA_open_switch( char *str[], int cn )
{
    int		i, fw, sw;

    fw = Disp_column/cn;		//�g���T�C�Y
    sw = fw-2;				//������T�C�Y

    R_init_scr_switch( );
    for( i = 0; i < cn; i++ ) {
        TWA_mes.str[ 0 ] = str[ i ];
        TWA_mes.xlen = sw;
        R_win_message( i*fw, Disp_line-3, &TWA_mes );
        R_append_scr_switch( ( i*fw+1 )*Jph_xsize, ( Disp_line-2 )*Jph_ysize,
            ( ( i+1 )*fw-1 )*Jph_xsize-1, ( Disp_line-1 )*Jph_ysize-1 );
    }
}
/************************************************
�@��ʃX�C�b�`�\���n�e�e
************************************************/

void	TWA_close_switch( int cn )
{
    int		i;

    R_close_scr_switch( );
    for( i=0; i<cn; i++ ) R_close_window( );

}

//�i�o�v�����ݒ�
void	TWA_jpw_setting( void )
{
    int			x1, x2, y1, y2, i;
    char		*jpnfunc[] = { "�W��i�V", "000�W��" };
    char		*engfunc[] = { "NO Marking", "Marking" };

    if( SpecData[0] != '0' ) return;

    sprintf( Comment, "Select specification" );
    R_chrdisp( 3, 12, Comment );

    TWA_Spec = SpecJPW;			//�i�o�v

    x1 = 1;
    y1 = ( Disp_line-2 )*Jph_ysize;
    x2 = FX_SIZE-2;
    y2 = ( Disp_line-1 )*Jph_ysize-1;
    R_set_cursor_window( x1, y1, x2, y2 );
    if( R_get_language() == JPN_DISP )  TWA_open_switch( jpnfunc, 2 );
    else                                TWA_open_switch( engfunc, 2 );
    R_cursor_on( );
    i = 1;
    while( i ){
        while( R_check_scr_switch( ) != 1 );	//�������X�C�b�`�҂�
        switch( ScrSw_select ) {
            case 1:		//�W��i�V
                TWA_Insp_mode = Insp_Pinhole;		//�s���z�[������
                i = 0;
                break;
            case 2:		//000
                i = 0;
                break;
        }
        while( R_check_scr_switch( ) );
    }
    TWA_close_switch( 2 );
    while( R_check_scr_switch( ) );
    R_set_cursor_window( 0, 0, FX_SIZE-1, FY_SIZE-1 );
    sprintf( Comment, "                            " );
    R_chrdisp( 3, 12, Comment );
}

// Low ohm check
void	TWA_R100_check( void )
    {
        int					n;
        double				r_ohm = 0.0;
        char				r_data[5] = { 0,0,0,0,0 };

            if( SpecData[0] != 'R' ) return;

            for( n=0; n<4; n++ ) {
                r_data[n] = SpecData[n];
                if( r_data[n] == 'R' ) {
                    r_data[n] = '.';
                }
            }
            r_ohm = atof( r_data );

            if( r_ohm < 0.100 ) TWA_Insp_mode = Insp_Pinhole;		//�s���z�[������

    }

/*********************************************************************************
    �֐����F	�s�v�`�Q�o���������Q������
    �@�@�\�F	�p�C���b�g���[�h���m�F����
    �`�@���F	int TWA_Pilot_chk(  )
    �������F	�Ȃ�
    �߂�l�F	�Ȃ�
    ���@���F	�s�v�`�{�̂��p�C���b�g���[�h�ɂȂ��Ă��邩�m�F����B
                �ُ펞�́A��ʂɕ\������B
*********************************************************************************/

void	TWA_Pilot_chk( void )
    {
        //Chk
            if( CheckPilotMode() != Pilot_mode ) return;

        //��ʏ���
            R_gray_fill( 0, 0, FxSize-1, FySize-1, 0 );
            R_bin_cls( 0, 0, FxSize-1, FySize-1 );

        //ERROR�\��
            R_chrdisp_center( 10,"TWA pilot mode abnormality" );
            while( R_get_cursor() == NO_TOUCH );
            while( R_get_cursor() != NO_TOUCH );

        //��ʏ���
            R_gray_fill( 0, 0, FxSize-1, FySize-1, 0 );
            R_bin_cls( 0, 0, FxSize-1, FySize-1 );
    }





/**********************************************************
    �֐����F	�r�������r����
    �@  �\�F
    �`  ���F	static int SpecSet( int nMenuNumber )
    �������F
    �߂�l�F
    ��  ���F
    ���ӎ����F
**********************************************************/

int SpecSet( int nMenuNumber )
{
//		int						old_InspValue;
	int						old_InspClass;
	char					old_MarkChar[10];
	int						dc, dl;
	int						ne, dn;
	int						nWidthX	= FxSize;
	int						nWidthY	= FySize;
	int						res;
	int						hSio;
    char					work[50], work1[50];
	int                     Sum_lot;               //Sosei Lot Detect 2010.10.17
	int                     number;                //Sosei Lot Detect 2010.10.17
	RTIME                   RTWaitTime;
	int                     Lot_Spec, Pcs_Per_reel;//2010.09.23
	char                    No_reel[3],No_reel1[2];//2010.09.23
	char					Buff[20],fdata[20];
	int						err;

	Pilot_mode = OFF;
	
	// Spec data backup
//	old_InspValue	= InspValue;
	old_InspClass	= InspClass;
	for( dn=0; dn<5; dn++ )	old_MarkChar[dn] = MarkChar[dn];

	// Initial spec data
	GLMode = NO;
	if(nMenuNumber != 1){
		for( dn=0; dn<20; dn++ ) {
			 SpecDataBak[dn]	= SpecData[dn];				// �O�̃o�[�R�[�h�f�[�^�[��ۑ�
			 SpecData[dn]		= 0;
		}
	}
	// Auto mode
	if( 0 == nMenuNumber ) {
		// --- SIO open
		if( (hSio=R_sio2_open( SIO_CHANNEL(SerialCh), RATE9600BPS, BIT_8, PARITY_NONE, STOP_2, FLOW_SOFT )) <= 0 ) {
		    if( R_get_language() == JPN_DISP )	strcpy( Comment, "!!! �V���A���̏������Ɏ��s���܂��� !!!" );
		    else								strcpy( Comment, "!!! Serial unable to initialize !!!" );
		    DrawMessage( Comment, DRAW_RED, 2, 0 );
		    return ERROR;
		}
		// --- Spec set sig. check
		if( R_get_language() == JPN_DISP )	strcpy( Comment, "�o�[�R�[�h�f�[�^�[�ǂݍ��ݏ�����" );
		else								strcpy( Comment, "During preparations" );
		DrawMessage( Comment, DRAW_YELLOW, 5, 2 );
		R_time_start(&RTWaitTime);
		while( CheckSpecSet()==0 ) {
		    if(5000000<R_time_read(&RTWaitTime)){
		        R_sio2_close(hSio);
		        DrawMessageOff();
		        if( R_get_language() == JPN_DISP )	strcpy( Comment, "!! �d�l�ݒ�^�C���A�E�g ERROR !!" );
		        else								strcpy( Comment, "!! ERROR Spec Set sig. Time-out" );
		        DrawMessage( Comment, DRAW_RED, 2, 0 );
		        return ERROR;
		    }
		    while( R_sio2_get_rcvbuff(hSio) ) R_sio2_getc(hSio);
		}
		DrawMessageOff();
		// --- Read data
		if( R_get_language() == JPN_DISP )	strcpy( Comment, "�o�[�R�[�h�f�[�^�[�ǂݍ��ݒ�" );
		else								strcpy( Comment, "Bar code data reading" );
		DrawMessage( Comment, DRAW_YELLOW, 5, 2 );
		dc = (nWidthX/Jph_xsize-20)/2;
		dl = (nWidthY/Jph_ysize)/2+4;
		sprintf( work, "DATA = " );
		R_chrdisp( dc, dl, work );
		dc+=7;

		if(Option & REPI)   ne = 13;        //REPI�d�l 20170131  MCR03/ESR25/SFR25
		else                ne = 14;        //RIST�d�l 20170131    20170508 debug

		for( dn=0; dn<ne; dn++ ) {
			R_time_start(&RTWaitTime);
			while( R_sio2_get_rcvbuff(hSio)==0 ) {
				if(5000000<R_time_read(&RTWaitTime)){
					DrawMessageOff();
					R_sio2_close(hSio);
					if( R_get_language() == JPN_DISP )	strcpy( Comment, "�o�[�R�[�h�f�[�^�[�Ǎ��� ERROR" );
					else								strcpy( Comment, "ERR. Read Out Bar code data" );
					DrawMessage( Comment, DRAW_RED, 2, 0 );
					// Bar code Data write
					SpecDataWrite( 2 );
					return ERROR;
				}
			}
			SpecData[dn] = R_sio2_getc(hSio);
			if(( dn == 0 )&&( SpecData[dn] == 'G' )) {
				ne = 4;
				GLMode = YES;
			}
			if(GLMode != YES){
				if(( SpecData[11] == SpecType[0] )&&( SpecData[12] == SpecType[1] )) Sum_lot = 1;             //2010.07.17
				else                                                                 Sum_lot = 0;             //2010.07.17
				//2014.03.06
				if(!bIsMCR01){
					if( SpecData[13] != ' ' )       				                          {Automotive_switch = ON;    TWA_Spec = SpecResin;    AUTO_flag = 1;}
//					if( SpecData[13] == 'A' )       				                          {Automotive_switch = ON;    TWA_Spec = SpecResin;    AUTO_flag = 1;}
//					else if(( Automotive_switch == ON )&&(SpecData[5] == ' '))                {Automotive_switch = ON;    TWA_Spec = SpecNormal;   AUTO_flag = 1;}
//					else if(( Automotive_switch == ON )&&(SpecData[5] == '3')) 				  {Automotive_switch = ON;    TWA_Spec = SpecResin;    AUTO_flag = 1;}
//					else if(( Automotive_switch == ON )&&(SpecData[5] == '8')) 				  {Automotive_switch = ON;    TWA_Spec = SpecJPW;      AUTO_flag = 1;}
					else                          					                       	  {Automotive_switch = OFF;   AUTO_flag = 0;                  	     }
					//	R_write_EEPROM_incy67( TWA_incy67_ch, Automotive_flag, AUTO_flag );
				} else {
					if( SpecData[13] != ' ' )	{Automotive_switch = ON;	AUTO_flag = 1;}
					else						{Automotive_switch = OFF;   AUTO_flag = 0;}
				}
			}
				
			work1[0] = SpecData[dn];
			work1[1] = 0;
			sprintf( work, "%s", work1 );
			R_chrdisp( dc+dn, dl, work );
		}

		while( R_sio2_get_rcvbuff(hSio) ) R_sio2_getc(hSio);
		if( GLMode == YES ) {
			R_sio2_close(hSio);
			for( dn=0; dn< 5; dn++ ) GLData[dn]		= SpecData[dn];
			for(     ; dn<10; dn++ ) GLData[dn]		= 0;
			for( dn=0; dn<20; dn++ ) SpecData[dn]	= SpecDataBak[dn];		// �O�̃o�[�R�[�h�f�[�^�[�ɖ߂�
			GLCount = 0;
			// GL Data write
			    SpecDataWrite( 1 );
			return OK;
		}
		DrawMessageOff();
		
		// --- �f�[�^���M�v���҂�
		R_time_start(&RTWaitTime);
		while( CheckSpecSet()!=0 ) {
			if(5000000<R_time_read(&RTWaitTime)){
				R_sio2_close(hSio);
				if( R_get_language() == JPN_DISP )	strcpy( Comment, "�f�[�^���M�^�C���A�E�g ERROR" );
				else								strcpy( Comment, "ERROR. Time-out" );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}
		}

		// --- �o�[�R�[�h�f�[�^���M
		for( dn=0; dn<500; dn++ ) R_time_delay( 1000 );		// �҂�����
		sprintf( work, "D%s\r", SpecData );
		if(Option & REPI){										//20170131 REPI
			if( Sum_lot == 0 ) number = 14; 					//GOSEI
			if( Sum_lot == 1 ) number = 15; 					//GOSEI
		} else {												//20170508 RIST
			if( Sum_lot == 0 ) number = 15; 					//2010.03.22
			if( Sum_lot == 1 ) number = 16; 					//2010.03.22
		}

		for( dn=0; dn<number; dn++ ) {                       //2010.07.17
			R_time_start(&RTWaitTime);
			while( R_sio2_get_sndbuff(hSio) != 0 ) {
				if(5000000<R_time_read(&RTWaitTime)){
					R_sio2_close(hSio);

					strcpy( Comment, "ERROR. Time-out" );
					DrawMessage( Comment, DRAW_RED, 2, 0 );
					return ERROR;
				}
			}
			res = R_sio2_putc(hSio, work[dn] );
			if( res == ERROR ) {
				R_sio2_close(hSio);
				strcpy( Comment, "ERROR. Data upload" );	//�o�[�R�[�h�f�[�^�[�Ǎ��� ERROR
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}
			R_time_delay( 10000 );	// �҂�����
		}

		strcpy( Comment, "Data upload end" );
		DrawMessage( Comment, DRAW_YELLOW, 5, 2 );
		// --- �f�[�^���M�v�������҂�
		R_time_start(&RTWaitTime);

		while( CheckSpecSet()==0 ) {
			while( R_sio2_get_rcvbuff(hSio) ) {
				work1[0] = R_sio2_getc(hSio);
			}
			if((5000000<R_time_read(&RTWaitTime))||( work1[0]=='X')) {
				DrawMessageOff();
				R_sio2_close(hSio);
				if( R_get_language() == JPN_DISP )	strcpy( Comment, "�f�[�^�m�F ERROR" );
				else								strcpy( Comment, "Data confirmation error" );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				strcpy( Comment, "Wait for Spec set sig. OFF" );
				DrawMessage( Comment, DRAW_YELLOW, 5, 2 );
				while( CheckSpecSet()==0 );
				DrawMessageOff();
				return ERROR;
			}
		}
		R_sio2_close(hSio);
		DrawMessageOff();

		// --- Bar code Data write
		SpecDataWrite( 0 );
		// �^�C�v��r
		if( Sum_lot == 0 ) number = 10;                    //GOSEI 2010.03.22
		if( Sum_lot == 1 ) number = 11;                    //GOSEI 2010.03.22

		for( dn=0; dn<2; dn++ ) {
//			if( SpecData[10+dn] != SpecType[dn] ) {
			if( SpecData[number+dn] != SpecType[dn] ) {       	//2010.07.17
				if( R_get_language() == JPN_DISP )	strcpy( Comment, "���i�^�C�v ERROR" );
				else								strcpy( Comment, "Product type error" );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
//				InspValue	= old_InspValue;
				InspClass	= old_InspClass;
				for( dn=0; dn<5; dn++ )	MarkChar[dn] = old_MarkChar[dn];
				return ERROR;
			}
		}
		// RIST 
		if((Option & REPI) == 0){
			if(GLMode != YES){
			//2010.09.23
				switch( SpecData[6] ) {
					case 'M':
					case 'F':
					case 'L':
							Pcs_Per_reel = 10000;
							break;
					case 'E':
					case 'K':
							Pcs_Per_reel = 5000;
							break;
					case 'N':
							Pcs_Per_reel = 30000;
							break;
					case 'O':
							Pcs_Per_reel = 50000;
							break;
					case 'Q':
							Pcs_Per_reel = 20000;
							break;
					case 'Y':
							Pcs_Per_reel = 15000;
							break;
					case 'R':
							Pcs_Per_reel = 40000;
							break;
					case 'J':
							Pcs_Per_reel = 4000;
							break;
					case 'H':
							Pcs_Per_reel = 2000;
							break;
					case 'P':
							Pcs_Per_reel = 200;
							break;
					default:
							break;
				}

				if(!bIsMCR01){
					if( Sum_lot == 1 ){            //Gather Lot case
						No_reel[0] = SpecData[8];
						No_reel[1] = SpecData[9];
						No_reel[2] = SpecData[10];
						Lot_Spec = atoi(No_reel) * Pcs_Per_reel;
					}else{                         //Single Lot case
						No_reel1[0] = SpecData[8];
						No_reel1[1] = SpecData[9];
						Lot_Spec = atoi(No_reel1) * Pcs_Per_reel;
					}
				} else {
					if( Sum_lot == 1 ){            //Gather Lot case
						No_reel[0] = SpecData[7];
						No_reel[1] = SpecData[8];
						No_reel[2] = SpecData[9];
						Lot_Spec = atoi(No_reel) * Pcs_Per_reel;
					}else{                         //Single Lot case
						No_reel1[0] = SpecData[7];
						No_reel1[1] = SpecData[8];
						Lot_Spec = atoi(No_reel1) * Pcs_Per_reel;
					}
				}

				if(Lot_Spec < LotSpectotal) NG_LIMIT = NGpcs_Lolimit;
				else                  		NG_LIMIT = NGpcs_Hilimit;
				// NG limit
			//	R_write_EEPROM_incy67( TWA_incy67_ch, NG_LIMIT_flag, NG_LIMIT );
			}
		}
	} else	if( 1 == nMenuNumber ) {
		number = 14;
		dc = (nWidthX/Jph_xsize-20)/2;
		dl = (nWidthY/Jph_ysize)/2+4;
		sprintf( work, "DATA = %s",SpecData );
		R_chrdisp( dc, dl, work );
		dc+=7;

	} else {
		// --- manual settting 
		if(Option & REPI){		// REPI
			if( OK != R_draw_input_keyboard( DRAW_YELLOW, DISP_H2, SpecData, "      ", "Input spec data", 0 ))	return ERROR;
			printf( "Input data [%s]\n", SpecData );
		} else {
			R_chrdisp( 3, 4, Comment );
			sprintf( Comment, "DATA = " );
			R_chrdisp( 3, 6, Comment );
			sprintf(Buff, "");
			sprintf(fdata,"%*s",7,"Buff");
		 	err = R_input_keyboard(Buff,fdata," Barcode DATA ", 0 );
			for( dn=0; dn<7; dn++ ) {
				SpecData[dn] = toupper(Buff[dn]);
				work1[0] = SpecData[dn];
				work1[1] = 0;
				sprintf( Comment, "%s", work1 );
				R_chrdisp( 10+dn,6, Comment );
			}
			for( ; dn<13; dn++ ) {                           //2010.03.22
				SpecData[dn] = ' ';
			}

			if( SpecData[0] == 'G' ){
				GLMode = YES;
			}else{ //9499-NOTE
				
				Automotive_switch = OFF;
			//	if( bIsMCR03 && SpecData[6] == 'A' )   Automotive_switch = ON;
				if( bIsMCR03 && SpecData[6] != ' ' )   Automotive_switch = ON;
				if( bIsMCR01 && SpecData[6] != ' ' )   Automotive_switch = ON;
			}
		//	AUTO_flag = R_read_EEPROM_incy67( TWA_incy67_ch, Automotive_flag );  //2013.05.14
/*
			if(( Automotive_switch == ON )&&( AUTO_flag != 1 )){
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				sprintf( Comment, "Product Spec Set Miss" );
				R_chrdisp_center_scale( 12, Comment );
				WaitMouseClick();
				for( dn=0; dn<20; dn++ ) SpecData[dn]=0;
				InitializeFlag = ERROR;
				return ERROR;
			}
			if(( Automotive_switch != ON )&&( AUTO_flag == 1 )){
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				sprintf( Comment, "Product Spec Set Miss" );
				R_chrdisp_center_scale( 12, Comment );
				WaitMouseClick();
				for( dn=0; dn<20; dn++ ) SpecData[dn]=0;
				InitializeFlag = ERROR;
				return ERROR;
			}
*/
			if( GLMode == YES ) {
				for( dn=0; dn< 5; dn++ ) GLData[dn]		= SpecData[dn];
				for(     ; dn<10; dn++ ) GLData[dn]		= 0;
				for( dn=0; dn<20; dn++ ) SpecData[dn]	= SpecDataBak[dn];		// �O�̃o�[�R�[�h�f�[�^�[�ɖ߂�
				GLCount = 0;
				// GL Data write
				SpecDataWrite( 1 );
				return OK;
			}
		}
	}

	// 2017/01/27 setting
	TWA_Spec		= SpecNormal;	//�ʏ�i
	TWA_Insp_level	= Insp_Low;		//�ʏ팟��
	TWA_Insp_mode   = Insp_Normal;
	Pilot_mode		= OFF;
	TWA_Mark_Insp	= 0;
	bIsG2Straight     = 0;
	bIsG2Separate     = 1;
//9499 NOTE
	// InspValue setting
	for( dn=0; dn<4; dn++ )	MarkChar[dn] = SpecData[dn];
	MarkChar[4] = 0;

	// InspClass setting
	switch( SpecData[4] ) {
		case 'J':
			InspClass = CLASS_J;		// J��
			break;
		case 'F':
			InspClass = CLASS_F;		// F��
			break;
		case 'D':
			InspClass = CLASS_D;		// D��
			break;
		default:
			if( R_get_language() == JPN_DISP ) {
				sprintf( Comment, " !!! �@��؂�ւ����s !!! \n �ݒ�f�[�^������܂��� " );
			} else {
				sprintf( Comment, " !!! Specification change fail !!! \n There are not setting data " );
			}
			DrawMessage( Comment, DRAW_RED, 2, 0 );
			//InspValue	= old_InspValue;
			InspClass	= old_InspClass;
			for( dn=0; dn<5; dn++ )	MarkChar[dn] = old_MarkChar[dn];
			return ERROR;
			break;
	}

	if(bIsMCR01){
		bIsG2Straight     = 1;	// �b�� �����i�Œ�
		bIsG2Separate     = 0;
		switch( SpecData[5] ) {
			case 'P':				//NP3
					TWA_Spec		= SpecResin;
					TWA_Insp_mode   = Insp_Pinhole;
					break;
			
			case 'J':					//�p�C���b�g
				if( SpecData[4] == 'J' ) {
					TWA_Insp_level = Insp_High;	//�ڍ׌���
					Pilot_mode = ON;
#ifdef	PILOT_chip
					TWA_Insp_mode = Insp_Pilot;	//�p�C���b�g�p�`�b�v���Y���[�h
#endif
				} else {
					strcpy( Comment, "Pilot setting ERROR" );
					R_chrdisp( 3,12, Comment );
					return(ERROR);
				}
				break;
			default:
				TWA_Insp_mode   = Insp_Pinhole;
				break;	
		}
		//�i�o�v�����ݒ�iOFF:0 ON:1�j
		if( SpecData[0] == '0' ) {
			if( SpecData[5] =='-' ) {
				TWA_Spec		= SpecSpJ;			// High
				TWA_Insp_mode	= Insp_Pinhole;			// Pinhole
			} else {
				TWA_Spec		= SpecJPW;			//JPW
				TWA_Insp_mode	= Insp_Pinhole;			//Pinhole
			}
		}

		if( Automotive_switch == ON )													TWA_Spec = SpecSpG;  //2013.05.14
		if(( Automotive_switch == ON )&&(SpecData[0] == '0'))						    TWA_Spec = SpecSpJ;  //2013.05.14
		if(( Automotive_switch == ON )&&(SpecData[0] == '0')&&(SpecData[5] == 'P'))     TWA_Spec = SpecSpG;  //2013.05.14

	}
			
	if(bIsMCR03){
		bIsG2Straight     = 1;	// �b�� �����i�Œ�
		bIsG2Separate     = 0;
		//�i�o�v�����ݒ�
		if(( SpecData[0] == '0' )&&( SpecData[5] != '8' )) {
			TWA_Spec = SpecJPW;	//�i�o�v����
			MarkChar[1]=MarkChar[2]=MarkChar[3]=' ';
		}
		switch( SpecData[5] ) {
		  case ' ':
		  case 'E':
		  case 'I':
		  case 'M':
		  case 'U':
		  case 'W':
		  case 'Y':
		  case 'Z':
			TWA_F_setting();
			break;
		  case 'D':
			//			case 'V':
			TWA_Insp_level = Insp_High;		//�ڍ׌���
			//				TWA_F_setting();
			break;
		  case '3':
			if( TWA_check_IEC24() != OK ) {
				work1[0] = SpecData[5];
				work1[1] = 0;
				if( R_get_language() == JPN_DISP ) {
					sprintf( Comment, "���ʐݒ�G���[�@�d�l�ُ�H[%s]", work1  );
				} else {
					sprintf( Comment, "Special setting error specification[%s]", work1  );
				}
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return(ERROR);
			}
		  case '8':
			if( TWA_Spec != SpecJPW ) {
				TWA_Spec = SpecResin;		//�m�oIII(ST)
				if( SpecData[0] == '0' ) {
					// JPW set
					MarkChar[1]=MarkChar[2]=MarkChar[3]=' ';
				} else {
					TWA_F_setting();
				}
			} else {
				work1[0] = SpecData[5];
				work1[1] = 0;
				if( R_get_language() == JPN_DISP ) {
					sprintf( Comment, "���ʐݒ�G���[�@�d�l�ُ�H[%s]", work1  );
				} else {
					sprintf( Comment, "Special setting error specification[%s]", work1  );
				}
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return(ERROR);
			}
			break;
		  case 'T':
			TWA_Insp_mode = Insp_Hyouri;	//�\������
			break;
		  case '9':
			TWA_Insp_level = Insp_High;		//�ڍ׌���
			TWA_Insp_mode = Insp_Pinhole;	//�s���z�[������
			TWA_Spec = SpecJPW;		//�i�o�v
			MarkChar[1]=MarkChar[2]=MarkChar[3]=' ';
			break;
		  case 'J':					//�p�C���b�g
			if( SpecData[4] == 'J' ) {
				if(Option & REPI)	TWA_Spec = SpecResin;// JHONG
				//					TWA_Insp_level = Insp_High2;	//�ڍ׌���
				if((Option & REPI)==0)	TWA_Insp_level = Insp_High;		//�ڍ׌���	// JHONG
				TWA_F_setting();
				//					TWA_Insp_mode = Insp_Pinhole;	//�s���z�[������
				Pilot_mode = ON;
#ifdef	PILOT_chip
				TWA_Insp_mode = Insp_Pilot;	//�p�C���b�g�p�`�b�v���Y���[�h
#endif
			} else {
				strcpy( Comment, "Pilot setting ERROR" );
				R_chrdisp( 3,12, Comment );
				return(ERROR);
			}
			break;
		  case '-':					//�W�󌟍��L�����Z��
			if( SpecData[0] == '0' ) {
				TWA_Spec = SpecJPW;		//�i�o�v
			}
			TWA_Mark_Insp = 1;
			break;
		  case '1':					//�p�C���b�g Resin
			if((Option & REPI)==0){  // RIST
				if( SpecData[4] == 'J' ) {
					TWA_Spec = SpecResin;		//�m�oIII(ST)
					//						TWA_Insp_level = Insp_High2;	//�ڍ׌���
					TWA_Insp_level = Insp_High;		//�ڍ׌���
					TWA_F_setting();
					//						TWA_Insp_mode = Insp_Pinhole;	//�s���z�[������
					Pilot_mode = ON;
#ifdef	PILOT_chip
					TWA_Insp_mode = Insp_Pilot;	//�p�C���b�g�p�`�b�v���Y���[�h
#endif
				} else {
					strcpy( Comment, "Pilot setting ERROR" );
					R_chrdisp( 3,12, Comment );
					return(ERROR);
				}
				break;
			}
		  default:
			work1[0] = SpecData[5];
			work1[1] = 0;
			if( R_get_language() == JPN_DISP ) {
				sprintf( Comment, "���ʐݒ�G���[�@�d�l�s���H[%s]", work1  );
			} else {
				sprintf( Comment, "Special setting error specification[%s]", work1  );
				R_chrdisp( 3,12, Comment );
				return(ERROR);
				break;
			}
		}
		TWA_mark_set_m03();
		
	}
	if(bIsMCR10){
		bIsG2Straight     = 0;
		bIsG2Separate     = 1;
			switch( SpecData[5] ) {
			case ' ':
			case 'A':
			case 'E':
			case 'H':
			case 'I':
			case 'K':
			case 'N':
			case 'Q':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Z':
				if( R_get_language() == JPN_DISP ) {
					TWA_jpw_setting();
				} else {
					if( SpecData[0] == '0' ) {
						TWA_Spec = SpecJPW;	//�i�o�v
					}
				}
				TWA_R100_check();
				break;
			case '2':
				if( SpecData[0] == '0' ) {
					TWA_Insp_mode = Insp_Pinhole;	//�s���z�[������
					TWA_Spec = SpecJPW;		//�i�o�v
				}
				TWA_R100_check();
				break;
			case 'P':
				TWA_Spec = SpecResin;			// Plastic goods
				TWA_R100_check();
				break;
			case 'T':
				TWA_Insp_mode = Insp_Hyouri;	//�\������
				break;
			case 'D':
				TWA_Insp_level = Insp_High;		//Details inspection 
			case 'M':
				if( SpecData[0] == '0' ) {
					TWA_Spec = SpecJPW;		//�i�o�v
				}
				TWA_R100_check();
				break;
			case '9':
				TWA_Insp_mode = Insp_Pinhole;	//Pinhole inspection 
				TWA_Spec = SpecJPW;		//�i�o�v
				TWA_Insp_level = Insp_High;				//�ڍ׌���
				break;
			case 'J':					//�p�C���b�g
				if( SpecData[4] == 'J' ) {
//					TWA_Insp_level = Insp_High;		//�ڍ׌���
					Pilot_mode = ON;
#ifdef	PILOT_chip
					TWA_Insp_mode = Insp_Pilot;	//�p�C���b�g�p�`�b�v���Y���[�h
#endif
				} else {
					strcpy( Comment, "Pilot setting ERROR" );
					R_chrdisp( 3,12, Comment );
					return(ERROR);
				}
				break;
			case '-':					//�W�󌟍��L�����Z��
				if( SpecData[0] == '0' ) {
					TWA_Spec = SpecJPW;		//�i�o�v
				}
				TWA_Mark_Insp = 1;
				break;
			default:
				work1[0] = SpecData[5];
				work1[1] = 0;
				if( R_get_language() == JPN_DISP ) {
					sprintf( Comment, "���ʐݒ�G���[�@�d�l�s���H[%s]", work1  );
				} else {
					sprintf( Comment, "Special setting error specification[%s]", work1  );
				}
				R_chrdisp( 3,12, Comment );
				return(ERROR);
				break;
		}

		TWA_mark_set_m10();
	}
	if(bIsMCR18){
		bIsG2Straight     = 0;
		bIsG2Separate     = 1;
			switch( SpecData[5] ) {
			case ' ':
			case 'E':
			case 'H':
			case 'I':
			case 'K':
			case 'Q':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Z':
				if( R_get_language() == JPN_DISP ) {
					TWA_jpw_setting();
				} else {
					if( SpecData[0] == '0' ) {
						TWA_Spec = SpecJPW;	//�i�o�v
					}
				}
				TWA_R100_check();
				break;
			case 'D':
				TWA_Insp_level = Insp_High;		//�ڍ׌���
				TWA_jpw_setting();
				TWA_R100_check();
				break;
			case 'P':
				TWA_Spec = SpecResin;			// Plastic goods
				TWA_R100_check();
				break;
			case 'T':
				TWA_Spec = SpecMCL;				//MCL
				TWA_Insp_mode = Insp_Pinhole;	//�s���z�[������
				break;
			case 'M':
				if( SpecData[0] == '0' ) {
					TWA_Spec = SpecJPW;		//�i�o�v
				}
				TWA_R100_check();
				break;
			case '9':
				TWA_Insp_mode = Insp_Pinhole;	//Pinhole inspection 
				TWA_Spec = SpecJPW;		//�i�o�v
				TWA_Insp_level = Insp_High;				//�ڍ׌���
				break;
			case 'J':					//�p�C���b�g
				if( SpecData[4] == 'J' ) {
					TWA_Insp_level = Insp_High;		//�ڍ׌���
					Pilot_mode = ON;
#ifdef	PILOT_chip
					TWA_Insp_mode = Insp_Pilot;	//�p�C���b�g�p�`�b�v���Y���[�h
#endif
				} else {
					strcpy( Comment, "Pilot setting ERROR" );
					R_chrdisp( 3,12, Comment );
					return(ERROR);
				}
				break;
			case '-':					//�W�󌟍��L�����Z��
				if( SpecData[0] == '0' ) {
					TWA_Spec = SpecJPW;		//�i�o�v
				}
				TWA_Mark_Insp = 1;
				break;
			default:
				work1[0] = SpecData[5];
				work1[1] = 0;
				if( R_get_language() == JPN_DISP ) {
					sprintf( Comment, "���ʐݒ�G���[�@�d�l�s���H[%s]", work1  );
				} else {
					sprintf( Comment, "Special setting error specification[%s]", work1  );
				}
				R_chrdisp( 3,12, Comment );
				return(ERROR);
				break;
		}

		TWA_mark_set_m18();
	}
	//if(bIsESR25!=0 || bIsSFR25!=0){
	if(bIsESR25 || bIsSFR25){
		bIsG2Straight     = 0;
		bIsG2Separate     = 1;
		switch( SpecData[5] ) {
			case ' ':
			case '5':
			case 'C':
			case 'D':
			case 'L':
			case 'M':
			case 'U':
			case 'S':
			case 'Z':
				TWA_jpw_setting();
				TWA_R100_check();
				break;
//			case 'T':
//				TWA_Spec = SpecMCL;				//MCL
//				TWA_Insp_mode = Insp_Pinhole;	//�s���z�[������
//				break;
			case 'J':							//�p�C���b�g
				if( SpecData[4] == 'J' ) {
					TWA_Insp_level = Insp_High;		//�ڍ׌���
					Pilot_mode = ON;
#ifdef	PILOT_chip
					TWA_Insp_mode = Insp_Pilot;	//�p�C���b�g�p�`�b�v���Y���[�h
#endif
				} else {
					strcpy( Comment, "Pilot setting ERROR" );
					R_chrdisp( 3,12, Comment );
					return(ERROR);
				}
				break;
			case '.':
				TWA_Insp_mode = Insp_Pinhole;	//�s���z�[������
				break;
			case '-':					//�W�󌟍��L�����Z��
				if( SpecData[0] == '0' ) {
					TWA_Spec = SpecJPW;		//�i�o�v
				}
				TWA_Mark_Insp = 1;
				break;
			default:
				work1[0] = SpecData[5];
				work1[1] = 0;
				if( R_get_language() == JPN_DISP ) {
					sprintf( Comment, "���ʐݒ�G���[�@�d�l�s���H[%s]", work1  );
				} else {
					sprintf( Comment, "Special setting error specification[%s]", work1  );
				}
				R_chrdisp( 3,12, Comment );
				return(ERROR);
				break;
		}
		TWA_mark_set_e25();
	}
	//if(bIsLTR50!=0){
	if(bIsLTR50){
		bIsG2Straight     = 0;
		bIsG2Separate     = 1;
		switch( SpecData[5] ) {
//		  case 'P':							// <0000>
		  case ' ':							// <0000>
			TWA_Spec = SpecResin;			// �����i
			break;
//		  case 'T':
//			TWA_Insp_mode = Insp_Hyouri;	//�\������
//			break;
		  case 'J':					//Pilot 
			if( SpecData[4] == 'J' ) {
				//					TWA_Insp_level = Insp_High;		//Details inspection // JHONG
				TWA_Insp_level = Insp_Low;	    //JHONG
				Pilot_mode = ON;
			} else {
				strcpy( Comment, "Pilot setting ERROR" );
				R_chrdisp( 3,12, Comment );
				return(ERROR);
			}
			break;
		  case '-':					//�W�󌟍��L�����Z��
			TWA_Mark_Insp = 1;
			break;

		  default:
			work1[0] = SpecData[5];
			work1[1] = 0;
			if( R_get_language() == JPN_DISP ) {
				sprintf( Comment, "���ʐݒ�G���[�@�d�l�s���H[%s]", work1  );
			} else {
				sprintf( Comment, "Special setting error specification[%s]", work1  );
			}
			R_chrdisp( 3,12, Comment );
			return(ERROR);
			break;
		}
			
		TWA_mark_set_l50();
	}
	
	if(bIsLTR18){		//9499-NOTE																//v2.20
		bIsG2Straight     = 0;
		bIsG2Separate     = 1;
		switch( SpecData[5] ) {
//		  case 'P':							// <0000>
		  case ' ':							// <0000>
			TWA_Spec = SpecResin;			// �����i
			break;
//		  case 'T':
//			TWA_Insp_mode = Insp_Hyouri;	//�\������
//			break;
		  case 'J':					//Pilot 
			if( SpecData[4] == 'J' ) {
				//					TWA_Insp_level = Insp_High;		//Details inspection // JHONG
				TWA_Insp_level = Insp_Low;	    //JHONG
				Pilot_mode = ON;
			} else {
				strcpy( Comment, "Pilot setting ERROR" );
				R_chrdisp( 3,12, Comment );
				return(ERROR);
			}
			break;
		  case '-':					//�W�󌟍��L�����Z��
			TWA_Mark_Insp = 1;
			break;

		  default:
			work1[0] = SpecData[5];
			work1[1] = 0;
			if( R_get_language() == JPN_DISP ) {
				sprintf( Comment, "���ʐݒ�G���[�@�d�l�s���H[%s]", work1  );
			} else {
				sprintf( Comment, "Special setting error specification[%s]", work1  );
			}
			R_chrdisp( 3,12, Comment );
			return(ERROR);
			break;
		}
			
		TWA_mark_set_l18();
	}
	
	if(!bIsNitto){
		if( Pilot_mode == ON ) {OutputPilotOn();}
		else                   {OutputPilotOff();}
	
		R_time_start(&RTWaitTime);
		while( CheckPilotMode() == Pilot_mode ) {
			if (3000000<R_time_read(&RTWaitTime)) {
				TWA_Pilot_chk();
				break;
			}
		}
		//Automotive Spec Set   // 20170519  2013.05.14
		if( Automotive_switch == ON )  {
			R_gpio_on(  Drcy80Ch|CNa_OUT_port_1, 0x40 );
            R_time_delay( 1000000 );	//�҂�����
			R_gpio_off(  Drcy80Ch|CNa_OUT_port_1, 0x40 );
		}

		//Normal Spec Set
		if( Automotive_switch == OFF )  {
				R_gpio_on(  Drcy80Ch|CNa_OUT_port_1, 0x80 );
				R_time_delay( 1000000 );	//�҂�����
				R_gpio_off(  Drcy80Ch|CNa_OUT_port_1, 0x80 );
		}
	}

	// --- �����������t���O
	InitializeFlag = OK;
	if( Pilot_mode == ON ) {sprintf( Comment, "%s[%s %sJ]%s", "Pilot", MarkChar, DataClass[InspClass], MachineName );}
	else                   {sprintf( Comment, "%s[%s %s]%s", ProductName, MarkChar, DataClass[InspClass], MachineName );}
//	DrawMessage( Comment, DRAW_RED, 2, 0 );
	// --- insp. Type name set
	//		sprintf( Comment, "%s[%s%s]%s", ProductName, MarkChar, DataClass[InspClass], MachineName );
	res = R_CS_SetInspectionInfo(1, 1, 1, 1, 1, 1, InspectionName, Comment);
	if(res!=OK)		return ERROR;
	R_CS_DispTitle(INSPECT_MANUAL);

	if( OK != SetsubiDataLoadfile() )	return ERROR;					// �ݔ��f�[�^�ǂݍ��� 2010.06.17

	// ���b�g�J�n Date & Time
#ifdef __DOS__
	_dos_getdate(&Lot_StartDate);		// ���݂̓��t���擾
	_dos_gettime(&Lot_StartTime);		// ���݂̎��Ԃ��擾
#elif __GNUC__
	ctLotStart = time(NULL);
	memcpy(&Lot_StartDateTime , localtime(&ctLotStart) ,sizeof(Lot_StartDateTime));
#endif



	return OK;

}	// SpecSet( int nMenuNumber ) end


