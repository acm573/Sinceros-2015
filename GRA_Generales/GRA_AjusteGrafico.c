/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Nombre del Aplicativo:	Módulo que administra las funciones que permiten
.							el ajuste gráfico de la representación en pantalla
.							de las posiciones de velocidades
. Archivo:					GRA_AjusteGrafico.c
. Propósito:				Funciones para el graficado de las posiciones de la
.							transmisión
. Lenguaje:					LabWindows/CVI 10.0.1 (434)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		04 de Marzo de 2013
. Responsable modificación: 
.
*******************************************************************************/

//includes
#include <formatio.h>
#include "toolbox.h"


//Variables globales
int iCanvasAncho=0;			//ancho en pixeles del control canvas
int iCanvasAlto=0;			//alto en pixeles del control canvas
int iCentroX=0;				//centro del canvas
int iCentroY=0;				//centro del canvas
int iFacX=0;
int iFacY=0;
ListType ListaPuntos=0;		//lista con los puntos a pintar
ListType ListaPintar=0;		//lista con los puntos a pintar
ListType ListaEtiqueta=0;	//lista con las etiquetas de cada punto
Point p; 					//objeto tipo punto



/*****************************************************************************
.
. Función C:			GRA_Limpiar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Limpia la lista de puntos
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	cero
. Fecha de creación:	04 de Marzo de 2013
.
*****************************************************************************/
void GRA_Limpiar()
{
	if (ListaPuntos != 0)
	{
		ListDispose(ListaPuntos);
		ListDispose(ListaEtiqueta);
	}
	
	//Inicializa la lista
	ListaPuntos = ListCreate (sizeof(Point));
	ListaEtiqueta = ListCreate (sizeof(char)*40);
}



int GRA_ActualizaEtiqueta(int iPosicion, char *pcCadena)
{
	ListReplaceItem(ListaEtiqueta, pcCadena, iPosicion);
	return 0;
}



/*****************************************************************************
.
. Función C:			GRA_InicializarCanvas
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Inicializa la información del canvas
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	cero
. Fecha de creación:	04 de Marzo de 2013
.
*****************************************************************************/
int GRA_InicializarCanvas(int iAlto, int iAncho, int iAltoC, int iAnchoC)
{
	iCanvasAlto = iAlto;
	iCanvasAncho = iAncho;
	
	iCentroX = iAncho/2;
	iCentroY = iAlto/2;
	
	iFacY = iAltoC/iAlto;
	iFacX = iAnchoC/iAncho;
	
	if (ListaPuntos != 0)
	{
		ListDispose(ListaPuntos);
	}
	
	//Inicializa la lista
	ListaPuntos = ListCreate (sizeof(Point));
	ListaEtiqueta = ListCreate (sizeof(char)*40);

	return 0;
}



/*****************************************************************************
.
. Función C:			GRA_InsertarPunto
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Inserta un punto en la lista de puntos
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	cero
. Fecha de creación:	04 de Marzo de 2013
.
*****************************************************************************/
char *GRA_InsertarPunto(int x, int y, char *pcEtiqueta)
{
	static char cPosicion[30];
	Point p;
	
	p.x = x;
	p.y = y;
	
	ListInsertItem (ListaPuntos, &p, END_OF_LIST);
	ListInsertItem (ListaEtiqueta, pcEtiqueta, END_OF_LIST);
	
	Fmt(cPosicion,"%s<%s%d%s%d%s","(",x,",",y,")");
	
	return cPosicion;
}


/*****************************************************************************
.
. Función C:			GRA_PintaPunto
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Crea la lista con los puntos ajustados y centrados
.						en las dimensiones del canvas
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	cero
. Fecha de creación:	04 de Marzo de 2013
.
*****************************************************************************/
void GRA_PintaPunto(Point p, Point a)
{
	Point x;
	
	p.x /= iFacX;
	p.y /= iFacY;
	
	x.y = p.y + a.y;
	x.x = p.x + a.x;
	
	ListInsertItem (ListaPintar, &x, END_OF_LIST);
	
}



