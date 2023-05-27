/******************************************************************************
	R_dialog.cpp / R_dialog.h
	
	•ÏX—š—ğ
	Ver 4.01	2016.04.25	V‹K’Ç‰Á								by araki
******************************************************************************/

#ifndef R_DIALOG_H
#define R_DIALOG_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief R_dialog_de_yesno
 * @param title	Window title
 * @param text	Text
 * @retval	YES	'YES' button clicked.
 * @retval	NO	'NO' button clicked.
 */
int	R_dialog_de_yesno(const char* title, const char* text);

int	R_dialog_binarization_of_grayscale(void);

#ifdef __cplusplus
}
#endif

#endif // R_DIALOG_H
