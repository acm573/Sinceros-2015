/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2014. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: PRI_Salir */
#define  PANEL_btnCierraArchivo_2         2       /* control type: command, callback function: PRI_CierraTDMS */
#define  PANEL_btnCierraArchivo           3       /* control type: command, callback function: PRI_CierraTDMS */
#define  PANEL_btnInsertarDatos           4       /* control type: command, callback function: PRI_InsertaDatos */
#define  PANEL_btnArchivoCreado           5       /* control type: command, callback function: PRI_ArchivoTDMS */
#define  PANEL_btnCrearTDMS               6       /* control type: command, callback function: PRI_CrearTDMS */
#define  PANEL_btnSeleccionArchivo        7       /* control type: command, callback function: PRI_SeleccionArchivoTDMS */
#define  PANEL_btnDatosArchivo            8       /* control type: command, callback function: PRI_DatosArchivoTDMS */
#define  PANEL_strArchivoLeer             9       /* control type: string, callback function: (none) */
#define  PANEL_strNombreArchivo           10      /* control type: string, callback function: (none) */
#define  PANEL_TEXTMSG                    11      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  12      /* control type: textMsg, callback function: (none) */
#define  PANEL_graGrafica4                13      /* control type: graph, callback function: (none) */
#define  PANEL_graGrafica3                14      /* control type: graph, callback function: (none) */
#define  PANEL_graGrafica2                15      /* control type: graph, callback function: (none) */
#define  PANEL_graGrafica1                16      /* control type: graph, callback function: (none) */
#define  PANEL_numBarra                   17      /* control type: scale, callback function: PRI_Desplazamiento */
#define  PANEL_numDatos                   18      /* control type: numeric, callback function: (none) */
#define  PANEL_TEXTMSG_3                  19      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK PRI_ArchivoTDMS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRI_CierraTDMS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRI_CrearTDMS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRI_DatosArchivoTDMS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRI_Desplazamiento(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRI_InsertaDatos(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRI_Salir(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRI_SeleccionArchivoTDMS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
