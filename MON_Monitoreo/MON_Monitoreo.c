
#include <formatio.h>
#include "userint.h"
#include "asynctmr.h"
#include "com_comunicacion.h"
#include "pre_variables.h"
#include <ansi_c.h>
#include <toolbox.h>
#include "ent_funciones.h"

int iTimer=0;


//prototipo de funciones
int MOT_PosicionActual(long *iRotacional, long *iLineal);
int CVICALLBACK Actualiza (int reserved, int timerId, int event, void *callbackData, 
		int eventData1, int eventData2);
char *PAT_PosicionCercaDe(double dCuentas, ListType miLista, int iPosLineal, 
	int iPosRotacional);
char *ENT_NeutralActual(void);


int MON_IniciaMonitoreo()
{
	
	iTimer = NewAsyncTimer (.05, -1, 1, Actualiza, 0);

	
	return 0;
}



int MON_FinMonitoreo()
{
	DiscardAsyncTimer(iTimer);
	
	return 0;
}




int CVICALLBACK Actualiza (int reserved, int timerId, int event, void *callbackData, 
		int eventData1, int eventData2)
{
	static int iMonCom=0;
	static int iMonCerca=0;
	long iRotacional;
	long iLineal;
	
	if (event==EVENT_TIMER_TICK)
	{
		if (iMonCom == 20)
		{
			COM_Respuesta();
			iMonCom=0;
		}
		iMonCom++;
		
		
		MOT_PosicionActual(&iRotacional, &iLineal);
		
		
		if (ENT_EstadoEntrenamiento() == CON_RECORRIDO)
		{
			if (iMonCerca == 20)
			{
				char *cUbicacion = PAT_PosicionCercaDe(DISTANCIA_DETECCION, miLista, iRotacional, iLineal);
				if (strlen(cUbicacion)>0)
				{
					ENT_PintarPuntos(RECORRIDO, cUbicacion);
					
				}
				iMonCerca=0;
			}
			iMonCerca++;
		}
			
	}		
	
	return 0;
}



