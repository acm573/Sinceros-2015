 /****************************************************************************
.MODULO			GRA_Genelares.c
.DESCRIPCION	Contiene las funciones de uso general por parte del sistema. 
.				Estas funciones se refieren a concatenación de cadenas, a
.				conversiones de datos, etc.
.AUTOR			Cesar Armando Cruz Mendoza
.LENGUAJE		C
.FECHA INICIO	29-MAYO-2008
*****************************************************************************/


/*--------------------------------------------------------------------------*/
/*	Archivos include                                                        */
/*--------------------------------------------------------------------------*/
#include <utility.h>
#include <formatio.h>
#include "toolbox.h"
#include <cvirte.h>		
#include <userint.h>


//variables globales
int iPanelModal=0;


/*--------------------------------------------------------------------------*/
/*	Prototipo de funciones                                                  */
/*--------------------------------------------------------------------------*/
int MA_CapturaDescripcion(void);



/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_XYSubmenu
. 	Descripcion: Ajusta la posición del submenu a las coordenadas que se hayan
.				 detectado del cursor del mouse.
.	Argumentos:	 int panel | Referencia del panel donde se encuentra el
.							 control
.				 int submenu | Referencia del panel donde se encuentre el
.							   submenu que se desea mostrar
.   Retorno:	 Nada
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
void GRA_XYSubmenu(int panel, int submenu)
{
	int panelX;
	int panelY;
	
	//Obtiene las coordenadas del cursor
	GetRelativeMouseState(panel,0,&panelX,&panelY,NULL,NULL,NULL);
	SetPanelAttribute (submenu, ATTR_LEFT, panelX-10);
	SetPanelAttribute (submenu, ATTR_TOP, panelY+10);
	
}





/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_ColorMenuEdo
. 	Descripcion: Fija el estado de seleccion de un elemento del menu
.	Argumentos:	 int panel | Referencia del panel donde se encuentra el
.							 control
.				 int control | Elemento del submenu que se ha seleccionado
.				 int estado | TRUE Activa la opción indicada
.							  FALSE Desactiva la opción indicada
.   Retorno:	 Nada
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
void GRA_ColorMenuEdo(int panel, int control, int estado)
{
	int color1;
	int color2;
	
	if (estado == FALSE)
	{
		color1 = 13947080;
		color2 = VAL_BLACK;
	}
	else
	{
		color1 = 153;
		color2 = VAL_WHITE;
	}
	
	SetCtrlAttribute (panel, control, ATTR_TEXT_BGCOLOR, color1);
	SetCtrlAttribute (panel, control, ATTR_TEXT_COLOR, color2);
}




/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_ColorMenu
. 	Descripcion: Resalta la opción de que se ha seleccionado en un submenu del
.				 sistema
.	Argumentos:	 int panel | Referencia del panel donde se encuentra el
.							 control
.				 int control | Elemento del submenu que se ha seleccionado
.   Retorno:	 Nada
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
int GRA_NoItems(int panel,int control)
{
	int iNoItems;
	
	GetNumListItems (panel, control, &iNoItems);
	
	return iNoItems;
}









/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_Etiqueta
. 	Descripcion: Retorna la etiqueta asociada al control tipo lista en el
.				 indice que actualmente ha seleccionado el usuario en la 
.				 pantalla.
.	Argumentos:	 int panel | Referencia al panel donde se encuentra localizado
.							 el control
.				 int control | Referencia al control que contiene la lista
.				 char *pcEtiqueta | Vector en el cual se retorna la etiqueta
.									que se obtiene del control
.   Retorno:	 Nada
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
int GRA_Etiqueta(int panel, int control, char *pcEtiqueta)
{
	int iIndice=0;
	int iNoItems=0;
	
	GetNumListItems (panel, control, &iNoItems);
	
	if (iNoItems==0)
		return -1;

	GetCtrlIndex(panel, control, &iIndice);
	GetLabelFromIndex (panel, control, iIndice, pcEtiqueta);
	
	return 0;
}




/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_Pausa
. 	Descripcion: Implementa una pausa asociada al ProcessSystemEvents con la
.				 finalidad de no perder respuesta al panel frontal del usuario
.	Argumentos:	 double dTiempo | Valor en segundos que se desea implementar  
.							 	  como pausa en el sistema.
.   Retorno:	 Cero
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
int GRA_Pausa(double dTiempo)
{
	double dT1;
	double dT2;
	
	dT1=Timer();
	dT2=Timer();
	
	while ((dT2-dT1)<dTiempo)
	{
		dT2 = Timer();
		ProcessSystemEvents();
	}
	return 0;
}





/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_Items
. 	Descripcion: Retorna el número de elementos que existen en una lista del
.				 control definido en el argumento.
.	Argumentos:	 int panel | Referencia al panel donde se encuentra el control
.				 int control | Referncia al control tipo lista
.   Retorno:	 iNumero con el número de elementos de la lista
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
int GRA_Items(int panel, int control)
{
	int iNumero=0;
	
	GetNumListItems (panel, control, &iNumero);
	
	return iNumero;
}




/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_Dimmed
. 	Descripcion: Habilita/Deshabilita el control especificado
.	Argumentos:	 int panel | Referencia al panel donde se encuentra el control
.				 int control | Referncia al control tipo lista
.				 int estado | TRUE / FALSE
.   Retorno:	 Cero
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
/*
int GRA_Dimmed(int panel, int control, int estado)
{
	SetCtrlAttribute (panel, control, ATTR_DIMMED, estado);
	
	return 0;
}
*/


/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_Oculta
. 	Descripcion: Muestra / Oculta el control especificado
.	Argumentos:	 int panel | Referencia al panel donde se encuentra el control
.				 int control | Referncia al control tipo lista
.				 int estado | TRUE / FALSE
.   Retorno:	 Cero
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
int GRA_Oculta(int panel, int control, int estado)
{
	SetCtrlAttribute (panel, control, ATTR_VISIBLE, !estado);
	return 0;
}



/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_Strcat
. 	Descripcion: Concatena todos los elementos que se pasaron como argumento a
.				 la función. Esto aplica únicamente para datos tipo string.
.	Argumentos:	 int iNoElementos | Contiene el número de elementos tipo
.									string que se han enviado.
.   Retorno:	 Apuntador tipo string
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
char *GRA_Strcat(int iNoElementos, ...)
{
	va_list pa;
	char	*pcCadena;
	static char	cSalida[6000];
	int		i=0;
	
	
	FillBytes(cSalida,0,6000,0);
	va_start(pa, iNoElementos);
	
	for (i=0;i<iNoElementos;i++)
	{
		pcCadena = va_arg(pa,char*);
		strcat(cSalida,pcCadena);
	}
	return cSalida;
}




/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
.	Función: 	 GRA_IntStr
. 	Descripcion: Convierte un valor entero a cadena y retorna el string
.	Argumentos:	 int iValor | Valor a convertir a cadena
.   Retorno:	 Apuntador tipo string
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
char *GRA_IntStr(int iValor)
{
	static char	cCadena[260]={0};

	Fmt(cCadena,"%s<%d",iValor);

	return cCadena;
}




int GRA_PanelModal(int iPanel)
{
	iPanelModal = iPanel;
	return 0;
}


int GRA_Dimmed(int iControl, int iModo)
{
	SetCtrlAttribute(iPanelModal, iControl, ATTR_DIMMED, iModo);
	return 0;
}


int GRA_Visible(int iControl, int iModo)
{
	SetCtrlAttribute(iPanelModal, iControl, ATTR_VISIBLE, iModo);
	return 0;
}
