/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2013. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1

#define  pMenu                            2
#define  pMenu_picCerrar                  2       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_cnvFondoPanel              3       /* control type: canvas, callback function: PRE_SeleccionMenu */
#define  pMenu_cnvFondo                   4       /* control type: canvas, callback function: (none) */
#define  pMenu_txtOpcion01                5       /* control type: textMsg, callback function: PRE_SeleccionMenu */
#define  pMenu_txtOpcion02                6       /* control type: textMsg, callback function: PRE_SeleccionMenu */
#define  pMenu_txtOpcion03                7       /* control type: textMsg, callback function: PRE_SeleccionMenu */
#define  pMenu_txtOpcion04                8       /* control type: textMsg, callback function: PRE_SeleccionMenu */
#define  pMenu_txtOpcion07                9       /* control type: textMsg, callback function: PRE_SeleccionMenu */
#define  pMenu_txtOpcion05                10      /* control type: textMsg, callback function: PRE_SeleccionMenu */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK PRE_SeleccionMenu(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
