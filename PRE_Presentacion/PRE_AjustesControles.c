/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Industrial
. Archivo:					PRE_AjustesControles.c
. Propósito:				Permite controlar el despliegue en pantalla del
.							panel principal.
. Lenguaje:					LabWindows/CVI 13.0.0 (632)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		31 de Marzo de 2014
. Responsable modificación: 
.
*******************************************************************************/
#include "toolbox.h"
#include "pre_variables.h"


int iPanelVisible=0;		//bandera que indica el estado del panel


/*****************************************************************************
.
. Función C:			PRE_PanelVisible
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Resuelve si el panel indicado es visible
. Parámetro de entrada:	int iPanel | Referencia del panel
. Parámetro de salida:	Retorna el estado del panel
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int PRE_PanelVisible(int iPanel)
{
	int iValor=0;
	
	//si el valor es cero, se retorna la referencia activa
	if (iPanel == 0)
		iValor = iPanelVisible;
	else
		iPanelVisible = 0;
	
	return iValor;
}



/*****************************************************************************
.
. Función C:			PRE_UbicarPanel
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Despliega un panel en pantalla considerando las
.						condiciones de la misma como lo es la posicion del
.						panel anterior y si existe el menu desplegado
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	Referencia a la cadena de caracteres con el mensaje
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int PRE_UbicarPanel(int iPanel)
{
	int iBarra=0;
	int iAlto=0;
	int iAncho=0;
	int iExistePanel=0;
	int iLeftPanelExiste=0;
	static int iPanelAnterior=0;
	

	if (iPanel!=0)
	{
		if (iPanelAnterior != 0)
			GetPanelAttribute (iPanelAnterior, ATTR_LEFT, &iLeftPanelExiste);
	
	
		if ((iPanelAnterior != 0) && (iPanelAnterior != iPanel))
		{
			//implementa la animación de salida
			int i=0;
		
			while (i<60)
			{
				SetPanelAttribute(iPanelAnterior, ATTR_LEFT, iLeftPanelExiste-i);
				DelayWithEventProcessing (.001);
				i+=20;
			}
		
			SetPanelAttribute (iPanelAnterior, ATTR_VSCROLL_OFFSET, 0);
			SetPanelAttribute (iPanelAnterior, ATTR_HSCROLL_OFFSET, 0);
		
			HidePanel(iPanelAnterior);
		}
	
	
		GetPanelAttribute(iPanelMenuPrincipal, ATTR_VISIBLE, &iExistePanel);
		if (iExistePanel==1)
			iLeftPanelExiste = ANCHO_MENU;
	
		GetPanelAttribute (iPanelPrincipal, ATTR_HEIGHT, &iAlto);
		GetPanelAttribute (iPanelPrincipal, ATTR_WIDTH, &iAncho);
	
		GetCtrlAttribute(iPanelPrincipal, pPrincipal_SPLITTER, ATTR_TOP, &iBarra);
	
		SetPanelAttribute(iPanel, ATTR_TOP, iBarra+8);
		SetPanelAttribute(iPanel, ATTR_LEFT, iLeftPanelExiste);
		SetPanelAttribute (iPanel, ATTR_TITLEBAR_VISIBLE, 0);
		SetPanelAttribute (iPanel, ATTR_FRAME_STYLE, VAL_HIDDEN_FRAME);
	
		SetPanelAttribute (iPanel, ATTR_HEIGHT, iAlto-120);
		SetPanelAttribute (iPanel, ATTR_WIDTH, iAncho-iLeftPanelExiste);
	
		GetPanelAttribute(iPanel, ATTR_VISIBLE, &iExistePanel);

		if (iExistePanel==0)
		{
			DisplayPanel(iPanel);
			iPanelVisible = iPanel;
		}
	
		iPanelAnterior = iPanel;
	}
	
	return 0;
}


