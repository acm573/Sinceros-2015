#include <userint.h>
#include "bds_basededatos.h"
#include "pre_tipos.h"



/*
 * Obtiene la lista de transmisiones y la dibuja en la pantalla
 */
stEntrenar ENT_CargarTransmisiones(int iPanel, int iControl)
{
	stEntrenar ModoControles = CTRL_SIN_TRANSMISIONES;
	
	if (BDS_LeerTransmisiones(iPanel, iControl)>0)
		ModoControles = CTRL_CON_TRANSMISIONES;
	
	
	return ModoControles;
}


/*
 * Carga las posiciones para la transmision  seleccionada
 */

stPosiciones ENT_CargarPosiciones(int iPanel, int iControl, int iCanvas)
{
	int iId=0;
	stPosiciones Posiciones = SIN_POSICIONES;
	
	GetCtrlVal (iPanel, iControl, &iId);
	
	
	
	//aqui va el codigo que recupera de la base de datos las posiciones entrenadas
	
	return Posiciones;
}
