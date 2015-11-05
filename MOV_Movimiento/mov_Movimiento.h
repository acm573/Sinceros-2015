#include "flexmotn.h"


typedef enum
{
	Tarjeta_01 = 1,
	Tarjeta_02 = 2,
	Tarjeta_03 = 3,
	Tarjeta_04 = 4,
	Tarjeta_05 = 5
}stTarjeta;

typedef enum
{
	INICIAR,
	FINALIZAR
}stProgramar;



typedef enum
{
	Rotacional = 1,
	Lineal = 2
}stEjes;



typedef enum
{
	BIT0,
	BIT1,
	BIT2,
	BIT3,
	BIT4,
	BIT5,
	BIT6,
	BIT7,
	BIT8,
	BIT9,
	BIT10,
	BIT11,
	BIT12,
	BIT13,
	BIT14,
	BIT15,
	BIT16,
	BIT17,
	BIT18,
	BIT19,
	BIT20,
	BIT21,
	BIT22,
	BIT23,
	BIT24,
	BIT25,
	BIT26,
	BIT27,
	BIT28,
	BIT29,
	BIT30,
	BIT31,
	TODOS
}stNumeroBit;


typedef enum
{
	Encendidos = 1,
	Apagados = 0
}stForzados;


typedef enum
{
	NINGUNA  = 0,
	ESCRIBIR = 10
}stAccion;


typedef enum
{
	OFF,
	ON,
	PULSO_CORTO,
	PULSO_LARGO,
	ACTIVADO,
	DESACTIVADO
}stEstadoBit;

typedef enum
{
	Puerto1 = 1,
	Puerto2 = 2,
	Puerto3 = 3,
	Puerto4 = 4,
	Puerto5 = 5,
	Puerto6 = 6,
	Puerto7 = 7,
	Puerto8 = 8
}stPuerto;


typedef struct
{
	int On[8];
	int Off[8];
}stPtoDigital;

typedef struct
{
	stTarjeta mov_tarjeta;		//referencia al número de tarjeta FlexMotion
	stPuerto mov_puerto;		//referncia al número del puerto de salida
	int *puerto;				//arreglo de bits para el puerto de salida
	stNumeroBit bitActivador;	//referencia al numero de bit que activa la captura
	stNumeroBit bitDato;		//referencia al numero de bit que contiene el dato
	double dTorqueDeseado;		//valor del torque que se desea aplicar
	double dTorqueReferencia;	//valor del torque de referencia 3Arms o 12Arms
}stConfiguraTorque;


int mov_Inicializar(stTarjeta tarjeta);
int mov_ConfiguraBitPuerto(int puerto[], int numeroBits, ...);

int mov_ConfiguraBitPuertoEscribe(stTarjeta mov_tarjeta, stPuerto mov_puerto, int puerto[], 
		stNumeroBit mov_bit, stEstadoBit mov_estado);
int mov_EscribePuerto(stTarjeta mov_tarjeta, stPuerto mov_puerto, int puerto[]);
int mov_EscribeTorque(stConfiguraTorque c);
int mov_VerificarPXI(stTarjeta mov_tarjeta);

