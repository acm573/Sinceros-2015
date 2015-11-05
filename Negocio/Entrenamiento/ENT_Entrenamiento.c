#include "pre_principal.h"
#include "mov_movimiento.h"
#include "dig_digitales.h"
#include "pre_tipos.h"
#include "pre_variables.h"
#include <formatio.h>


//prototipo de funciones
int MOT_EstadoMotores(stMotores Modo);
int MOT_PosicionMotores(stMotores Modo);
int (*funcionBoton1)(void);
int (*funcionBoton1_2)(int);
int (*funcionP)(void);
int (*funcionG)(void);
int ENT_LimpiarDesplazamientoPrueba(void);
int COM_EstructuraMensaje(unsigned char ucComando, unsigned int uiDireccion, unsigned char ucControlador,
		int iValor);


//variables globales
static char cDesplazamientoPrueba[500]={0};
static char cNeutralActual[20]={0};
static stEdoEntrenamiento EstadoEntrenamiento = SIN_RECORRIDO;

int ENT_PosicionCero(stNumeroBit noBit, stEstadoBit estado)
{
	MOT_PosicionMotores(MOT_POSICION_CERO);
	funcionP();
	return 0;
}


int ENT_Finalizado(stNumeroBit noBit, stEstadoBit estado)
{
	if (estado == PULSO_CORTO)
		funcionP();
	return 0;
}

int ENT_NuevoPunto(stNumeroBit noBit, stEstadoBit estado)
{
	if (estado == PULSO_CORTO)
		funcionG();
	return 0;
}


int ENT_DetenerPrueba(stNumeroBit noBit, stEstadoBit estado)
{
	if (estado == PULSO_CORTO)
		funcionG();
	return 0;
}


int ENT_RecorridoConfirmado(stNumeroBit noBit, stEstadoBit estado)
{
	if (estado == PULSO_CORTO)
		funcionP();
	return 0;
}


int ENT_AvanzarSiguientePaso(stNumeroBit noBit, stEstadoBit estado)
{
	if (estado == PULSO_CORTO)
		funcionP();
	return 0;
}

int ENT_ModalidadPasoAPaso(stNumeroBit noBit, stEstadoBit estado)
{
	if (estado == PULSO_CORTO)
		funcionBoton1();
	
	if (estado == PULSO_LARGO)
		funcionBoton1_2(0);
	return 0;
}


int ENT_AceptarPosicionTest(stNumeroBit noBit, stEstadoBit estado)
{
	if (estado == PULSO_CORTO)
		funcionP();
	return 0;
}

int ENT_SetearPosicion(stNumeroBit noBit, stEstadoBit estado)
{
	if (estado == PULSO_CORTO)
		funcionG();
	return 0;
}


int ENT_AccionesIniciarEntrenamiento(int (*funcionPaso)(void))
{
	if (funcionPaso != NULL)
	{
		//primero deshabilita los motores
		MOT_EstadoMotores(MOT_DESHABILITAR);
	
		DIG_CrearTarea(0, 0);
		DIG_Evento(BIT15, ENT_PosicionCero);
	
		funcionP = funcionPaso;
		ENT_LimpiarDesplazamientoPrueba();
		EstadoEntrenamiento = SIN_RECORRIDO;
	}
	else
	{
		DIG_Evento(BIT15, NULL);
	}
	return 0;
}


int ENT_AccionesSegundoPasoEntrenamiento(int (*funcionPaso)(void), int (*funcionPaso2)(void))
{
	if (funcionPaso != NULL)
	{
		DIG_Evento(BIT7, ENT_Finalizado);
		DIG_Evento(BIT8, ENT_NuevoPunto);
		funcionP = funcionPaso;
		funcionG = funcionPaso2;
	}
	else
	{
		DIG_Evento(BIT7, NULL);
		DIG_Evento(BIT8, NULL);
	}
	return 0;
}

int ENT_ModoPasoAPaso(int (*funcionPaso)(void), int (*funcionPaso2)(int))
{
	if (funcionPaso != NULL)
	{
		DIG_Evento(BIT7, ENT_ModalidadPasoAPaso);
		funcionBoton1 = funcionPaso;
		funcionBoton1_2 = funcionPaso2;
	}
	else
	{
		DIG_Evento(BIT15, NULL);
	}
	return 0;
}


int ENT_SiguientePaso(int (*funcionPaso)(void))
{
	if (funcionPaso != NULL)
	{
		DIG_Evento(BIT15, ENT_AvanzarSiguientePaso);
		funcionP = funcionPaso;
	}
	else
	{
		DIG_Evento(BIT15, NULL);
	}
	return 0;
}



int ENT_ConfirmaInicioRecorridoPrueba(int (*funcionPaso)(void), int (*funcionPaso2)(void))
{
	if (funcionPaso != NULL)
	{
		DIG_Evento(BIT15, ENT_RecorridoConfirmado);
		DIG_Evento(BIT8, ENT_DetenerPrueba);
		funcionP = funcionPaso;
		funcionG = funcionPaso2;
	}
	else
	{
		DIG_Evento(BIT15, NULL);
		DIG_Evento(BIT8, NULL);
	}
	return 0;
}


