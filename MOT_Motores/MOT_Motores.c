#include <userint.h>
#include "PRE_Principal.h"
#include "mov_movimiento.h"
#include "com_comunicacion.h"
#include "pre_tipos.h"
#include "dig_digitales.h"

//prototipo de funciones


int puerto2[8]={0};
int puerto3[8]={0};
int puerto4[8]={0};


int MOT_CondicionesIniciales()
{
	/*
	Puerto 3 Motion
		0 Enable
		1 LImites
		2 Limites
		3 Reset Rotacional
		4 Enable
		5 limites
		6 limites
		7 Reset Lineal
	*/

	mov_Inicializar(Tarjeta_01);

	mov_ConfiguraBitPuerto(puerto3, 1, TODOS, OFF);
	//mov_ConfiguraBitPuerto(puerto3, 6, BIT0,ON,BIT1,ON,BIT2,ON,BIT4,ON,BIT5,ON,BIT6,ON);
	mov_ConfiguraBitPuerto(puerto3, 4, BIT1,ON,BIT2,ON,BIT5,ON,BIT6,ON);

	mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);

	//Limpia cualquier error que pueda tener el drive
	mov_ConfiguraBitPuerto(puerto3, 2, BIT3,ON,BIT7,ON);
	mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);

	mov_ConfiguraBitPuerto(puerto3, 2, BIT3,OFF,BIT7,OFF);
	mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);

	mov_ConfiguraBitPuerto(puerto4, 1, TODOS,OFF);
	mov_EscribePuerto(Tarjeta_01, Puerto4, puerto4);
	
	return 0;
}



int MOT_CondicionSalida()
{
	mov_ConfiguraBitPuerto(puerto3, 1, TODOS, OFF);
	mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);
	
	return 0;
}



int MOT_EstadoMotores(stMotores Modo)
{
	if (Modo==MOT_DESHABILITAR)
	{
		mov_ConfiguraBitPuerto(puerto3, 2, BIT0,OFF,BIT4,OFF);
		mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);
		Delay(.5);
	}
	else
	{
		mov_ConfiguraBitPuerto(puerto3, 2, BIT0,ON,BIT4,ON);
		mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);
		Delay(.5);
	}
	return 0;
}



int MOT_PosicionActual(long *iRotacional, long *iLineal)
{
	flex_read_pos_rtn (1, NIMC_AXIS1, iRotacional);
	flex_read_pos_rtn (1, NIMC_AXIS2, iLineal);

	return 0;
}


int MOT_SentidoGiroABB(stMotores Modo)
{
	if (Modo == MOT_GIRO_HORARIO)
	{
		mov_ConfiguraBitPuerto(puerto4, 1, BIT7,ON);
	}
	else
	{
		mov_ConfiguraBitPuerto(puerto4, 1, BIT7,OFF);
	}
	mov_EscribePuerto(Tarjeta_01, Puerto4, puerto4);
	

	return 0;
}


int MOT_VelocidadABB(double dVelocidadComando)
{
	double dVoltaje=0;
			
	dVoltaje = (10.0/3560.0)*dVelocidadComando;
	
	//Inicia la configuración de la tarea
	
	DAQmxCreateTask ("MotorABB", &miHandle);
	
	DAQmxCreateAOVoltageChan (miHandle, "Dev1/ao1", "", -10.0, 10.0,
							  DAQmx_Val_Volts, "");
	
	DAQmxWriteAnalogScalarF64 (miHandle, 1, 10.0, dVoltaje, 0);
	
	DAQmxStartTask (miHandle);
	DAQmxStopTask (miHandle);
	DAQmxClearTask (miHandle);
			
	return 0;
}


int MOT_MotorABB(stMotores Modo)
{
	if (Modo == MOT_ABB_HABILITAR)
	{
		mov_ConfiguraBitPuerto(puerto4, 1, BIT6,ON);
		mov_ConfiguraBitPuerto(puerto2, 1, BIT7,ON);
	}
	else
	{
		MOT_VelocidadABB(0.0);
		mov_ConfiguraBitPuerto(puerto4, 1, BIT6,OFF);
		mov_ConfiguraBitPuerto(puerto2, 1, BIT7,OFF);
	}
	mov_EscribePuerto(Tarjeta_01, Puerto4, puerto4);
	mov_EscribePuerto(Tarjeta_01, Puerto2, puerto2);
	
	return 0;
}


int MOT_PosicionMotores(stMotores Modo)
{
	long iRotacional=0;
	long iLineal=0;
	double dValor=0;
	stConfiguraTorque tTorque;
	
	
	if (Modo == MOT_POSICION_ACTUAL)
		MOT_PosicionActual(&iRotacional, &iLineal);

	
	COM_EstructuraMensaje(2, 10462, 1, 0);
	COM_EstructuraMensaje(2, 10462, 2, 0);
	
	Delay(1.0);
	
	mov_Inicializar(Tarjeta_01);

	flex_reset_pos (1, NIMC_AXIS1, iRotacional, 0, 0xFF);
	flex_reset_pos (1, NIMC_AXIS2, iLineal, 0, 0xFF);
	
	mov_ConfiguraBitPuerto(puerto3, 1, TODOS, OFF);
	mov_ConfiguraBitPuerto(puerto3, 4, BIT1,ON,BIT2,ON,BIT5,ON,BIT6,ON);

	mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);

	//Limpia cualquier error que pueda tener el drive
	mov_ConfiguraBitPuerto(puerto3, 2, BIT3,ON,BIT7,ON);
	mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);

	mov_ConfiguraBitPuerto(puerto3, 2, BIT3,OFF,BIT7,OFF);
	mov_EscribePuerto(Tarjeta_01, Puerto3, puerto3);


	mov_ConfiguraBitPuerto(puerto4, 1, TODOS,OFF);
	mov_EscribePuerto(Tarjeta_01, Puerto4, puerto4);

	return 0;
}



