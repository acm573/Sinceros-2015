#include "PRE_Principal.h"
#include <userint.h>
#include "toolbox.h"
#include "pre_variables.h"



//int PRE_AnimacionMenu(int inicio, int fin)
int PRE_AnimacionMenu(stMenuAcciones Accion)
{
	int iExistePanel=0;
	int inicio=0;
	int fin=0;
	int iLeftPanel=0;
	
	if (Accion == MOSTRAR)
	{
		inicio = -ANCHO_MENU;
		fin = 0;
	}
	else
	{
		fin = -ANCHO_MENU;
		inicio = 0;
	}

	GetPanelAttribute(iPanelMenuPrincipal, ATTR_VISIBLE, &iExistePanel);

	if (iExistePanel==1)
	{
		
		if (inicio<fin)
		{	
			if (PRE_PanelVisible(0)!=0)
				GetPanelAttribute (PRE_PanelVisible(0), ATTR_LEFT, &iLeftPanel);
			
			
			while (inicio <= fin)
			{
				SetPanelAttribute (iPanelMenuPrincipal, ATTR_LEFT, inicio);	
		
				if ((inicio+ANCHO_MENU) > ANCHO_BOTON_MENU)
					SetCtrlAttribute (iPanelPrincipal, pPrincipal_txtEstado, ATTR_LEFT, inicio+ANCHO_MENU);
			
				
				if (PRE_PanelVisible(0)!=0)
				{
					iLeftPanel+=PASO_ANIMACION;
					SetPanelAttribute (PRE_PanelVisible(0), ATTR_LEFT, iLeftPanel);
				}

				
				inicio+=PASO_ANIMACION;
				ProcessSystemEvents();
				
			}
		
			SetCtrlAttribute (iPanelPrincipal, pPrincipal_txtEstado, ATTR_LEFT, ANCHO_MENU);
			SetCtrlAttribute (iPanelPrincipal, pPrincipal_txtEstado, ATTR_LEFT, ANCHO_MENU-10);
			
			if (PRE_PanelVisible(0)!=0)
			{
				SetPanelAttribute (PRE_PanelVisible(0), ATTR_LEFT, ANCHO_MENU);
			}
		}
		else
		{
		
			if (PRE_PanelVisible(0)!=0)
			{
				GetPanelAttribute (PRE_PanelVisible(0), ATTR_LEFT, &iLeftPanel);
			}
			
			while (inicio >= fin)
			{
				SetPanelAttribute (iPanelMenuPrincipal, ATTR_LEFT, inicio);
		
				if ((inicio+ANCHO_MENU) > ANCHO_BOTON_MENU)
					SetCtrlAttribute (iPanelPrincipal, pPrincipal_txtEstado, ATTR_LEFT, inicio+ANCHO_MENU);	
				
				
				if (PRE_PanelVisible(0)!=0)
				{
					iLeftPanel-=PASO_ANIMACION;
					SetPanelAttribute (PRE_PanelVisible(0), ATTR_LEFT, iLeftPanel);
				}
				
				inicio-=PASO_ANIMACION;
		
				ProcessSystemEvents();
			}
		 
			if (PRE_PanelVisible(0)!=0)
			{
				SetPanelAttribute (PRE_PanelVisible(0), ATTR_LEFT, 0);
			}
			SetCtrlAttribute (iPanelPrincipal, pPrincipal_txtEstado, ATTR_LEFT, -ANCHO_MENU);	
			SetCtrlAttribute (iPanelPrincipal, pPrincipal_txtEstado, ATTR_LEFT, ANCHO_BOTON_MENU);
			HidePanel(iPanelMenuPrincipal);
		}
	}
	
	//Ahora verifica si existe un panel visible en este momento
	PRE_UbicarPanel(PRE_PanelVisible(0));
	PRE_BarraEstado("");
	
	return 0;
}



int PRE_ConfiguraMenu()
{
	int iAlto=0;
	
	GetPanelAttribute(iPanelPrincipal, ATTR_HEIGHT, &iAlto);
	SetPanelAttribute (iPanelMenuPrincipal, ATTR_TITLEBAR_VISIBLE, 0);
	SetPanelAttribute (iPanelMenuPrincipal, ATTR_FRAME_STYLE,
					   VAL_HIDDEN_FRAME);
	SetPanelAttribute (iPanelMenuPrincipal, ATTR_TOP, 0);
	SetPanelAttribute (iPanelMenuPrincipal, ATTR_LEFT, 0);
	SetPanelAttribute (iPanelMenuPrincipal, ATTR_HEIGHT, iAlto);
	
	
	//manda el menu hasta la posicion -100
	SetPanelAttribute (iPanelMenuPrincipal, ATTR_LEFT, -ANCHO_MENU);
	
	DisplayPanel(iPanelMenuPrincipal);
	
	PRE_AnimacionMenu(MOSTRAR);
	return 0;
}



