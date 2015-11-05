/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Industrial
. Archivo:					TDM_Archivos.h
. Propósito:				Contiene las definiciones principales requeridas
.							para la implementación de TDM_Archivos.c
. Lenguaje:					LabWindows/CVI 13.0.0 (632)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		31 de Marzo de 2014
. Responsable modificación: 
.
*******************************************************************************/


typedef enum
{
	TDMS_Correcto,
	TDMS_Error
}stEdoTDM;

typedef enum
{
	TDM_Entero,
	TDM_Doble,
	TDM_Flotante,
	TDM_Char
}stTipoTDM;


//prototitpos
stEdoTDM TDM_DatosArchivo(char *pcRuta, char *pcPrefijo, char *pcDescripcion,
						char *pcTitulo, char *pcAutor, char *pcNombre);

stEdoTDM TDM_Crear(int iNumeroCanales, ...);
stEdoTDM TDM_NombreArchivo(char *pcNombre);
stEdoTDM TDM_Cerrar(void);
stEdoTDM TDM_Insertar(int iNumeroDatos, ...);
stEdoTDM TDM_Abrir(char *pcArchivo, unsigned int *piDatos);
stEdoTDM TDM_Leer(int iNumeroDatos, ...);
const char *TDM_MensajeError(void);



