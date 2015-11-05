/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Industrial
. Archivo:					PAT_Movimientos.c
. Propósito:				Entrada a la aplicacion.
. Lenguaje:					LabWindows/CVI 13.0.0 (632)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		31 de Marzo de 2014
. Responsable modificación: 
.
*******************************************************************************/
#include <userint.h>
#include "PRE_Principal.h"
#include "mov_movimiento.h"
#include "pre_variables.h"
#include <formatio.h>
#include "pre_tipos.h"

#define HOST 0xFF			// Return Variable to Host Computer (not FlexMotion program variable)


//Definicion de las funciones prototipo
int MOT_PosicionActual(long *iRotacional, long *iLineal);
char *PAT_PosicionCercaDe(double dCuentas, ListType Lista, int iPosLineal, int iPosRotacional);
void * UTL_Split (char *pcCadena, char *pcPatron, char (*c)[30], int *total);
char *ENT_LeerDesplazamientoPrueba(void);
int PAT_ObtienePlano(char *pcVel);
char *PAT_ObtieneNeutral(int iPlano);
int PAT_AnalizarPatronCambioFinal(char *cPatronCambio);
char *PAT_AnalizarMovimientoLista2(char (*c)[30]);
int ENT_LeeEsperaBoton(void);
int ENT_ColocaMensaje(char *cMensaje);
char *GRA_Strcat(int iNoElementos, ...);
int ENT_CicloConcluido(void);

double dVelocidadMovimiento=0;
double dTiempoEmbrague=0;
double dTiempoVelocidad=0;
int iPaso=0;
stEdoEjecucion EstadoEjecucion=DESHABILITAR_MOVIMIENTO;



int PAT_AsignaEstadoEjecucion(stEdoEjecucion Estado)
{
	EstadoEjecucion = Estado;
	return 0;
}


stPosicion PAT_Informacion(char *c)
{
	char cVelocidad[10]={0};
	static int iElementos=0;
	stPosicion p = MistPosicion;
	
	//obtiene el indice de la velocidad
	iElementos = ListNumItems(miLista);
	
	for (int i=0;i<iElementos;i++)
	{
		ListGetItem(miLista, &p, i+1);
		
		if (CompareStrings(p.cDescripcion, 0, c, 0, 0)==0)
		{
			i=iElementos;
		}
	}
	return p;
}



double PAT_CalculaVelocidad(stPosicion destino, double dTiempo)
{
	double dVelocidad=0;
	static int iLineal=0;
	static int iRotacional=0;
	double dDistancia=0;
	
	//Ahora debe calcula la distancia entre 2 puntos y determinar la velocidad
	//a la que debe viajar para cubrir el recorrido en el tiempo indicado
	dDistancia = sqrt( pow((destino.iLineal - iLineal),2) + pow((destino.iRotacional-iRotacional),2) );
	
	//d = v/t       v = d * t
	
	//dVelocidad = 	dDistancia (cuentas) * dTiempo (segundos)
	//dVelocidad = 	dDistancia (cuentas)/  * dTiempo (segundos)
	
	dDistancia = dDistancia/4096.0;
	
	dTiempo = dTiempo / 60.0;
	
	dVelocidad = dDistancia / dTiempo;
	
	iLineal = destino.iLineal;
	iRotacional = destino.iRotacional;
	
	return dVelocidad;
}