int PRE_AjustarComponentes(int iDato)
{
	//Toma el ancho de la ventana
	int iBarra=0;
	int iAlto=0;
	int iLeft=0;
	int iAncho=0;
	int iAnchoLogo=0;
	int iExisteMenu=0;
	int iPanel=0;
	Rect    bounds;
	int iExistePanel=0;
	
	bounds.height=0;
	bounds.left=0;
	bounds.top=0;
	bounds.width=0;
	
	//Si el menu se encuentra desplegado, se ajusta la posición izquierda
	GetPanelAttribute(iPanelMenuPrincipal, ATTR_VISIBLE, &iExisteMenu);
	if (iExisteMenu==1)
		iLeft = ANCHO_MENU-5;
	else
		iLeft = ANCHO_BOTON_MENU;
	
	
	//Obtiene el alto del panel para ajustar la posición de la barra
	GetPanelAttribute (iPanelPrincipal, ATTR_HEIGHT, &iAlto);
	GetPanelAttribute (iPanelPrincipal, ATTR_WIDTH, &iAncho);
	
	if (iDato!=0)
		GetPanelEventRect(iDato, &bounds);
	
	
	if (iDato!=0)
	{
		int     heightMin, heightMax, widthMin, widthMax;
		int     sizingTop=0, sizingLeft=0;
	
		heightMin=500;
		heightMax=3000;
		widthMin=960;
		widthMax=3000;
	
	
		switch (iDato) 
		{
	        case PANEL_SIZING_TOP:
				break;
	        case PANEL_SIZING_TOPRIGHT:
	            sizingTop = 1;
	            break;
	        case PANEL_SIZING_LEFT:
				break;
	        case PANEL_SIZING_BOTTOMLEFT:
	            sizingLeft = 1;
	            break;
	        case PANEL_SIZING_TOPLEFT:
	            sizingTop = 1;
	            sizingLeft = 1;
	            break;
		}
	
		if (bounds.height < heightMin) 
		{
	        if (sizingTop)
	            bounds.top = bounds.top + bounds.height - heightMin;
    	
			bounds.height = heightMin;
	   	}
	    else if (bounds.height > heightMax) 
		{
	        if (sizingTop)
	            bounds.top = bounds.top + bounds.height - heightMax;
	        bounds.height = heightMax;
		}
	    if (bounds.width < widthMin) 
		{
	        if (sizingLeft)
	            bounds.left = bounds.left + bounds.width - widthMin;
	        bounds.width = widthMin;
		}
	    else if (bounds.width > widthMax) 
		{
	        if (sizingLeft)
	            bounds.left = bounds.left + bounds.width - widthMax;
	        bounds.width = widthMax;
		}
    
		SetPanelEventRect(iDato, bounds);
	}

	GetCtrlAttribute(iPanelPrincipal, pPrincipal_picLogoCidesi, ATTR_WIDTH, &iAnchoLogo);

	SetCtrlAttribute (iPanelPrincipal, pPrincipal_txtEstado, ATTR_LEFT, iLeft);
	SetCtrlAttribute (iPanelPrincipal, pPrincipal_txtEstado, ATTR_TOP, iAlto-30);

	//Ajusta el boton del menu de inicio
	SetCtrlAttribute (iPanelPrincipal, pPrincipal_btnMenu, ATTR_LEFT, 0);
	SetCtrlAttribute (iPanelPrincipal, pPrincipal_btnMenu, ATTR_TOP, iAlto-46);

	//Ajusta el logo de CIDESI
	SetCtrlAttribute (iPanelPrincipal, pPrincipal_picLogoCidesi,
					  ATTR_LEFT, iAncho-iAnchoLogo);

	//Ajusta la imagen de la transmision
	SetCtrlAttribute (iPanelPrincipal, pPrincipal_picLinea,
					  ATTR_LEFT, iAncho-1750);
	
	GetPanelAttribute(iPanelMenuPrincipal, ATTR_VISIBLE, &iExistePanel);
	if (iExistePanel==1)
	{
		SetPanelAttribute (iPanelMenuPrincipal, ATTR_HEIGHT, iAlto);
		iPanel = iPanelMenuPrincipal;
	}

	GetPanelAttribute(iPanelCatTransmisiones, ATTR_VISIBLE, &iExistePanel);
	if (iExistePanel==1)	
	{
		iPanel = iPanelCatTransmisiones;
	}
	
	GetPanelAttribute(iPanelEntrenamiento, ATTR_VISIBLE, &iExistePanel);
	if (iExistePanel==1)	
	{
		iPanel = iPanelEntrenamiento;
	}
	
	if ((iPanel!=0) && (iPanel != iPanelMenuPrincipal))
		PRE_UbicarPanel(iPanel);
	return 0;
}




int PRE_InicializarInterfaz()
{
	PRE_AjustarComponentes(0);
	
	return 0;
}










