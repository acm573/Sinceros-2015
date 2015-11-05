#include <userint.h>
#include "PRE_Principal.h"
#include "pre_variables.h"


int (*miFuncionPRE)(void);



/* 
 * Captura los eventos registrados sobre los controles el panel iPanelPrincipal
 */

int CVICALLBACK PRE_SeleccionPantallaPrincipal (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE:
			PRE_ResaltarOpcion(panel, control,pMenu_cnvFondo,1, pPrincipal_txtEstado);
			break;
			
		case EVENT_LEFT_CLICK_UP:
			
			
			break;
	}
	return 0;
}


int PRI_LlamadasRemotas(int (*funcionPaso)(void))
{
	miFuncionPRE = funcionPaso;
	SetCtrlAttribute(iPanelPrincipal, pPrincipal_tmrLlamadas, ATTR_ENABLED, 1);
	
	return 0;
}



int CVICALLBACK PRI_Llamadas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			SetCtrlAttribute(panel, control, ATTR_ENABLED, 0);
			miFuncionPRE();
			break;
	}
	return 0;
}
