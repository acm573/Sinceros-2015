#include "toolbox.h"
#include "pre_variables.h"




/* 
 * Captura los eventos registrados sobre los controles del panel iPanelMenuPrincipal
 */

int CVICALLBACK PRE_SeleccionMenu (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE:
			PRE_ResaltarOpcion(panel, control,pMenu_cnvFondo,2, pMenu_cnvFondoPanel, pMenu_picCerrar);
			break;
		
			
		case EVENT_LEFT_CLICK_UP:
			
			if (control == pMenu_picCerrar)
			{
				PRE_AnimacionMenu(OCULTAR);
			}
			
			if (control == pMenu_picSalir)
			{
				PRE_AnimacionMenu(OCULTAR);
				PRE_FinalizarAplicacion();
			}
			
			if (control == pMenu_picTransmisiones)
			{
				SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_cnvFondo, ATTR_LEFT, -1000);
				PRE_UbicarPanel(iPanelCatTransmisiones);
			}
			
			if (control == pMenu_picEntrenamiento)
			{
				SetCtrlAttribute(iPanelEntrenamiento, pEntrenar_cnvFondo, ATTR_LEFT, -1000);
				PRE_IniciarEntrenamiento();
			}
			break;
	}
	
	return 0;
}






