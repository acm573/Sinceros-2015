#include <ansi_c.h>
#include "mov_movimiento.h"
#include <utility.h>
#include <userint.h>
#include "asynctmr.h"
#include <NIDAQmx.h>

#define CON_TAREA	1
#define SIN_TAREA 	0

TaskHandle misDigitales; 
int iTareaCreada=0;		//indica si la tarea ya se encuentra en ejecucion
int iTimerDigital=0;
int iPanel, iControl;
int EstadoEntradas[32]={0};
double TiempoAnterior[32]={0};
double TiempoEntrada[32]={0};

int iConfigurarEntrada[32]={0};			//0 igual a usa contador    1 solo detecta cambio estado
int iEstadoAnterior[32]={0};
int iEstadoActual[32]={0};
int iContador[32]={0};
int (*funcion[32])(stNumeroBit, stEstadoBit);



double DIG_Tiempo(int iBit)
{
	return TiempoEntrada[iBit];
}


int DIG_EstadoEntrada(int iEntrada)
{
	return iEstadoActual[iEntrada];
}



int DIG_Separar(unsigned int iValor)
{
	int iEstado=0;
	int iBit=1;
	
	for (int i=0;i<32;i++)
	{
		iEstado = (iValor & (iBit << i)) >> i;

		//Analiza si el iEstado es diferente al actual
		
		if (iEstado != iEstadoActual[i])
		{
			//Almacena el estado actual como el anterior
			iEstadoAnterior[i] = iEstadoActual[i];
		
			//Actualiza el estado actual
			iEstadoActual[i] = iEstado;	
		}
		
		
		// si mi transision es de  OFF a ON, entonces se activa el contador
		// de tiempo
		if ((iContador[i] == OFF) && ((iEstadoAnterior[i] == OFF) && (iEstadoActual[i] == ON)))
		{
			
			iContador[i] = ON;
			TiempoAnterior[i]=Timer();
			TiempoEntrada[i]=0.0;
			
			if (iConfigurarEntrada[i]==1)
			{
				if (funcion[i]!=NULL)
					funcion[i](i,ACTIVADO);
			}
		}
		
		
		if ((iContador[i] == ON) && ((iEstadoAnterior[i] == ON) && (iEstadoActual[i] == OFF)))
		{
			if (iConfigurarEntrada[i]==0)
			{
				if (TiempoEntrada[i] < 0.5)
				{
					if (funcion[i]!=NULL)
						funcion[i](i,PULSO_CORTO);
				}
				else
				{
					if (funcion[i]!=NULL)
						funcion[i](i,PULSO_LARGO);
				}
				
				iContador[i]=OFF;
			}
			else
			{
				if (funcion[i]!=NULL)
					funcion[i](i,DESACTIVADO);
				
				iContador[i]=OFF;
			}
			
		}
		
		
		
		if (iContador[i] == ON)
		{
			TiempoEntrada[i] += Timer() - TiempoAnterior[i];
			TiempoAnterior[i]=Timer();
		}
	}
	
	
	
	
	return 0;
}

int DIG_InicializaVectores()
{
	for (int i=0;i<32;i++)
	{
		iEstadoAnterior[i]=OFF;
		iEstadoActual[i]=OFF;
		iContador[i]=OFF;
		funcion[i]=NULL;
	}
	
	//comportamiento de las entradas
	iConfigurarEntrada[4]=1;
	iConfigurarEntrada[3]=1;
	iConfigurarEntrada[0]=1;

	return 0;
}



int CVICALLBACK DIG_LeerPuertos(int reserved, int timerId, int event, 
	void *callbackData, int eventData1, int eventData2)
{
	uInt32 iValor=0;
	
	if (event == EVENT_TIMER_TICK)
	{
		DAQmxReadDigitalScalarU32 (misDigitales, 10.0, &iValor, 0);
	
		DIG_Separar(iValor);
		
		//SetCtrlVal(iPanel, iControl, iValor);
	}
		
	
	return 0;
}


int DIG_CrearTarea(int ipanel, int icontrol)
{
	iPanel = ipanel;
	iControl = icontrol;
	
	if (iTareaCreada == SIN_TAREA)
	{
		DAQmxCreateTask ("Digitales", &misDigitales);

		DAQmxCreateDIChan (misDigitales, "Dev1/port0:2", "", DAQmx_Val_ChanForAllLines);
	
		DAQmxStartTask (misDigitales);
	
		//inicializa vectores
		DIG_InicializaVectores();
	
		iTimerDigital = NewAsyncTimer (0.05, -1, 1, DIG_LeerPuertos, 0);
		
		iTareaCreada=CON_TAREA;
	}
	
	return 0;
}



int DIG_Evento(stNumeroBit numeroBit, int (*funcionEvento)(stNumeroBit, stEstadoBit))
{
	
	if (numeroBit != TODOS)
		funcion[numeroBit] = funcionEvento;
	
	if (numeroBit == TODOS)
	{
		for (int i=0; i<32; i++)
			funcion[i] = funcionEvento;
	}
	
	return 0;
}


int DIG_Detener()
{
	
	if (iTareaCreada == CON_TAREA)
	{
		DiscardAsyncTimer (iTimerDigital);
	
		SetBreakOnLibraryErrors (0);
		DAQmxStopTask (misDigitales);
			
		DAQmxClearTask (misDigitales);
	
		SetBreakOnLibraryErrors (1);
		
		iTareaCreada = SIN_TAREA;
	}
	
	
	return 0;
}



int DIG_Estado(int iBit)
{
	return EstadoEntradas[iBit];
}