int ENT_AceptarPosicionTest01(int (*funcionPaso)(void), int (*funcionPaso2)(void))
{
	if (funcionPaso != NULL)
	{
		DIG_CrearTarea(0, 0);
		DIG_Evento(BIT15, ENT_AceptarPosicionTest);
		DIG_Evento(BIT8, ENT_SetearPosicion);
		funcionP = funcionPaso;
		funcionG = funcionPaso2;
	}
	else
	{
		DIG_Evento(BIT15, NULL);
		DIG_Evento(BIT8, NULL);
	}
	return 0;
}





int ENT_AccionesDetenerEntrenamiento()
{
	DIG_Detener();
	
	return 0;
}




int ENT_CalculaDesplazamientoPrueba()
{
	//debe realizar un recorrido por los diferentes planos en busca de las velocidades y su neutral
	int iElementos=0;
	stPosicion Analisis[TOTAL_ANALISIS]={0};
	int iPlanos[TOTAL_PLANOS]={0};
	int iTotalPlanos=0;
	int iIndiceAnalisis=0;
	char cAnalisis[500]={0};
	char c[30][30]={0};
	char cPrimerNeutral[20]={0};
	int iTotal=0;
	
	iElementos = ListNumItems(miLista);
	
	//el primer ciclo es para identificar el total de planos configurados
	for (int i=0;i<iElementos;i++)
	{
		stPosicion p;
		
		ListGetItem(miLista, &p, i+1);
		
		int iPosicionLibre=-1;
		int iExiste=0;
		//primero verifica que no se haya registrado el plano ya
		for (int j=0;j<TOTAL_PLANOS;j++)
		{
			if ((iPlanos[j] == 0) && (iPosicionLibre == -1))
				iPosicionLibre=j;
			
			if (iPlanos[j] == p.iPlano)
			{
				iExiste = 1;
				break;
			}
		}
		
		
		//si el plano no ha sido registrado
		if (iExiste == 0)
		{
			iPlanos[iPosicionLibre] = p.iPlano;
			iTotalPlanos++;
		}
	}
	
	
	//ahora recorre cada uno de los planos localizando sus velocidades disponibles
	int iIndice=0;
	for (int j=0;j<iTotalPlanos;j++)
	{
		iIndiceAnalisis=0;
		
		//inicializa el arreglo de Analisis
		for (int i=0;i<TOTAL_ANALISIS;i++)
			Analisis[i].iTipo = -999;
		
		for (int i=0;i<iElementos;i++)
		{
			stPosicion p;
		
			ListGetItem(miLista, &p, i+1);
		
			if (iIndiceAnalisis!=3)
			{
				if (iPlanos[j] == p.iPlano)
				{
					Analisis[iIndiceAnalisis++] = p;
				}
			}
			else
				break;
		}
		
		//ahora ordena la informacion de Analisis como Velocidad - Neutral - Velocidad
		//Revisa si el elemento de la posicion 1 es diferente de Neutral
		if (Analisis[1].iTipo != NEUTRAL)
		{
			stPosicion p;
			
			//revisa si la primer posicion es neutral
			if (Analisis[0].iTipo == NEUTRAL)
			{
				p=Analisis[0];
				Analisis[0] = Analisis[1];
				Analisis[1] = p;
			}
			else
			{
				p=Analisis[2];
				Analisis[2] = Analisis[1];
				Analisis[1] = p;
			}
		}
		
		
		//Aqui tengo en Analisis los elementos del plano j
		//inicia con la neutral
		strcat(cAnalisis,Analisis[1].cDescripcion);
		strcat(cAnalisis,"->");
		
		if (strlen(cPrimerNeutral)==0)
		{
			Fmt(cPrimerNeutral,"%s<%s",Analisis[1].cDescripcion);
			Fmt(cNeutralActual,"%s<%s",Analisis[1].cDescripcion);
		}
		
		for (int i=0;i<iIndiceAnalisis;i++)
		{
			strcat(cAnalisis,Analisis[i].cDescripcion);
			strcat(cAnalisis,"->");
		}
		
		if (iIndiceAnalisis > 2)
		{
			//termina con la neutral
			strcat(cAnalisis,Analisis[1].cDescripcion);
			strcat(cAnalisis,"->");
		}
	}
	
	strcat(cAnalisis,cPrimerNeutral);
	
	Fmt(cDesplazamientoPrueba,"%s<%s",cAnalisis);
	
	return 0;
}


int ENT_AsignaEstadoEntrenamiento(stEdoEntrenamiento Tipo)
{
	EstadoEntrenamiento = Tipo;
	return 0;
}


stEdoEntrenamiento ENT_EstadoEntrenamiento()
{
	return EstadoEntrenamiento;
}


char *ENT_LeerDesplazamientoPrueba()
{
	return cDesplazamientoPrueba;
}



int ENT_LimpiarDesplazamientoPrueba()
{
	FillBytes(cDesplazamientoPrueba,0,500,0);
	FillBytes(cNeutralActual,0,20,0);
	return 0;
}


char *ENT_NeutralActual()
{
	static char cNeutral[20]={0};
	
	Fmt(cNeutral,"%s<%s",cNeutralActual);
	
	return cNeutral;
}


int ENT_TorqueMotores(double dRotacional, double dLineal)
{
	dRotacional = dRotacional*1000.0;
	COM_EstructuraMensaje(2, 10462, 1, (int)dRotacional);
	
	dLineal = dLineal*1000.0;
	COM_EstructuraMensaje(2, 10462, 2, (int)dLineal);
	
	return 0;
}



