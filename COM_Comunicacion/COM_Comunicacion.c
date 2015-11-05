#include "asynctmr.h"
#include <userint.h>
#include <formatio.h>
#include <tcpsupp.h>
#include <utility.h>
#include <toolbox.h>
#include "com_comunicacion.h"
#include "com_monitor.h"


//Variables globales
int iRefAppLabVIEW=0;
unsigned int HdlClienteTCP=500;
int iTimerComandos=0;
ListType lstListaComandos=0;
ListType lstListaRespuestas=0;
stEstado Estado=SIN_CONEXION;
int iNumComandos=0;
int iDriverHabilitado=0;


//Definición de prototipos
int CVICALLBACK COM_LecturaTCP (unsigned handle, int event, int error,
                             void *callbackData);
int CVICALLBACK COM_EnviarComandos (int reserved, int timerId, int event, void *callbackData, 
	int eventData1, int eventData2);


//Lanza la aplicación de comunicación con LabVIEW
int COM_IniciaAplicacion()
{
	if ((iPanelMonitor = LoadPanel (0,
									"c:\\Users\\CIDESI\\Desktop\\Sincros\\COM_Comunicacion\\COM_Monitor.uir",
									P_MONITOR)) < 0)
		return -1;
	
	DisplayPanel(iPanelMonitor);
	
	//Estado inicial de comunicacion
	EdoComunicacion.iCargandoCmd=0;
	EdoComunicacion.iEnviandoCmd=0;
	
	LaunchExecutableEx ("SINCROS_TCP.exe", LE_HIDE, &iRefAppLabVIEW);
	
	return 0;
}


//retorna el estado de la conexion con el modulo de comunicación de LabVIEW
stEstado COM_Estado()
{
	return Estado;
}


//Finaliza la ejecución de la aplicación de comunicación con LabVIEW
int COM_FinalizaAplicacion()
{
	int iTmp=0;
	int iRetardo=0;
	
	//mientras se detecta que la aplicación esta corriendo o el contador
	//de retardo sea menor a 500, esperamos a que la aplicación
	//se finalice automaticamente
	do
	{
		iTmp = ExecutableHasTerminated (iRefAppLabVIEW);
		Delay(0.01);
		iRetardo++;
	}while ((iTmp==0) && (iRetardo<500));
		
	
	//en caso de que el contador haya superado el limite y la aplicación
	//continue corriendo, se procede a destruir el proceso
	if (ExecutableHasTerminated (iRefAppLabVIEW)==0)
	{
		TerminateExecutable (iRefAppLabVIEW);
		
		if (lstListaComandos!=0)
		{
			ListDispose(lstListaComandos);
			ListDispose(lstListaRespuestas);
		}
	}
	
	HidePanel(iPanelMonitor);
	
	DiscardPanel(iPanelMonitor);
	
	return 0;
}



//Levanta el servicio de conexión con el servidor TCP
int COM_IniciaClienteTCP()
{
	int iError=0;
	
	SetBreakOnLibraryErrors (0);
	iError = ConnectToTCPServer (&HdlClienteTCP, 2055, "localhost",
								 COM_LecturaTCP, 0, 5000);
	SetBreakOnLibraryErrors (1);
	
	if (iError==0)
	{
		lstListaComandos = ListCreate(sizeof(stRegistros));
		lstListaRespuestas = ListCreate (sizeof(stRespuesta));
		
		//instala el timer asincrono para el proceso de escritura de los comandos
		iTimerComandos = NewAsyncTimer (1, -1, 1, COM_EnviarComandos, 0);
		
		Estado = CONECTADO;
	}

	SetCtrlVal (iPanelMonitor, P_MONITOR_numLed, Estado);
	
	return iError;
}



//manda un comando de forma arbitraria
int COM_ComandoUnico(char cMensaje[])
{
	
	ClientTCPWrite (HdlClienteTCP, cMensaje, 8, 1000);
	
	return 0;
}



int COM_EstructuraMensaje(unsigned char ucComando, unsigned int uiDireccion, unsigned char ucControlador,
		int iValor)
{
	unsigned int uiValor=0;
	unsigned char c[8]={0};
	
	uiValor = iValor;						//se obtiene el valor en un dato sin signo
	
	c[0]=ucComando;							//comando       
											//1 lectura
									   		//2 escritura
	c[1]=ucControlador;						//controlador
	c[2]= (uiDireccion & 0xFF00) >> 8;		//direccion parte alta
	c[3]= (uiDireccion & 0xFF);				//direccion parte baja
	c[4]= (uiValor & 0xFF000000) >> 24;		//parte alta 1 de iValor;
	c[5]= (uiValor & 0xFF0000) >> 16;		//parte alta 2 de iValor;
	c[6]= (uiValor & 0xFF00) >> 8;			//parte alta 3 de iValor;
	c[7]= (uiValor & 0xFF);					//parte baja de iValor;
			
	
	COM_InsertarComando(ucComando, c);
	
	return 0;
}




