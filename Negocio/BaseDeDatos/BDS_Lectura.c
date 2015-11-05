#include <userint.h>





int BDS_LeerTransmisiones(int iPanel, int iControl)
{
	int iRegistros=0;
	
	//limpia la lista
	DeleteListItem(iPanel, iControl, 0, -1);
	
	//insertar aqui llamada a lectura de base de datos
	
	InsertListItem(iPanel, iControl, -1, "Transmision 01", 1);
	InsertListItem(iPanel, iControl, -1, "Transmision 02", 2);
	
	iRegistros=1;
	
	return iRegistros;
}
