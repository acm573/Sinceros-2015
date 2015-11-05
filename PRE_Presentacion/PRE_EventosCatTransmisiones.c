#include "toolbox.h"
#include <userint.h>
#include "PRE_Principal.h"
#include "pre_variables.h"




/* 
 * Captura los eventos registrados sobre el panel iPanelCatTransmisiones
 */
int CVICALLBACK PRE_PanelCatTransmisiones (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			PRE_AnimacionMenu(OCULTAR);
			break;
	}
	return 0;
}



/* 
 * Captura los eventos registrados sobre los controles del panel iPanelCatTransmisiones
 */

int CVICALLBACK PRE_SeleccionCatTransmisiones (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	if (event == EVENT_MOUSE_POINTER_MOVE)
	{
		
		PRE_ResaltarOpcion(panel, control, pCatTransm_cnvFondo, 3, pCatTransm_lstTransmisiones, 
			pCatTransm_numNumeroVelocidades, pCatTransm_txtNombreTransmision);
	}
	
	if (event == EVENT_LEFT_CLICK_UP)
	{
		if (control == pCatTransm_picCerrar)
		{
			PRE_OcultarPanel(panel);
		}
		
	}
	
	
	return 0;
	
}