int PAT_EjecutaMovimiento(stPosicion destino)
{
	u8 vectorSpaceNum;		// Vector space that will use the X and Y axes
	u8 xAxisNum;			// X Axis number
	u8 yAxisNum;			// Y Axis number 
	i32 xTargetPosition;	// Position to which the X axis will move when started
	i32 yTargetPosition;	// Position to which the Y axis will move when started 
	f64 velocity_RPM;		// Maximum vector velocity of the axes in RPM
	f64 acceleration_RPSPS;	// Vector acceleration rate for the axes
	i32 status;				// Error checking variable
	u8 boardID;				// Reference number for board to use (from Board Configuration Utility)    

	//Variables for modal error handling
	u16 commandID;					// The commandID of the function
	u16 resourceID;					// The resource ID
	u16 csr = 0;					// Communication Status Register
	i32 errorCode;
	u16 estado;
	double dTiempo=0;				//tiempo para cada movimiento
	
	
	//Analiza a que velocidad debe realizar el movimiento
	//GetCtrlVal (panelHandle, PANEL_numTiempo, &dTiempo);
	dTiempo = dVelocidadMovimiento;
	
	velocity_RPM = PAT_CalculaVelocidad(destino,dTiempo);
	
	vectorSpaceNum = NIMC_VECTOR_SPACE1;
	errorCode = flex_config_vect_spc (1, vectorSpaceNum, 1, 2, 0);
	
	errorCode = flex_set_op_mode (1, vectorSpaceNum, NIMC_ABSOLUTE_POSITION);
	
	errorCode = flex_load_rpm (1, vectorSpaceNum, velocity_RPM, HOST);
	errorCode = flex_load_rpsps (1, vectorSpaceNum, NIMC_BOTH, 900, HOST);
	
	errorCode = flex_load_vs_pos (1, vectorSpaceNum, destino.iRotacional, destino.iLineal, 0, HOST);
	
	errorCode = flex_start (1, vectorSpaceNum, 0);
	
	estado=0;
	
	//printf("************************************************\n");
		
	//i32 rota;
	//i32 linea;
		
	do 
	{
		//flex_read_pos_rtn (1, 1, &rota);
		//flex_read_pos_rtn (1, 2, &linea);
		
		//printf("[%d,%d] de [%d,%d]\n",rota, linea, destino.iRotacional, destino.iLineal);
		
		if (EstadoEjecucion == DESHABILITAR_MOVIMIENTO)
		{
			flex_stop_motion (1, vectorSpaceNum, NIMC_DECEL_STOP, 0x0);
			return 0;
		}
		
		errorCode = flex_read_trajectory_status_rtn (1, vectorSpaceNum,
										 NIMC_MOVE_COMPLETE_STATUS, &estado);
		DelayWithEventProcessing(0.010);
		
	} while ((estado & 0x02)==0);
	
	DelayWithEventProcessing(0.050);
	
	//GetKey ();
	
	return 0;
}



int PAT_EjecucionPorPatron2(char *cPatron)
{
	//char cPatron[260]={0};
	char cLista[30][30]={0};
	static int iTotal=0;
	stPosicion p;
	
	//GetCtrlVal (panelHandle, PANEL_strRutaAcercar, cPatron);
	
	UTL_Split(cPatron, "->", cLista, &iTotal);
	
	//iDibujarEjes=1;
	
	for (int i=0;i<iTotal; i++)
	{
		if (EstadoEjecucion == HABILITAR_MOVIMIENTO)
		{
			p=PAT_Informacion(cLista[i]);
			
			//printf("Movimiento ->> %s  \n", p.cDescripcion);
			
			PAT_EjecutaMovimiento(p);
		}
		else
			break;
	}
	
	return 0;
}





int PAT_Acercar(char *cRuta)
{
	char cPatron[100]={0};
	char cInicio[20]={0};
	char cDestino[20]={0};
	char *c;
	int iControl=0;
	long iRotacional;
	long iLineal;
	
	//GetCtrlVal (panelHandle, PANEL_strDetalle, cPatron);
	//GetCtrlVal (panelHandle, PANEL_strCerca, cInicio);
	//Fmt(cPatron,"%s<%s",ENT_LeerDesplazamientoPrueba());		<-------------
	Fmt(cPatron,"%s<%s",cRuta);								//	<-------------
	
	ENT_ColocaMensaje("PAT_Acercar()");
	ENT_ColocaMensaje(GRA_Strcat(2,"cPatron -> ", cPatron));
	
	DelayWithEventProcessing(2.0);
		
	MOT_PosicionActual(&iRotacional, &iLineal);
	Fmt(cInicio,"%s<%s",PAT_PosicionCercaDe(600, miLista, iRotacional, iLineal));
	
	ENT_ColocaMensaje(GRA_Strcat(2,"cInicio -> ", cInicio));
	
	if (strlen(cPatron)>0)
	{
		char cLista[30][30]={0};
		int iTotal=0;
		
		UTL_Split(cPatron, "->", cLista, &iTotal);
		
		Fmt(cPatron,"%s<%s%s%s",cInicio,"->",cLista[0]);
		Fmt(cDestino,"%s<%s",cLista[0]);
		
		ENT_ColocaMensaje(GRA_Strcat(2,"El movimiento de acercamiento es de ", cPatron));
		
		//verifica que la palanca se encuentre en una poscion diferente al inicio
		if (CompareStrings(cInicio,0,cDestino,0,0) != 0)
		{
			UTL_Split(cPatron, "->", cLista, &iTotal);
		
			c=PAT_AnalizarMovimientoLista2(cLista);
			
			ENT_ColocaMensaje(GRA_Strcat(2,"c -> ", c));
			
		
			UTL_Split(c, "->", cLista, &iTotal);
		
		
			for (int i=0;i<iTotal;i++)
			{
				if (iControl==0)
				{
					//Busca la primer coincidencia
					if (CompareStrings(cLista[i],0,cInicio,0,0)==0)
					{
						Fmt(cPatron,"%s<%s",cInicio);
						strcat(cPatron,"->");
						iControl++;
					}
				}
				else
				{
					//espera hasta encontrar cDestino
					if (CompareStrings(cLista[i],0,cDestino,0,0)==0)
					{
						strcat(cPatron,cDestino);
						strcat(cPatron,"->");
						i=iTotal;
					
					}
					else
					{
						strcat(cPatron,cLista[i]);
						strcat(cPatron,"->");
					}
				}
		
			}
		
			//SetCtrlVal (panelHandle, PANEL_strRutaAcercar, cPatron);
		
			ENT_ColocaMensaje(GRA_Strcat(2,"Se va a ejecutar --> ", cPatron));
			ENT_LeeEsperaBoton();
			
			PAT_EjecucionPorPatron2(cPatron);
		}
	}
	
	return 0;
}


