/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Industrial
. Archivo:					PRI_Principal.c
. Propósito:				Funciones para mostrar el funcionamiento de 
.							TDM_Archivos.c
. Lenguaje:					LabWindows/CVI 13.0.0 (632)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		31 de Marzo de 2014
. Responsable modificación: 
.
*******************************************************************************/
#include <analysis.h>
#include "toolbox.h"
#include <cvirte.h>		
#include <userint.h>
#include "PRI_Principal.h"
#include "tdm_archivos.h"

//definiciones
#define BLOQUE_ESCRIBE 1000		//simula la cantidad de datos por canal a escribir
#define BLOQUE_LECTURA 100		//simula la cantidad de datos a leer de archivo

//globales
static int iPrincipal;		//referencia al panel principal
int iDatosInsertados=0;		//indicador de numero de datos insertados a TDMS


/*****************************************************************************
.
. Función C:			main
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Funcion de entrada a la aplicación
. Parámetro de entrada:	int argc | numero de argumentos por linea de comando
.						char *argv | arreglo de cadenas con los parametros
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((iPrincipal = LoadPanel (0, "PRI_Principal.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (iPrincipal);
	RunUserInterface ();
	DiscardPanel (iPrincipal);
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_Salir
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Cierra la aplicación
. Parámetro de entrada:	Los de una funcion callback
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRI_Salir (int panel, int event, void *callbackData,
						   int eventData1, int eventData2)
{
	if (event==EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_DatosArchivoTDMS
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Muestra la forma de configurar la información para
.						un archivo TDMS y el manejo de errores de la funcion.
. Parámetro de entrada:	Los de una funcion callback
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRI_DatosArchivoTDMS (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	if (event==EVENT_COMMIT)
	{
		//Se invoca al método que permite configurar la información que se desee
		//adjuntar al archivo TDMS
		
		//La lista de parámetros es:
		//Primero: Path donde se ubicará el archivo
		//Segundo: Prefijo que se desee poner a los archivos TDMS			
		//Tercero: Descripción para el archivo (opcional)
		//Cuarto: Título para el archivo (opcional)
		//Quinto: Nombre del autor del archivo (opcional)
		
		if (TDM_DatosArchivo("c:\\tdms","prefijo_","Alguna descripcion aqui","Un titulo","Autor","Nombre")==TDMS_Error)
		{
			MessagePopup ("Error TDMS", TDM_MensajeError());
		}
	}
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_CrearTDMS
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Muestra el proceso para crear un archivo TDMS, en donde
.						se configuren los canales que se van a requerir, así
.						como su manejo de errores.
. Parámetro de entrada:	Los de una funcion callback
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRI_CrearTDMS (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	if (event==EVENT_COMMIT)
	{
		//Se define el numero de canales que se van a utilizar, seguido de
		//las parejas en las que se indican titulo del canal  y el tipo
		//de dato que se espera recibir
		
		//Los tipos disponibles son:  TDM_Doble, TDM_Entero, TDM_Flotante
		if (TDM_Crear(4,
				  	"Canal 01", TDM_Doble,
					"Canal 02", TDM_Doble,
					"Titulo 3", TDM_Doble, 
					"Titulo 4", TDM_Doble)==TDMS_Error)
		{
			MessagePopup ("Error TDMS", TDM_MensajeError());
		}
		iDatosInsertados=0;
		
		
		/*
		Por ejemplo, si se desea configurar para un archivo de 3 canales dobles,
		la llamada a la funcion queda:
		
		TDM_Crear(3,"Tit 1", TDM_Doble,"Tot 2", TDM_Doble, "Tit 3", TDM_Doble);
		*/
	}
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_ArchivoTDMS
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Muestra como se puede obtener el nombre del archivo
.						tdms que se ha creado.
. Parámetro de entrada:	Los de una funcion callback
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRI_ArchivoTDMS (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	char cNombre[260]={0};
					   
	if (event==EVENT_COMMIT)
	{
		//Obtiene el nombre del archivo TDMS que se ha creado
		TDM_NombreArchivo(cNombre);
		SetCtrlVal (iPrincipal, PANEL_strNombreArchivo, cNombre);
	}
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_InsertaDatos
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Ejemplifica el proceso para insertar información en el
.						archivo TDMS, utilizando como ejemplo algunos arreglos
.						con patrones de señales simuladas.
. Parámetro de entrada:	Los de una funcion callback
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRI_InsertaDatos (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	double d1[BLOQUE_ESCRIBE];
	double d2[BLOQUE_ESCRIBE];
	double d3[BLOQUE_ESCRIBE];
	double d4[BLOQUE_ESCRIBE];

	if (event == EVENT_COMMIT)
	{
		//simula señales
		SinePattern (BLOQUE_ESCRIBE, 1.0, 0.0, 3, d1);
		SinePattern (BLOQUE_ESCRIBE, 2.0, 0.0, 10, d2);
		SinePattern (BLOQUE_ESCRIBE, .5, 0.0, 20, d3);
		SinePattern (BLOQUE_ESCRIBE, 10, 0.0, 15, d4);
		//inserta la información en el archivo TDMS
		//deben ir los vectores separados por coma, en el mismo orden en que
		//se configuró el archivo
		TDM_Insertar(BLOQUE_ESCRIBE, d1,d2,d3,d4);
		
		/*
		Por ejemplo, si se deseara insertar una cantidad de 3 canales, la llamada
		a la funcion queda:
		
		TDM_Insertar(BLOQUE_ESCRIBE, d1,d2,d3);
		
		El orden de los arreglos, debe ser en el mismo orden en que se creo el 
		archivo TDMS.
		
		*/
		
		iDatosInsertados+=BLOQUE_ESCRIBE;
		SetCtrlVal(iPrincipal, PANEL_numDatos, iDatosInsertados);
	}
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_CierraTDMS
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Muestra el proceso para cerrar el archivo TDMS
. Parámetro de entrada:	Los de una funcion callback
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRI_CierraTDMS (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	if (event == EVENT_COMMIT)
	{
		//cierra el archivo TDMS 
		TDM_Cerrar();
	}
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_LimpiarGraficas
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Limpia el contenido de las graficas de la pantalla
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int PRI_LimpiarGraficas()
{
	DeleteGraphPlot (iPrincipal, PANEL_graGrafica1, -1, VAL_IMMEDIATE_DRAW);
	DeleteGraphPlot (iPrincipal, PANEL_graGrafica2, -1, VAL_IMMEDIATE_DRAW);
	DeleteGraphPlot (iPrincipal, PANEL_graGrafica3, -1, VAL_IMMEDIATE_DRAW);
	DeleteGraphPlot (iPrincipal, PANEL_graGrafica4, -1, VAL_IMMEDIATE_DRAW);
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_SeleccionArchivoTDMS
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Muestra el proceso para abrir un archivo TDMS y 
.						obtener el numero de datos totales que hay disponible
.						en el archivo para su graficación.
. Parámetro de entrada:	Los de una funcion callback
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRI_SeleccionArchivoTDMS (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char cNombreArchivo[260]={0};
	unsigned int iNumero;
	
	switch (event)
	{
		case EVENT_COMMIT:
			if (FileSelectPopup ("", "*.tdms", "*.tdms", "Seleccionar un archivo...",
							 VAL_SELECT_BUTTON, 0, 0, 1, 0, cNombreArchivo)==1)
			{
				SetCtrlVal(iPrincipal, PANEL_strArchivoLeer, cNombreArchivo);
				
				PRI_LimpiarGraficas();
				//Abre el archivo TDMS indicado, seguido de una variable en donde
				//se retornara el numero de datos totales por canal.
				TDM_Abrir(cNombreArchivo, &iNumero);
				
				SetCtrlAttribute (iPrincipal, PANEL_numBarra, ATTR_MAX_VALUE, iNumero-BLOQUE_LECTURA);
				SetCtrlVal (iPrincipal, PANEL_numBarra, 0);
			}
			break;
	}
	return 0;
}

/*****************************************************************************
.
. Función C:			PRI_Desplazamiento
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Muestra el proceso para leer la información del TDMS
.						y utilizarla para graficación.
. Parámetro de entrada:	Los de una funcion callback
. Parámetro de salida:	Valor de cero
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRI_Desplazamiento (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	double d1[BLOQUE_LECTURA];
	double d2[BLOQUE_LECTURA];
	double d3[BLOQUE_LECTURA];
	double d4[BLOQUE_LECTURA];
	unsigned int iIndice;
	
	
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			PRI_LimpiarGraficas();
				
			
			GetCtrlVal(iPrincipal, PANEL_numBarra, &iIndice);
			
			
			//Obtiene del archivo TDMS la cantidad de datos marcada en el primer parámetro,
			//seguido del indice que define la barra de desplazamiento y la lista
			//de arreglos en donde se colocará la información de cada canal
			if (TDM_Leer(BLOQUE_LECTURA, iIndice, d1,d2,d3,d4) == TDMS_Correcto)
			{
			
				PlotY (iPrincipal, PANEL_graGrafica1, d1, BLOQUE_LECTURA, VAL_DOUBLE,
					   VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
				PlotY (iPrincipal, PANEL_graGrafica2, d2, BLOQUE_LECTURA, VAL_DOUBLE,
					   VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
				PlotY (iPrincipal, PANEL_graGrafica3, d3, BLOQUE_LECTURA, VAL_DOUBLE,
					   VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
				PlotY (iPrincipal, PANEL_graGrafica4, d4, BLOQUE_LECTURA, VAL_DOUBLE,
					   VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
	}
	return 0;
}


