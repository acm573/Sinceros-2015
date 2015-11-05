/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Nombre del Aplicativo:	Módulo que analiza los patrones de desplazamiento
.							solicitados
. Archivo:					PAT_PatronesMovimiento.c
. Propósito:				Contiene diversas funciones para el análisis de los
.							patrones de cambio solicitados para el sistema
. Lenguaje:					LabWindows/CVI 10.0.1 (434)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		18 de Febrero de 2013
. Responsable modificación: 
.
*******************************************************************************/

//Includes
#include <formatio.h>
#include "toolbox.h"

//Definiciones
typedef struct
{
	int iLineal;
	int iRotacional;
	char cDescripcion[20];
	int iTipo;
	int iPlano;									
	int x;
	int y;
}stPosicion;

//prototipo de funciones
void * UTL_Split (char *pcCadena, char *pcPatron, char (*c)[30], int *total);


//Variables gloables locales
ListType miLista;



int PAT_ObtienePlano(char *pcVel)
{
	stPosicion p;
	char cVelocidad[10]={0};
	static int iElementos=0;
	
	p.iPlano=0;
	//obtiene el indice de la velocidad
	iElementos = ListNumItems(miLista);
	
	for (int i=0;i<iElementos;i++)
	{
		ListGetItem(miLista, &p, i+1);
		
		if (CompareStrings(p.cDescripcion, 0, pcVel, 0, 0)==0)
		{
			i=iElementos;
		}
	}
	
	return p.iPlano;
}


char *PAT_ObtieneNeutral(int iPlano)
{
	stPosicion p;
	static int iElementos=0;
	static char c[20]={0};
	
	
	iElementos = ListNumItems(miLista);
	
	for (int i=0;i<iElementos;i++)
	{
		ListGetItem(miLista, &p, i+1);
		
		if (iPlano == p.iPlano)
		{
			//Ahora busca la neutral
			if (p.iTipo == -1)
			{
				//se ha encontrado la neutral
				i=iElementos;
				Fmt(c,"%s<%s",p.cDescripcion);
			}
		}
	}
	
	return c;
}


int PAT_AnalizarPatronCambioFinal(char *cPatronCambio)
{
	char cNuevoPatron[260]={0};
	char cLista[30][30]={0};
	char cAnt[10];
	int iTotal=0;
	
	UTL_Split(cPatronCambio, "->", cLista, &iTotal);
	
	strcat(cNuevoPatron,cLista[0]);
	strcat(cNuevoPatron,"->");
	
	Fmt(cAnt,"%s<%s",cLista[0]);
	
	for (int i=1;i<iTotal;i++)
	{
		//Verifica que sean diferentes
		if (CompareStrings (cAnt, 0, cLista[i], 0, 0)!=0)
		{
			strcat(cNuevoPatron,cLista[i]);
			strcat(cNuevoPatron,"->");
			
			Fmt(cAnt,"%s<%s",cLista[i]);
		}
	}
	
	Fmt(cPatronCambio,"%s<%s",cNuevoPatron);
	
	return 0;
}


char *PAT_AnalizarMovimientoLista(char (*c)[30])
{
	char cVelocidad[10]={0};
	static char cPatronCambio[100]={0};
	char cInicio[10]={0};
	int iPlano1=0;
	int iPlano2=0;
	
	FillBytes(cPatronCambio, 0, 50,0);
	
	//Analiza si los 2 elementos pertenecen al mismo plano
	Fmt(cVelocidad,"%s<%s",c[0]);
	iPlano1 = PAT_ObtienePlano(cVelocidad);
	
	strcat (cInicio, PAT_ObtieneNeutral(iPlano1));
	strcat (cPatronCambio, cInicio);
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, cVelocidad);
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, cInicio);
	strcat (cPatronCambio, "->");
	
	Fmt(cVelocidad,"%s<%s",c[1]);
	iPlano2 = PAT_ObtienePlano(cVelocidad);
	
	strcat (cPatronCambio, PAT_ObtieneNeutral(iPlano2));
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, cVelocidad);
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, PAT_ObtieneNeutral(iPlano2));
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, cInicio);
	
	PAT_AnalizarPatronCambioFinal(cPatronCambio);
	
	return cPatronCambio;
}


