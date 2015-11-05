/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Nombre del Aplicativo:	Módulo con funciones comunes diversas
. Archivo:					UTL_Utilerias.c
. Propósito:				Contiene diversas funciones para la ejecución de
.							rutinas comunes en la aplicación
. Lenguaje:					LabWindows/CVI 10.0.1 (434)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		18 de Febrero de 2013
. Responsable modificación: 
.
*******************************************************************************/

//Includes
#include "toolbox.h"
#include "regexpr.h"
#include <ansi_c.h>
#include <formatio.h>



/*****************************************************************************
.
. Función C:			UTL_Split 
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Separa una cadena en N partes utilizando un patron de
.						texto como separador
. Parámetro de entrada:	char *pcCadena | contiene la cadena a analizar
.						char *pcPatron | cadena que se utilizará como separador
.						char *c[] | arreglo donde se colocarán los elementos
.									que se han separado
.						int *total | referencia en donde se almacenará el total
.									 del número de elementos que fueron 
									 encontrados en la cadena original
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
void * UTL_Split (char *pcCadena, char *pcPatron, char (*c)[30], int *total)
{
    int iResultado;
	int iLongitud;
	int iPosicion;
	int iEncontrado;
	char cCadena[260]={0};
	int gMatchPosition=0;
	ListType miLista;
	int iIndice=0;
	
	
	miLista = ListCreate (sizeof(cCadena));
	iResultado=0;
	while (iResultado==0)
	{
		iResultado = RegExpr_FindPatternInText (pcPatron, 0, pcCadena+gMatchPosition,
												(int)strlen(pcCadena+gMatchPosition),
												RegExpr_SearchForwards,
												RegExpr_MatchLargestNumChars,
												&iEncontrado, &iPosicion,
												&iLongitud);
		
		CopyString (cCadena, 0, pcCadena+gMatchPosition, 0, iPosicion);
		
		if (strlen(cCadena)!=0)
		{
			gMatchPosition += iPosicion+iLongitud;
			Fmt(c[iIndice],"%s<%s",cCadena);
			iIndice++;
			*total = iIndice;
		}
		else
			iResultado=1;
	}
	
	return 0;
}




/*****************************************************************************
.
. Función C:			UTIL_Wait 
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Implementa una pausa
. Parámetro de entrada:	double delay | valor double de retardo en segundos
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int UTIL_Wait (double delay) 
{
	static double time1;
	static double time2;
	
	time1 = Timer();
	time2 = Timer();
	
	while ((time2-time1) < delay)
	{
		time2 = Timer();
		ProcessSystemEvents();
	}
	
	return 0;
}
