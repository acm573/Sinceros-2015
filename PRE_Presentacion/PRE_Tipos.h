#ifndef __PRE_Tipos__
#define __PRE_Tipos__


#include "toolbox.h"

#define ANCHO_BOTON_MENU 	46
#define ANCHO_MENU 			170
#define PASO_ANIMACION 		20
#define ANCHO_ROTACIONAL 	35000	//46610
#define ALTO_LINEAL 		10000	//13439
#define TOTAL_PLANOS		8
#define TOTAL_VELOCIDADES	8
#define TOTAL_NEUTRALES		8
#define TOTAL_ANALISIS		5
#define TIEMPO_RECORRIDO_PRUEBA 2.0		//segundos
#define DISTANCIA_DETECCION 500

typedef enum
{
	CTRL_NUEVO_ENTRENAMIENTO,
	CTRL_SIN_TRANSMISIONES,
	CTRL_INICIA_ENTRENAMIENTO,
	CTRL_CON_TRANSMISIONES,
	CTRL_ENTRENAMIENTO_POSICION_CERO,
	CTRL_ENTRENAMIENTO_TERMINADO,
	CTRL_ACEPTA_CONFIGURACION,
	CTRL_INICIA_CONFIGURACION,
	CTRL_IR_POSICION_INICIAL,
	CTRL_RECORRIDO_PRUEBA,
	CTRL_RECORRIDO_FINALIZADO,
	CTRL_ESPERA_BOTON_INICIO_PRUEBA
}stEntrenar;

typedef enum 
{
	HABILITAR_MOVIMIENTO,
	DESHABILITAR_MOVIMIENTO
}stEdoEjecucion;


typedef enum
{
	ENTRENANDO,
	CONFIGURANDO,
	RECORRIDO
}stPintaMarcas;

typedef enum
{
	PRUEBA_RECORRIDO_INICIO,
	PRUEBA_RECORRIDO_PUNTOS,
	PRUEBA_LIBERADA
}stTipoRecorrido;

typedef enum
{
	SIN_RECORRIDO,
	CON_RECORRIDO
}stEdoEntrenamiento;

typedef enum
{
	VELOCIDAD=0,
	NEUTRAL=-1,
	REVERSA=1,
	EMBRAGUE=10,
	DESEMBRAGUE=20,
	ROTACIONAL=30,
	LINEAL=40
}stTipo;


typedef enum
{
	CON_POSICIONES,
	SIN_POSICIONES
}stPosiciones;

typedef enum
{
	MOT_HABILITAR,
	MOT_DESHABILITAR,
	MOT_POSICION_CERO,
	MOT_ABB_HABILITAR,
	MOT_ABB_DESHABILITAR,
	MOT_POSICION_ACTUAL,
	MOT_GIRO_HORARIO,
	MOT_GIRO_ANTIHORARIO
}stMotores;

typedef enum
{
	DIG_REARME,
	DIG_PARO_EMERGENCIA
}stDigitales;



typedef enum
{
	INSERTAR,
	INICIALIZAR
}stTabla;


typedef struct
{
	int iLineal;
	int iRotacional;
	char cDescripcion[20];
	int iTipo;
	int iPlano;	
	int iNumeroVelocidad;  //implementar en la tabla un ring con numero de velocidad incremental y reversa -1
	int x;
	int y;
}stPosicion;


									 
//referencias de paneles
int iPanelPrincipal;
int iPanelCatTransmisiones;
int iPanelMenuPrincipal;
int iPanelEntrenamiento;


//definiciones
typedef enum
{
	MOSTRAR,
	OCULTAR
}stMenuAcciones;


typedef stEntrenar tipo;




#endif