char *PAT_AnalizarMovimientoLista2(char (*c)[30])
{
	static char cDetalle[500]={0};
	char cVelocidad[10]={0};
	char cPatronCambio[50]={0};
	char cInicio[10]={0};
	int iPlano1=0;
	int iPlano2=0;
	
	FillBytes(cDetalle,0,500,0);
	
	//Analiza si los 2 elementos pertenecen al mismo plano
	Fmt(cVelocidad,"%s<%s",c[0]);
	iPlano1 = PAT_ObtienePlano(cVelocidad);
	
	strcat (cInicio, PAT_ObtieneNeutral(iPlano1));
	strcat (cPatronCambio, cInicio);
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, cVelocidad);
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, cInicio);
	strcat (cPatronCambio, "->");
	
	Fmt(cVelocidad,"%s<%s",c[1]);
	iPlano2 = PAT_ObtienePlano(cVelocidad);
	
	strcat (cPatronCambio, PAT_ObtieneNeutral(iPlano2));
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, cVelocidad);
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, PAT_ObtieneNeutral(iPlano2));
	strcat (cPatronCambio, "->");
	strcat (cPatronCambio, cInicio);
	
	PAT_AnalizarPatronCambioFinal(cPatronCambio);
	
	//SetCtrlVal (panelHandle, PANEL_strDetalle, cPatronCambio);
	
	Fmt(cDetalle,"%s<%s",cPatronCambio);
	
	return cDetalle;
}


/*****************************************************************************
.
. Función C:			PAT_AnalizaMovimiento 
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Analiza el movimiento del patron de cambio indicado y
.						retorna la cadena con la secuencia de desplazamientos
.						que corresponden para el patron solicitado
. Parámetro de entrada:	char *pcPatron | cadena con el patron de cambio
.						ListType Lista | referencia a la lista que contiene
.										 las posiciones entrenadas vigentes
.										 en memoria del sistema
. Parámetro de salida:	char * | Retorna la cadena con la secuencia de la
.								 trayectoria que aplica para el patron de 
.								 cambio solicitado
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
char *PAT_AnalizaMovimiento(char *pcPatron, ListType Lista)
{
	static char cRuta[100]={0};
	char c[30][30]={0};
	int iTotal=0;
	
	UTL_Split(pcPatron,"->", c, &iTotal);
	
	miLista = Lista;
	
	Fmt(cRuta,"%s<%s",PAT_AnalizarMovimientoLista(c));
		
	return cRuta;
}


double PAT_Distancia(Point a, Point b)
{
	double dDistancia=0;
	
	//Ahora debe calcula la distancia entre 2 puntos y determinar la velocidad
	//a la que debe viajar para cubrir el recorrido en el tiempo indicado
	dDistancia = sqrt( pow((a.x - b.x),2) + pow((a.y-b.y),2) );
	
	return dDistancia;
}


/*****************************************************************************
.
. Función C:			PAT_PosicionCercaDe 
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Determina cerca de qué velocidad se encuentra la
.						palanca de velocidades a fin de poder mover de forma
.						automática a la palanca a la posicion de inicio del
.						movimiento
. Parámetro de entrada:	double dCuentas | margen de error permitido
.						ListType Lista | referencia a la lista que contiene
.										 las posiciones entrenadas vigentes
.										 en memoria del sistema
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
char *PAT_PosicionCercaDe(double dCuentas, ListType Lista, int iPosLineal, 
	int iPosRotacional)
{
	//revisa si existe una lista de posiciones cargadas en memoria
	int iElementos=0;
	static char cCerca[20]={0};
	
	miLista = Lista;

	FillBytes(cCerca, 0, 20, 0);
	iElementos = ListNumItems (miLista);
	
	if (iElementos>0)
	{
		Point a;
		Point b;
		
		a.y = iPosLineal;
		a.x = iPosRotacional;
		
		stPosicion p;
		
		for (int i=0;i<iElementos;i++)
		{
			ListGetItem(miLista, &p, i+1);	
			
			b.x = p.iLineal;
			b.y = p.iRotacional;
			
			if(PAT_Distancia(a,b)<=dCuentas)
			{
				i=iElementos;
				Fmt(cCerca,"%s<%s",p.cDescripcion);
			}
		}
		
	}
	return cCerca;
}


char *PAT_RutaAcercar(char *pcPatron, char *pcInicio, ListType Lista)
{
	char cPatron[100]={0};
	char cInicio[20]={0};
	char cDestino[20]={0};
	char *c;
	int iControl=0;
	static char cRutaAcercar[100]={0};
	
	miLista = Lista;
	
	FillBytes(cRutaAcercar,0,100,0);
	
	Fmt(cPatron,"%s<%s",pcPatron);
	Fmt(cInicio,"%s<%s",pcInicio);
	
	if (strlen(cPatron)>0)
	{
		char cLista[30][30]={0};
		int iTotal=0;
		
		UTL_Split(cPatron, "->", cLista, &iTotal);
		
		Fmt(cPatron,"%s<%s%s%s",cInicio,"->",cLista[0]);
		Fmt(cDestino,"%s<%s",cLista[0]);
		
		UTL_Split(cPatron, "->", cLista, &iTotal);
		
		c=PAT_AnalizarMovimientoLista2(cLista);
		
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
		
		Fmt(cRutaAcercar,"%s<%s",cPatron);
		
		//SetCtrlVal (panelHandle, PANEL_strRutaAcercar, cPatron);
		
		//EjecucionPorPatron2();
	}
	return cRutaAcercar;
}
