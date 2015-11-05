/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Industrial
. Archivo:					PRE_Principal.c
. Propósito:				Entrada a la aplicacion.
. Lenguaje:					LabWindows/CVI 13.0.0 (632)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		31 de Marzo de 2014
. Responsable modificación: 
.
*******************************************************************************/
#include <NIDAQmx.h>
#include <analysis.h>
#include "PRE_Principal.h"
#include <cvirte.h>		
#include <userint.h>
#include "pre_variables.h"


/*****************************************************************************
.
. Función C:			main
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Resuelve si el panel indicado es visible
. Parámetro de entrada:	int iPanel | Referencia del panel
. Parámetro de salida:	Retorna el estado del panel
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int main (int argc, char *argv[])
{
	//carga las referencias de todos los paneles
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((iPanelPrincipal = LoadPanel (0, "PRE_Principal.uir", pPrincipal)) < 0)
		return -1;
	
	if ((iPanelMenuPrincipal = LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pMenu)) < 0)
		return -1;
	
	if ((iPanelCatTransmisiones = LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pCatTransm)) < 0)
		return -1;
	
	if ((iPanelEntrenamiento= LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pEntrenar)) < 0)
		return -1;
	
	PRE_InicializarInterfaz();
	PRE_InicializarSistema();
	
	DisplayPanel (iPanelPrincipal);
	
	RunUserInterface ();
	DiscardPanel (iPanelPrincipal);
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_Principal
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Captura los eventos que modifican la ventana principal
. Parámetro de entrada:	los de una funcion callback
. Parámetro de salida:	Retorna valor 0
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRE_Principal (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			PRE_FinalizarAplicacion();
			break;
			
		case EVENT_PANEL_SIZING:
			PRE_AjustarComponentes(eventData2);
			break;
		case EVENT_PANEL_SIZE:
			PRE_AjustarComponentes(eventData2);
			break;
			
		case EVENT_LEFT_CLICK:
			PRE_AnimacionMenu(OCULTAR);
			break;
	}
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_MostrarMenu
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Despliega el menu en la pantalla principal
. Parámetro de entrada:	los de una funcion callback
. Parámetro de salida:	Retorna valor 0
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int CVICALLBACK PRE_MostrarMenu (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			PRE_ConfiguraMenu();
			break;
			
		case EVENT_MOUSE_POINTER_MOVE:
			PRE_BarraEstado("Despliega el menú principal del sistema.");
			break;
	}
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_FinalizarAplicacion
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Finaliza la aplicación en ejecución
. Parámetro de entrada:	los de una funcion callback
. Parámetro de salida:	Retorna valor 0
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int PRE_FinalizarAplicacion()
{
	if (ConfirmPopup ("Finalizar sesión con el sistema",
				  "¿Desea cerrar la aplicación?")==1)
	{
		PRE_Finalizar();
		QuitUserInterface (0);	
		
	}   
	return 0;
}




/*****************************************************************************
	ELIMINAR ESTE CODIGO INICIANDO AQUI..... 

*/


int CVICALLBACK crearArchivo (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			//carga los datos de inicio del archivo tdms
			if (TDM_DatosArchivo("c:\\tdms","sincros_","Alguna descripcion aqui","Un titulo","Autor","Nombre")==TDMS_Error)
			{
				MessagePopup ("Error TDMS", TDM_MensajeError());
			}
			
			if (TDM_Crear(1,"Canal 01", TDM_Doble)==TDMS_Error)
			{
				MessagePopup ("Error TDMS", TDM_MensajeError());
			}
			
			//ejemplo de insertar informacion usando una senoidal
			/*double d1[1000]={0};
							 
			SinePattern (1000, 1.0, 0.0, 3, d1);
			TDM_Insertar(1000, d1);
			TDM_Cerrar();
			*/
			break;
	}
	return 0;
}


int32 CVICALLBACK guardarDatos2 (TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{				  
	double dDatos[1000]={0};
	int32 datosLeidos=0;
	
	//inserta los datos
	DAQmxReadAnalogF64 (miTarea, 1000, 10.0, DAQmx_Val_GroupByChannel,
						dDatos, 1000, &datosLeidos, 0);
	
	TDM_Insertar(datosLeidos, dDatos);

	return 0;
}



int CVICALLBACK iniciarAdquisicion (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DAQmxLoadTask ("NuevaCeldaCarga", &miTarea);
			DAQmxRegisterEveryNSamplesEvent (miTarea,
											 DAQmx_Val_Acquired_Into_Buffer, 1000,
											 0, guardarDatos2, NULL);
			DAQmxStartTask (miTarea);
			break;
	}
	return 0;
}


int CVICALLBACK detener (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DAQmxStopTask (miTarea);
			DAQmxClearTask (miTarea);
			TDM_Cerrar(); 
			
			break;
	}
	return 0;
}



/*
	.... Y TERMINANDO AQUI 

*****************************************************************************/