//timer que se encarga de estar enviando los comandos que se han colocado en la lista
int CVICALLBACK COM_EnviarComandos (int reserved, int timerId, int event, void *callbackData, 
	int eventData1, int eventData2)
{
	stRegistros r;
	double dTiempo=0;
	int iNumero=0;
	
	if (event == EVENT_TIMER_TICK)
	{
		if (HdlClienteTCP != 500)
		{
			if (EdoComunicacion.iCargandoCmd==0)
			{
				EdoComunicacion.iEnviandoCmd=1;
				//verifica que existan comandos en la lista para ser enviados por TCP
				iNumComandos= ListNumItems (lstListaComandos);
				iNumero = iNumComandos;
				
				if (iNumero > 0)
				{
					int i=0;
					
					while (i<iNumero)
					{
						ListGetItem (lstListaComandos, &r, i+1);
				
						SetBreakOnLibraryErrors (0);
						ClientTCPWrite (HdlClienteTCP, r.cDato, 8, 1000);
						SetBreakOnLibraryErrors (1);
						
						//Analiza el tipo de comando para determinar si se debe eliminar
						if (r.ucComando != 1)
						{
							ListRemoveItem(lstListaComandos, &r, i+1);
							iNumero--;
						}
						else
							i++;
					
					}
				}
				EdoComunicacion.iEnviandoCmd=0;
				
				//actualiza el intervalo de ejecucion de acuerdo al numero de comandos
				if (iNumComandos>0)
				{
					dTiempo = (iNumComandos*2+4)*0.060;
					SetAsyncTimerAttribute (iTimerComandos, ASYNC_ATTR_INTERVAL, dTiempo);
				}
				
			}
		}	
	}
	return 0;
}


int COM_Comandos()
{
	return iNumComandos;
}

int COM_DriverHabilitado()
{
	return iDriverHabilitado;
}


int COM_ActualizaPanel(stRespuesta r)
{
	SetCtrlVal (iPanelMonitor, P_MONITOR_numComandos, COM_Comandos());
			
	if (r.iRespuesta == 1)
	{
		if (r.Detalle.iDireccion == 10462)
		{
			if (r.Detalle.ucControlador==1)
			{
				SetCtrlVal (iPanelMonitor, P_MONITOR_numArmsP1, (double)r.Detalle.iValor/1000.0);
			}														
		
			if (r.Detalle.ucControlador==2)
			{
				SetCtrlVal (iPanelMonitor, P_MONITOR_numArmsP2, (double)r.Detalle.iValor/1000.0);
			}
		}
		
		
		if (r.Detalle.iDireccion == 10220)
		{
			if (r.Detalle.ucControlador==1)
			{
				SetCtrlVal (iPanelMonitor, P_MONITOR_numEnableE1, r.Detalle.iValor);
				iDriverHabilitado = r.Detalle.iValor;
			}														
		
			if (r.Detalle.ucControlador==2)
			{
				SetCtrlVal (iPanelMonitor, P_MONITOR_numEnableE2, r.Detalle.iValor);
				iDriverHabilitado = r.Detalle.iValor;
			}
		}
		
		if (r.Detalle.iDireccion == 10460)
		{
			if (r.Detalle.ucControlador==1)
			{
				SetCtrlVal (iPanelMonitor, P_MONITOR_numArmsN1, (double)r.Detalle.iValor/1000.0);
			}
		
			if (r.Detalle.ucControlador==2)
			{
				SetCtrlVal (iPanelMonitor, P_MONITOR_numArmsN2, (double)r.Detalle.iValor/1000.0);
			}
		}
	}
	
	if (COM_Estado()!=CONECTADO) 
	{
		SetCtrlVal (iPanelMonitor, P_MONITOR_numLed, 0);
	}
	
	return 0;
}

//solicita la siguiente respuesta disponible por parte del modulo de comunicación de LabVIEW
stRespuesta COM_Respuesta()
{
	stRespuesta r;
	int iNumero=0;
	
	//inicializa el contenido
	r.iRespuesta=-1;
	r.Detalle.iDireccion=0;
	r.Detalle.iValor=0;
	r.Detalle.ucComando=0;
	r.Detalle.ucControlador=0;
	
	//verifica que exista información para ser enviada
	iNumero = ListNumItems(lstListaRespuestas);
	
	if (iNumero>0)
	{
		ListRemoveItem (lstListaRespuestas, &r, FRONT_OF_LIST);
		
		//Actualiza el panel monitor
		COM_ActualizaPanel(r);
	}
	
	
	
	return r;
}


//Evento que captura la recepción de dato por TCP
int CVICALLBACK COM_LecturaTCP (unsigned handle, int event, int error,
                             void *callbackData)
{
	unsigned char buff[256] = {0};
	ssize_t dataSize = sizeof(buff) - 1;
	stRespuesta r;	

	switch (event)
	{
        case TCP_DATAREADY:
            if ((dataSize = ClientTCPRead (HdlClienteTCP, buff, 8, 1000))
                < 0)
            {
				//SetCtrlVal (panelHandle, MAINPNL_RECEIVE, "Receive Error\n");
            }
            else
            {
				buff[dataSize] = '\0';
				
				r.Detalle.ucComando = buff[0];
				r.Detalle.ucControlador = buff[1];
	
				r.Detalle.iDireccion = (buff[2] << 8) + buff[3];
	
				r.Detalle.iValor = (buff[4] << 24) + (buff[5] << 16) + (buff[6] << 8) + buff[7]; 

				r.iRespuesta=1;
				
				ListInsertItem (lstListaRespuestas, &r, END_OF_LIST);
            }
            break;
        case TCP_DISCONNECT:
            HdlClienteTCP = 500;
			Estado = SIN_CONEXION;
            break;
	}
    return 0;
}



//Inserta el comando a enviar al programa de comunicación con LabVIEW
int COM_InsertarComando(unsigned char ucComando, char cMensaje[])
{
	stRegistros reg;
	
	//carga la información enviada por parámetros
	reg.ucComando = ucComando;
	for (int i=0;i<8;i++)
		reg.cDato[i]=cMensaje[i];
	
	
	//Le informa al timer que envia comandos que se espere
	//en la primer oportunidad
	EdoComunicacion.iCargandoCmd=1;
	
	while (EdoComunicacion.iEnviandoCmd==1)
		ProcessSystemEvents();
	
	ListInsertItem (lstListaComandos, &reg, END_OF_LIST);
	
	
	EdoComunicacion.iCargandoCmd=0;
	
	return 0;
}







