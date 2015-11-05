/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Nombre del Aplicativo:	Módulo que controla el desplegado de información
.							en el canvas de la aplicación
. Archivo:					CAN_DespliegaInformacion.c
. Propósito:				Contiene las funciones que se encargan de desplegar
.							cada uno de los diferentes tipos de información
.							sobre el canvas de trabajo
. Lenguaje:					LabWindows/CVI 10.0.1 (434)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		18 de Febrero de 2013
. Responsable modificación: 
.
*******************************************************************************/

//include
#include "toolbox.h"
#include <userint.h>


//Variables gloables locales
int iPanelCanvas=0;
int iControlCanvas=0;



/*****************************************************************************
.
. Función C:			CAN_InfoControl
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Recibe las referencias del panel y control canvas
. Parámetro de entrada:	int iPanel | referencia del panel donde existe el control
.						int iControl | referencia del control canvas
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int CAN_InfoControl(int iPanel, int iControl)
{
	iPanelCanvas = iPanel;
	iControlCanvas = iControl;
	return 0;
}



/*****************************************************************************
.
. Función C:			CAN_InfoControl
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Dibuja la marca solicitada en el canvas del panel
. Parámetro de entrada:	int x | coordenada x de la marca
.						int y | coordenada y de la marca
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int CAN_DibujarMarca(int x, int y, int color)
{
	SetCtrlAttribute (iPanelCanvas, iControlCanvas, ATTR_PEN_FILL_COLOR,
					  color);
	
	CanvasDrawOval (iPanelCanvas, iControlCanvas, MakeRect (y, x, 30, 30),
					VAL_DRAW_INTERIOR);
	
	/*
	VAL_LT_GRAY
    VAL_DK_YELLOW
    VAL_OFFWHITE
    VAL_DK_MAGENTA
    VAL_DK_CYAN
    VAL_DK_GREEN
    VAL_DK_RED
    VAL_MAGENTA
    VAL_GREEN
    VAL_BLUE
    VAL_CYAN
    VAL_RED
    VAL_YELLOW
    VAL_DK_BLUE
    VAL_BLACK
    VAL_GRAY
    VAL_TRANSPARENT
    VAL_PANEL_GRAY
    VAL_DK_GRAY
    VAL_WHITE
*/
	
	return 0;
}



int CAN_DibujarMarcaRecorrido(int x, int y, int color)
{
	SetCtrlAttribute (iPanelCanvas, iControlCanvas, ATTR_PEN_FILL_COLOR,
					  color);
	
	CanvasDrawOval (iPanelCanvas, iControlCanvas, MakeRect (y, x, 5, 5),
					VAL_DRAW_INTERIOR);
	
	/*
	VAL_LT_GRAY
    VAL_DK_YELLOW
    VAL_OFFWHITE
    VAL_DK_MAGENTA
    VAL_DK_CYAN
    VAL_DK_GREEN
    VAL_DK_RED
    VAL_MAGENTA
    VAL_GREEN
    VAL_BLUE
    VAL_CYAN
    VAL_RED
    VAL_YELLOW
    VAL_DK_BLUE
    VAL_BLACK
    VAL_GRAY
    VAL_TRANSPARENT
    VAL_PANEL_GRAY
    VAL_DK_GRAY
    VAL_WHITE
*/
	
	return 0;
}



/*****************************************************************************
.
. Función C:			CAN_DibujarEtiqueta
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Dibuja la leyenda en la posicion indicada
. Parámetro de entrada:	int x | coordenada x de la marca
.						int y | coordenada y de la marca
.						char *pcCadena | cadena que se desea dibujar
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int CAN_DibujarEtiqueta(int x, int y, char *pcCadena, int color)
{
	SetCtrlAttribute (iPanelCanvas, iControlCanvas, ATTR_PEN_COLOR,
					  color);
	SetCtrlAttribute (iPanelCanvas, iControlCanvas, ATTR_PEN_FILL_COLOR,
					  VAL_TRANSPARENT);
	
	CreateMetaFont ("Sincros01", VAL_APP_META_FONT, 14, 1, 0, 0, 0);
	
	CanvasDrawText (iPanelCanvas, iControlCanvas, pcCadena,
					"Sincros01", MakeRect (y+5, x+5, 20, 20), VAL_CENTER);
	
	return 0;
}
