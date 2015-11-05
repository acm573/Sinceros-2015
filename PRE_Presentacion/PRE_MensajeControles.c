#include <ansi_c.h>
#include "pre_variables.h"



int PRI_MensajeControl(int iPanel, int iControl)
{
	char *pcMensaje="";
	
	
	/*
	 * Mensajes para los elementos del panel iPanelMenuPrincipal
	 */
	
	if (iPanel == iPanelMenuPrincipal)
	{
		switch (iControl)
		{
			case pMenu_picCerrar:
				pcMensaje = "Oculta el menú de opciones del sistema.";
				break;
				
			case pMenu_picTransmisiones:
				pcMensaje = "Permite dar mantenimiento al catálogo de tranmisiones.";
				break;
				
			case pMenu_picEntrenamiento:
				pcMensaje = "Habilita la función de entrenamiento de posiciones para una transmisión.";
				break;
				
			case pMenu_picSecuencias:
				pcMensaje = "Permite diseñar las características de una secuencia de prueba.";
				break;
				
			case pMenu_picModoAutomatico:
				pcMensaje = "Da inicio a una secuencia de prueba, aplicando las características definidas por el operadr.";
				break;
				
			case pMenu_picModoManual:
				pcMensaje = "Muestra la interfaz de operación con el banco en modo manual.";
				break;
				
			case pMenu_picCalibracion:
				pcMensaje = "Inicia el módulo de calibración para los componentes de medición del sistema.";
				break;
				
			case pMenu_picRegistros:
				pcMensaje = "Permite recuperar la información almacenada de pruebas pasadas.";
				break;
				
			case pMenu_picSalir:
				pcMensaje = "Finaliza la sesión con el sistema.";
				break;
			
		}
	}
	
	
	/*
	 * Mensajes para los elementos del panel iPanelPrincipal
	 */
	if (iPanel == iPanelPrincipal)
	{
		switch (iControl)
		{
			case pPrincipal_txtEstado:
				pcMensaje = "Despliega información referente a las opciones disponibles en el sistema.";
				break;
		}
	}
	
	
	/*
	 * Mensajes para los elementos del panel iPanelCatTransmisiones
	 */
	if (iPanel == iPanelCatTransmisiones)
	{
		switch (iControl)
		{
			case pCatTransm_picAgregar:
				pcMensaje = "Permite capturar la información que describe a una nueva transmisión.";
				break;
				
			case pCatTransm_picEditar:
				pcMensaje = "Habilita la información de una transmisión para que sea actualizada.";
				break;
				
			case pCatTransm_picEliminar:
				pcMensaje = "Elimina del sistema a la transmisión seleccionada.";
				break;
				
			case pCatTransm_picGuardar:
				pcMensaje = "Guarda la nueva transmisión o los cambios realizados a la información de una transmisión existente.";
				break;
				
			case pCatTransm_picCancelar:
				pcMensaje = "Cancela cualquier cambio en la información que se haya realizado para una transmisión.";
				break;
				
			case pCatTransm_picCerrar:
				pcMensaje = "Cierra el módulo de mantenimiento a transmisiones.";
				break;
				
			case pCatTransm_lstTransmisiones:
				pcMensaje = "Muestra la relación de los nombres de transmisiones existentes en el sistema.";
				break;
				
			case pCatTransm_txtNombreTransmision:
				pcMensaje = "Campo que muestra o captura el nombre asignado a una transmisión.";
				break;
				
			case pCatTransm_numNumeroVelocidades:
				pcMensaje = "Indica el número de velocidades disponibles en la transmisión seleccionada. El numero de velocidades no incluye la reversa";
				break;
			
			
		}
	}
	
	
		/*
	 * Mensajes para los elementos del panel iPanelEntrenamiento
	 */
	if (iPanel == iPanelEntrenamiento)
	{
		switch (iControl)
		{
			case pEntrenar_picNuevo:
				pcMensaje = "Permite ingresar una nueva configuración de posiciones de velocidad para la transmisión.";
				break;
				
			case pEntrenar_picEliminar:
				pcMensaje = "Elimina la configuración de velocidades para la transmisión seleccionada.";
				break;
				
			case pEntrenar_picCerrar:
				pcMensaje = "Cierra el módulo de entrenamiento.";
				break;
				
			case pEntrenar_lstTransmisiones:
				pcMensaje = "Muestra la relación de los nombres de transmisiones existentes en el sistema.";
				break;
				
			case pEntrenar_canDistribucionVel:
				pcMensaje = "Muestra gráficamente la configuración de velocidades para la transmisión actual.";
				break;
				
			case pEntrenar_tblEntrenamiento:
				pcMensaje = "Despliega los puntos que se han entrenado para la transmisión actual.";
				break;
				
			
		}
	}
	
	PRE_BarraEstado(strlen(pcMensaje)>0?pcMensaje:"");
		
	return 0;
}



int PRE_BarraEstado(char *pcCadena)
{
	SetCtrlVal (iPanelPrincipal, pPrincipal_txtEstado, pcCadena);
	
	return 0;
}
