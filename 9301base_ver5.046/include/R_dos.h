/******************************************************************************
	R_DOS.H / R_DOS.C
	
	変更履歴
	Ver 3.00	1998/04/28	正式リリース
	Ver 3.45	2005/03/30	コード定義追加
	Ver 3.46	2005/06/08	コード定義追加
	Ver 3.47	2006/02/09	コード定義追加（SHIFTKEY_～）
	Ver 3.50	2009/06/17	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_DOS_H_
#define	R_DOS_H_

#define	KEYBOARD_BACK_SPACE		(int)0x0008		// BACK SPACE
#define	KEYBOARD_TAB			(int)0x0009		// TAB

// function key
#define	KEYBOARD_F1				(int)0xFF3B		// F1
#define	KEYBOARD_F2				(int)0xFF3C		// F2
#define	KEYBOARD_F3				(int)0xFF3D		// F3
#define	KEYBOARD_F4				(int)0xFF3E		// F4
#define	KEYBOARD_F5				(int)0xFF3F		// F5
#define	KEYBOARD_F6				(int)0xFF40		// F6
#define	KEYBOARD_F7				(int)0xFF41		// F7
#define	KEYBOARD_F8				(int)0xFF42		// F8
#define	KEYBOARD_F9				(int)0xFF43		// F9
#define	KEYBOARD_F10			(int)0xFF44		// F10

#define	KEYBOARD_HOME			(int)0xFF47		// Home
#define	KEYBOARD_CURSOR_UP		(int)0xFF48		// ↑
#define	KEYBOARD_PAGE_UP		(int)0xFF49		// Page Up
#define	KEYBOARD_CURSOR_LEFT	(int)0xFF4B		// ←
#define	KEYBOARD_CURSOR_RIGHT	(int)0xFF4D		// →
#define	KEYBOARD_END			(int)0xFF4F		// End
#define	KEYBOARD_CURSOR_DOWN	(int)0xFF50		// ↓
#define	KEYBOARD_PAGE_DOWN		(int)0xFF51		// Page Down
#define	KEYBOARD_INSERT			(int)0xFF52		// Insert
#define	KEYBOARD_DELETE			(int)0xFF53		// Delete

// shift + function key
#define	KEYBOARD_F1_SHIFT		(int)0xFF54		// SHIFT + F1
#define	KEYBOARD_F2_SHIFT		(int)0xFF55		// SHIFT + F2
#define	KEYBOARD_F3_SHIFT		(int)0xFF56		// SHIFT + F3
#define	KEYBOARD_F4_SHIFT		(int)0xFF57		// SHIFT + F4
#define	KEYBOARD_F5_SHIFT		(int)0xFF58		// SHIFT + F5
#define	KEYBOARD_F6_SHIFT		(int)0xFF59		// SHIFT + F6
#define	KEYBOARD_F7_SHIFT		(int)0xFF5A		// SHIFT + F7
#define	KEYBOARD_F8_SHIFT		(int)0xFF5B		// SHIFT + F8
#define	KEYBOARD_F9_SHIFT		(int)0xFF5C		// SHIFT + F9
#define	KEYBOARD_F10_SHIFT		(int)0xFF5D		// SHIFT + F10

// control + function key
#define	KEYBOARD_F1_CTRL		(int)0xFF5E		// CTRL + F1
#define	KEYBOARD_F2_CTRL		(int)0xFF5F		// CTRL + F2
#define	KEYBOARD_F3_CTRL		(int)0xFF60		// CTRL + F3
#define	KEYBOARD_F4_CTRL		(int)0xFF61		// CTRL + F4
#define	KEYBOARD_F5_CTRL		(int)0xFF62		// CTRL + F5
#define	KEYBOARD_F6_CTRL		(int)0xFF63		// CTRL + F6
#define	KEYBOARD_F7_CTRL		(int)0xFF64		// CTRL + F7
#define	KEYBOARD_F8_CTRL		(int)0xFF65		// CTRL + F8
#define	KEYBOARD_F9_CTRL		(int)0xFF66		// CTRL + F9
#define	KEYBOARD_F10_CTRL		(int)0xFF67		// CTRL + F10

// alternate + function key
#define	KEYBOARD_F1_ALT			(int)0xFF68		// ALT + F1
#define	KEYBOARD_F2_ALT			(int)0xFF69		// ALT + F2
#define	KEYBOARD_F3_ALT			(int)0xFF6A		// ALT + F3
#define	KEYBOARD_F4_ALT			(int)0xFF6B		// ALT + F4
#define	KEYBOARD_F5_ALT			(int)0xFF6C		// ALT + F5
#define	KEYBOARD_F6_ALT			(int)0xFF6D		// ALT + F6
#define	KEYBOARD_F7_ALT			(int)0xFF6E		// ALT + F7
#define	KEYBOARD_F8_ALT			(int)0xFF6F		// ALT + F8
#define	KEYBOARD_F9_ALT			(int)0xFF70		// ALT + F9
#define	KEYBOARD_F10_ALT		(int)0xFF71		// ALT + F10

#define	SHIFTKEY_RIGHT			(int)0x1
#define	SHIFTKEY_LEFT			(int)0x2
#define	SHIFTKEY_BOTH			(int)0x3

int		R_dos_getch(void);
int		R_dos_shift_key_is_pushed(void);

#endif	// R_DOS_H_