stTipo PAT_TipodeMovimiento(stPosicion a, stPosicion b)
{
	stTipo t=EMBRAGUE;
	
	
	if ((a.iTipo == NEUTRAL) && (b.iTipo == VELOCIDAD))
		t=EMBRAGUE;
	
	if ((a.iTipo == VELOCIDAD) && (b.iTipo == NEUTRAL))
		t=DESEMBRAGUE;
	
	if ((a.iTipo == NEUTRAL) && (b.iTipo == NEUTRAL))
		t=DESEMBRAGUE;
	
	return t;
}


stTipo PAT_MoviendoEje(stPosicion a, stPosicion b)
{
	stTipo t=ROTACIONAL;
	
	
	if ((a.iTipo == NEUTRAL) && (b.iTipo == VELOCIDAD))
		t=ROTACIONAL;
	
	if ((a.iTipo == VELOCIDAD) && (b.iTipo == NEUTRAL))
		t=ROTACIONAL;
	
	if ((a.iTipo == NEUTRAL) && (b.iTipo == NEUTRAL))
		t=LINEAL;
	
	return t;
}




int PAT_CargaMovimientoSecuencial(stPosicion destino, stTipo t, int indiceMovimiento)
{
	u8 vectorSpaceNum;		// Vector space that will use the X and Y axes
	u8 xAxisNum;			// X Axis number
	u8 yAxisNum;			// Y Axis number 
	i32 xTargetPosition;	// Position to which the X axis will move when started
	i32 yTargetPosition;	// Position to which the Y axis will move when started 
	f64 velocity_RPM;		// Maximum vector velocity of the axes in RPM
	f64 acceleration_RPSPS;	// Vector acceleration rate for the axes
	i32 status;				// Error checking variable
	u8 boardID;				// Reference number for board to use (from Board Configuration Utility)    
	u8	mustOn;
	u8	mustOff;
	u32	timeout;
	

	//Variables for modal error handling
	u16 commandID;					// The commandID of the function
	u16 resourceID;					// The resource ID
	u16 csr = 0;					// Communication Status Register
	i32 errorCode;
	u16 estado;
	double dTiempo=0;				//tiempo para cada movimiento
	
	
	if (t==EMBRAGUE)
	{
		dTiempo = dTiempoEmbrague;
	}
	else
	{
		dTiempo=dTiempoVelocidad;
	}
	
	
	velocity_RPM = PAT_CalculaVelocidad(destino,dTiempo);
	
	vectorSpaceNum = NIMC_VECTOR_SPACE1;
	
	//esta configuracion solo se debe ejecutar en una ocasion
	if (iPaso==0)
	{
		flex_config_vect_spc (1, vectorSpaceNum, 1, 2, 0);
		flex_set_op_mode (1, vectorSpaceNum, NIMC_ABSOLUTE_POSITION);
		
		flex_load_rpsps (1, vectorSpaceNum, NIMC_BOTH, 900, HOST);
		iPaso=1;
	}

		
	flex_load_rpm (1, vectorSpaceNum, velocity_RPM, HOST);
	flex_load_vs_pos (1, vectorSpaceNum, destino.iRotacional, destino.iLineal, 0, HOST);
	
	flex_start (1, vectorSpaceNum, 0);
	
	do 
	{
		if (EstadoEjecucion == DESHABILITAR_MOVIMIENTO)
		{
			flex_stop_motion (1, vectorSpaceNum, NIMC_DECEL_STOP, 0x0);
			return 0;
		}
		
		flex_read_trajectory_status_rtn (1, vectorSpaceNum,
										 NIMC_MOVE_COMPLETE_STATUS, &estado);
		DelayWithEventProcessing(0.01);
	} while ((estado & 0x02)==0);
	
	return 0;
}