/*****************************************************************************
.
. Función C:			GRA_CalculaAjuste
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Analiza el ajuste que se debe hacer 
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	cero
. Fecha de creación:	04 de Marzo de 2013
.
*****************************************************************************/
void GRA_CalculaAjuste(Point c, Point p, Point *a)
{
	//p.x /= iFacX;
	//p.y /= iFacY;
	
	//aqui debe determinar los ajustes para cada uno de los cuadrantes
	//primero analizamos el caso del eje x
	
	if (c.x < p.x)
	{
		//entonces el plano se encuentra cargado hacia el lado izquierdo
		//por lo tanto a la coordenada de p se debe sumar la diferencia
		a->x = c.x - p.x;
	}
	else
	{
		//se debe restar la diferencia 
		a->x = c.x - p.x;
	}
	
	
	if (c.y < p.y)
	{
		//entonces el plano se encuentra cargado hacia el lado izquierdo
		//por lo tanto a la coordenada de p se debe sumar la diferencia
		a->y = c.y - p.y;
	}
	else
	{
		//se debe restar la diferencia 
		a->y = c.y - p.y;
	}
}


void GRA_Invertir(Point *p, int iSentido)
{
	int iTmp=0;
	
	if (iSentido==1)
	{
		//Determina de que lado del eje X del centro se encuentra
		if (iCentroX > p->x)
		{
			p->x = (iCentroX - p->x) + iCentroX;
		}
		else
		{
			p->x = iCentroX - (p->x - iCentroX);
		}
		
		
	}
	
}



void GRA_Invertir2(Point *p, int iSentido)
{
	int iTmp=0;
	
	if (iSentido==1)
	{
		//Determina de que lado del eje X del centro se encuentra
		if (iCentroY > p->y)
		{
			p->y = (iCentroY - p->y) + iCentroY;
		}
		else
		{
			p->y = iCentroY - (p->y - iCentroY);
		}
		
		
	}
	
}


void GRA_Rotar(Point *p, int iRotar)
{
	if (iRotar == 1)
	{
	 	//invierte el eje X
		GRA_Invertir(p,1);
		GRA_Invertir2(p,1);
	}	
}



/*****************************************************************************
.
. Función C:			GRA_Dibujar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Retorna la lista de puntos que se deben dibujar con 
.						sus coordenadas ajustadas
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	cero
. Fecha de creación:	04 de Marzo de 2013
.
*****************************************************************************/
ListType GRA_Dibujar()
{
	int iNumero=0;
	Point p;
	Point c;
	Point a;
	int xMe=0;
	int xMa=0;
	int yMe=0;
	int yMa=0;
	int t=0;
	
	if (ListaPintar != 0)
		ListDispose(ListaPintar);
	
	ListaPintar = ListCreate (sizeof(Point));
	
	
	c.x = iCentroX;		//esto esta en pixeles
	c.y = iCentroY;		//esto está en pixeles
	
	
	iNumero = ListNumItems(ListaPuntos);
	
	if (iNumero>0)
	{
		if (iNumero > 1)
		{
			//se necesita localizar el centro de todos los puntos
			//comenzando por localizar el X menor y la X mayor
			//la Y menor y la Y mayor
			for (int i=0;i<iNumero;i++)
			{
				if (i==0)
				{
					ListGetItem(ListaPuntos, &p, 1);
					xMe=p.x;
					xMa=p.x;
					yMe=p.y;
					yMa=p.y;
				}
				else
				{
					ListGetItem(ListaPuntos, &p, i+1);
					
					if (xMe > p.x)
						xMe=p.x;
					
					if (xMa < p.x)
						xMa=p.x;
					
					if (yMe > p.y)
						yMe=p.y;
					
					if (yMa < p.y)
						yMa=p.y;
				}
			}
			
			//en este punto, se tienen las 4 coordenadas
			//y ahora se calcula el centro
			int x,y;
			
			x = (((xMa-xMe)/2)+xMe) / iFacX;
			y = (((yMa-yMe)/2)+yMe) / iFacY;
			
			Point a,g;
			
			g.x = x;
			g.y = y;
			
			GRA_CalculaAjuste(c,g,&a);
			
			for (int i=0;i<iNumero;i++)
			{
				ListGetItem(ListaPuntos, &p, i+1);
				GRA_PintaPunto(p,a);
			}
		}
		else
		{
			ListGetItem(ListaPuntos, &p, 1);
			GRA_CalculaAjuste(c,p,&a);
			GRA_PintaPunto(p,a);
		}
	}	
	
	return ListaPintar;
}
 


/*****************************************************************************
.
. Función C:			GRA_Dibujar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Retorna la lista de puntos que se deben dibujar con 
.						sus coordenadas ajustadas
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	cero
. Fecha de creación:	04 de Marzo de 2013
.
*****************************************************************************/
ListType GRA_Etiquetas()
{
	return ListaEtiqueta;
}










