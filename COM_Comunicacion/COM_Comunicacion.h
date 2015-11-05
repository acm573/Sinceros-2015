//Definicion de las estructuras globales
typedef struct
{
	unsigned char ucComando;
	char cDato[8];
				/*
											---------------------------------
											|   |   |   |   |   |   |   |   |
											|   |   |   |   |   |   |   |   |
											---------------------------------
					  						  7   6   5   4   3   2   1   0
			  													  
					  						  |   |   |   |	  |   |   |   |   
					Byte 0 de iValor 	 <-----   |   |   |   |   |	  |   |   	
					Byte 1 de iValor     <--------	  |   |   |	  |	  |   |
					Byte 2 de iValor     <------------	  |   |   |	  |   |
					Byte 3 de iValor     <----------------	  |   |	  |	  |
					Byte bajo de Direccion   <----------------	  |   |   |
					Byte alto de Direccion   <--------------------	  |   |
					Numero de controlador 	 <------------------------	  |
					Comando a ejecutar    	 <----------------------------
				*/

}stRegistros;


typedef struct
{
	int iRespuesta;
	struct
	{
		unsigned char ucComando;
		int iDireccion;
		unsigned char ucControlador;
		int iValor;
	}Detalle;
}stRespuesta;

typedef enum
{
	SIN_CONEXION,
	CONECTADO,
	E_RROR
}stEstado;

typedef enum
{
	CONTROLADOR_1 = 1,
	CONTROLADOR_2 = 2
}stControlador;

typedef enum
{
	BITS_16 = 16,
	BITS_32 = 32
}stLongitud;

typedef struct
{
	int iEnviandoCmd;
	int iCargandoCmd;
}stCtrlCom;


//Definicion de la variables globales
stCtrlCom EdoComunicacion;
int iPanelMonitor;

//Definicion de prototipo de funciones
int COM_IniciaClienteTCP(void);
stRespuesta COM_Respuesta(void);
int COM_InsertarComando(unsigned char ucComando, char cMensaje[]);
stEstado COM_Estado(void);
int COM_ComandoUnico(char cMensaje[]);
int COM_EstructuraMensaje(unsigned char ucComando, unsigned int uiDireccion, unsigned char ucControlador,
		int iValor);
int COM_Comandos(void);
int COM_IniciaAplicacion(void);
int COM_FinalizaAplicacion(void);