int PAT_EjecucionPorSecuencia(int iNumCiclos, char *cRuta)
{
	
	char cPatron[260]={0};
	char cLista[30][30]={0};
	int iTotal=0;
	u16 estado;
	stPosicion p;   //fin
	stPosicion q;   //inicio
	stTipo t;
	stTipo eje;
	
	int iCiclos=0;
	
	
	ENT_ColocaMensaje("PAT_EjecucionPorSecuencia()");
	
	//Fmt(cPatron,"%s<%s",ENT_LeerDesplazamientoPrueba());
	Fmt(cPatron,"%s<%s",cRuta);
	
	ENT_ColocaMensaje(GRA_Strcat(2,"cPatron -> ", cPatron));
	
	iCiclos=iNumCiclos;
	
	UTL_Split(cPatron, "->", cLista, &iTotal);
	
	q=PAT_Informacion(cLista[0]);

	iPaso=0;
	
	for (int j=0;j<iCiclos;j++)
	{
		for (int i=1;i<iTotal; i++)
		{
			p=PAT_Informacion(cLista[i]);
			t = PAT_TipodeMovimiento(q,p);
			
			ENT_ColocaMensaje(GRA_Strcat(4,"Moviendo de ",q.cDescripcion," a ",p.cDescripcion));
			ENT_LeeEsperaBoton();
			
			if (EstadoEjecucion == DESHABILITAR_MOVIMIENTO)
				break;
			
			if (t == EMBRAGUE)
			{
				
				Delay (dPausaMovimientoEmb);
				
				/*
				Se debe notificar que a partir de este momento se registre la fuerza
				en una lista paralela
				
				<<----  Insertar llamada a función aquí!!!
				DAQ_CapturaFuerza(INICIAR);
				
				*/
			}
			
			eje = PAT_MoviendoEje(q,p);
			
			if (t == EMBRAGUE)
			{
				/*
				
				Se debe validar si se ha alcanzado el máximo de fuerza solicitado
				
				<<---- Insertar llamada de código a partir de aqui
				DAQ_CapturaFuerza(FINALIZAR);
				
				La funcion RegistroFuerzaMaxima se sugiere se implemente:
					- Descargar todo el contenido de la lista Fuerza
					- Para cada elemento de la lista, obtener el máximo del vector
					- El valor maximo resultante, será la fuerza máxima aplicada
					- Limpiar la lista de Fuerza
				
				if (RegistroFuerzaMaxima() < FUERZA_LIMITE)
				{
					DAQ_CapturaFuerza(INICIAR);
					PAT_EjecutaSobreViaje();
					DAQ_CapturaFuerza(FINALIZAR);
					
					Registrar la nueva fuerza alcanzada 
					Imprimir RegistroFuerzaMaxima()
				}
				
				*/
			}
		
			
			//Aqui ejecuta el movimiento (Embrague - Desembrague - Neutral)
			PAT_CargaMovimientoSecuencial(p,t, i);
			
			if (t == EMBRAGUE)
			{
				
				Delay (dPausaMovimientoDes);
				
				/*
				Se debe notificar que a partir de este momento se registre la fuerza
				en una lista paralela
				
				<<----  Insertar llamada a función aquí!!!
				DAQ_CapturaFuerza(INICIAR);
				
				*/
			}
			
			
			q = p;
			
			//if (t == EMBRAGUE)
			//	DelayWithEventProcessing(1.0);
			
			if (EstadoEjecucion == DESHABILITAR_MOVIMIENTO)
				break;
			
			
		}
		
		ENT_CicloConcluido();
		
		ProcessSystemEvents();
		ProcessDrawEvents();
		
		if (EstadoEjecucion == DESHABILITAR_MOVIMIENTO)
			break;
	}
	
	return 0;
}





int PAT_ConfigurarTiemposRecorrido(stTipoRecorrido Tipo, double dTiempo, int iNumCiclos, char *cRuta)
{
	
	
	
	switch (Tipo)
	{
		case PRUEBA_RECORRIDO_INICIO:
			//primero debe acercar la palanca al inicio del recorrido
			dVelocidadMovimiento = dTiempo; //TIEMPO_RECORRIDO_PRUEBA;
			
			//ejecuta el movimiento para acercar
			PAT_Acercar(cRuta);
			break;
			
		case PRUEBA_RECORRIDO_PUNTOS:
			
			dTiempoEmbrague = dTiempo; //TIEMPO_RECORRIDO_PRUEBA;
			dTiempoVelocidad = dTiempo; //TIEMPO_RECORRIDO_PRUEBA;
			
			//ejecuta la secuncia que recorre las diferentes posiciones de velocidad
			PAT_EjecucionPorSecuencia(iNumCiclos,cRuta);
			
			
			break;
	}
	
	return 0;
}


