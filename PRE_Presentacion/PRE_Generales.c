#include "toolbox.h"
#include <ansi_c.h>
#include "pre_variables.h"
#include "stdarg.h"




int PRE_ResaltarOpcion(int iPanel, int iControl, int iFondo, int iCantidad, ...)
{
	int iTop = 0;
	int iLeft = 0;
	int *iLista;
	va_list pa;
	int iExiste = 0;
	
	va_start(pa, iCantidad);
	
	//Crea la lista en la cual colocará cada una de las referencias de los controles
	//y verifica que el parámetro iControl es diferente a lo que hay en iLista
	iLista = malloc(sizeof(int)*iCantidad);
	
	for (int i=0;i<iCantidad;i++)
	{
		iLista[i] = va_arg(pa, int);
		if (iControl == iLista[i])
		{
			iExiste=1;
			break;
		}
	}
	
	
	if (iExiste == 0)
	{
		GetCtrlAttribute(iPanel, iControl, ATTR_TOP, &iTop);
		GetCtrlAttribute(iPanel, iControl, ATTR_LEFT, &iLeft);
	
	
		SetCtrlAttribute(iPanel, iFondo, ATTR_TOP, iTop);
		SetCtrlAttribute(iPanel, iFondo, ATTR_LEFT, iLeft);
	}
	
	//Llama al proceso que pinta el mensaje de ayuda en pantalla
	PRI_MensajeControl(iPanel, iControl);

	return 0;
}


int PRE_Desmarcar(int iControl)
{
	SetCtrlAttribute(PRE_PanelVisible(0), iControl, ATTR_LEFT, -1000);
	return 0;
}


int PRE_OcultarPanel(int iPanel)
{
	int iLeft=0;
	int i=0;
	
	GetPanelAttribute(iPanel, ATTR_LEFT, &iLeft);
	
	i=0;
	while (i<50)
	{
		SetPanelAttribute(iPanel, ATTR_LEFT, iLeft-i);
		DelayWithEventProcessing(0.0);
		i+=10;
	}
	
	SetPanelAttribute (iPanel, ATTR_VSCROLL_OFFSET, 0);
	SetPanelAttribute (iPanel, ATTR_HSCROLL_OFFSET, 0);
	
	HidePanel(iPanel);
	
	PRE_PanelVisible(iPanel);	
	
	return 0;
}
