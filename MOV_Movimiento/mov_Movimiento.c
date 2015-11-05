#include <userint.h>
#include "asynctmr.h"
#include "mov_movimiento.h"
#include <formatio.h>
#include <utility.h>


//Variables para el proceso de escritura del torque
stConfiguraTorque configT;		//referencia que contiene la configuración del torque a escribir
char cadenaTorque[20]={0};		//cadena de unos y ceros equivalentes al torque en 16 bits

/*
.
. Pasa un valor entero a su equivalente binario en 16 bits
.
*/
const char *mov_Byte_to_Binary(int x)
{
    static char b[20];
    b[0] = '\0';

    int z;
    for (z = 32768; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}



/*
. Inicializa el controlador de movimiento con los parámetros
. especificados en el MAX
*/
int mov_Inicializar(stTarjeta mov_tarjeta)
{
	int iError=0;
	u16 commandID;
	u16 resourceID;
	i32 errorCode;
	i8 *c=NULL;
	u32 *s=NULL;
	
	
	
	
	iError = flex_initialize_controller(mov_tarjeta,NULL);
	
	/*if (iError<0)
	{
		//iError = nimcResetController(2);
		nimcResetController
		iError = flex_initialize_controller(mov_tarjeta,NULL);
	}*/
	
	return 0;
}



/*
. Inicializa el controlador de movimiento con los parámetros
. especificados en el MAX
*/
int mov_VerificarPXI(stTarjeta mov_tarjeta)
{
	f64 temperatura=0;
	int iEstado=0;
	
	
	mov_Inicializar(mov_tarjeta);
	SetBreakOnLibraryErrors (0);
	iEstado = flex_read_board_temperature (mov_tarjeta, &temperatura);
	SetBreakOnLibraryErrors (1);
	
	if (iEstado==0)
		iEstado=1;
	else
		iEstado=0;
	
	return iEstado;
}


/*
.
. Configura un arreglo de enteros de 8 posiciones, con un valor de 0-1
. para especificar lo que se desea escribir en el puerto
.
*/
int mov_ConfiguraBitPuerto(int puerto[], int numeroBits, ...)
{
	va_list pa;
	
	va_start(pa, numeroBits);
	
	for (int i=0;i<numeroBits;i++)
	{
		int mov_bit=0;
		int mov_estado=0;
		
		mov_bit = va_arg(pa,stNumeroBit);
		mov_estado = va_arg(pa,stEstadoBit);
		
		if (mov_bit != TODOS)
			puerto[mov_bit]=mov_estado;
		else
		{
			for (int i=0;i<8;i++)
				puerto[i]=mov_estado;
		}
	}
	return 0;
}


/*
.
. Configura un arreglo de enteros de 8 posiciones, con un valor de 0 - 1
. y al finalizar escribe la información a la tarjeta y puerto indicado
.
*/
int mov_ConfiguraBitPuertoEscribe(stTarjeta mov_tarjeta, stPuerto mov_puerto, int puerto[], 
		stNumeroBit mov_bit, stEstadoBit mov_estado)
{
	if (mov_bit != TODOS)
		puerto[mov_bit]=mov_estado;
	else
	{
		for (int i=0;i<8;i++)
			puerto[i]=mov_estado;
	}
	
	mov_EscribePuerto(mov_tarjeta, mov_puerto, puerto);
	return 0;
}




/*
.
. Escribe un arreglo de enteros de 8 bits como el valor deseado en el
. puerto digital indicado
.
*/
int mov_EscribePuerto(stTarjeta mov_tarjeta, stPuerto mov_puerto, int puerto[])
{
	u8 iDecimal=0;
	u8 iDecimal_Neg=0;
	
	//obtiene el valor decimal del arreglo pasado por referencia
	for (int i=0;i<8;i++)
		iDecimal += (puerto[i] << i);
	
	iDecimal_Neg = 0xFF - iDecimal;
	
	flex_set_port(mov_tarjeta, mov_puerto, iDecimal, iDecimal_Neg, 0xFF);
	
	return iDecimal;
}




