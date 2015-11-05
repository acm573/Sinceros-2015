/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Industrial
. Archivo:					TDM_Archivos.c
. Propósito:				Contiene las funciones que permiten el manipular
.							archivos TDMS de manera más transparente y sencilla.
. Lenguaje:					LabWindows/CVI 13.0.0 (632)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		31 de Marzo de 2014
. Responsable modificación: 
.
*******************************************************************************/
#include <analysis.h>
#include <cvitdms.h>
#include <ansi_c.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h> 
#include "tdm_archivos.h"

//Definición de estructuras
typedef struct
{
	int iNumeroCanales;				//numero de canales a inlcuir en el archivo
	int *iTipoDatos;				//lista con los tipos de datos de los canales
	char **pcTitulosCanales;		//nombre de los canales
	char cArchivoNuevo[260];		//nombre del archivo creado
	char cDescripcion[250];			//contiene la descripcion deseada para el archivo
	char cTitulo[250];				//titulo para el archivo
	char cNombre[250];				//nombre para el archivo
	char cAutor[250];				//autor del documento
	int iIndice;					//indice de datos
	unsigned int iTotalDatos;		//total de datos por canal
	TDMSFileHandle TdmsHandle;		//manejador del archivo TDMS
	TDMSChannelGroupHandle GrupoHandle;	//manejador del grupo
	TDMSChannelHandle *CanalHandle;	//lista de manejadores de canales
	int iConfigurado;				//bandera para saber si se ha configurado el archivo
	int iTdmsActivo;				//bandera para saber si se ha creado un TDMS
}stTDMS;

//globales
char cRuta[250]={0};			//mantiene la ruta donde se ubicará el archivo
char cPrefijo[50]={0};			//contiene el prefijo a utilizar en el nombre
static stTDMS Tdms;				//estructura con el detalle del archivo TDMS
char cMsjError[1000]={0};		//mensaje de error del sistema


/*****************************************************************************
.
. Función C:			TDM_MensajeError
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Retorna la cadena con el mensaje de error.
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	Referencia a la cadena de caracteres con el mensaje
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
const char *TDM_MensajeError()
{
	return cMsjError;
}

/*****************************************************************************
.
. Función C:			TDM_Error
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Almacena el mensaje de error.
. Parámetro de entrada:	Cadena con el mensaje de error
. Parámetro de salida:	Valor TDMS_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_Error(char *pcMensaje)
{
	Fmt(cMsjError,"%s<%s",pcMensaje);
	
	return TDMS_Error;
}

/*****************************************************************************
.
. Función C:			TDM_LimpiaDatosAnteriores
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Limpia las variables y manejadores utilizados para la
.						manipulación de los archivos TDMS
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_LimpiaDatosAnteriores()
{
	stEdoTDM estado = TDMS_Correcto;	//estado de la función
	int i=0;	//para uso del for
	
	//limpia la estructura tdms
	if (Tdms.iTipoDatos != NULL)
	{
		free(Tdms.iTipoDatos);
		Tdms.iTipoDatos=NULL;
	}
	
	if (Tdms.pcTitulosCanales != NULL)
	{
		for (i=0;i<Tdms.iNumeroCanales; i++)
			free(Tdms.pcTitulosCanales[i]);
		
		free(Tdms.pcTitulosCanales);
		Tdms.pcTitulosCanales=NULL;
	}
	
	if (Tdms.CanalHandle != NULL)
	{
		free (Tdms.CanalHandle);
		Tdms.CanalHandle = NULL;
	}
	
	Tdms.iNumeroCanales = 0;
	Tdms.iIndice=0;
	Tdms.iTotalDatos = 0;
	Tdms.iConfigurado = 0;
	Tdms.iTdmsActivo = 0;
	
	
	FillBytes(Tdms.cArchivoNuevo,0,260,0);
	if (Tdms.TdmsHandle!=NULL)
	{
		Tdms.TdmsHandle=NULL;
	}
	
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_ConsecutivoTDMS
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Calcula el siguiente número consecutivo para el
.						archivo TDMS que se va a crear. El numero se determina
.						al obtener el número que se contiene el archivo que se
.						ha pasado por parámetro.
. Parámetro de entrada:	char *pcArchivo | Recibe el nombre del ultimo archivo
.										  TDMS que se encontró.
. Parámetro de salida:	Número entero para el nuevo archivo a crear.
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
int TDM_ConsecutivoTDMS(char *pcArchivo)
{
	int iNumero=0;			//contiene el consecutivo del archivo
	char cNumero[20]={0};	//fragmento de cadena con el numero
	
	CopyString (cNumero, 0, pcArchivo, strlen(pcArchivo)-9, 4);
	Fmt(&iNumero,"%d<%s",cNumero);
	
	return iNumero+1;
}

/*****************************************************************************
.
. Función C:			TDM_SiguienteTDMS
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Localiza el último archivo TDMS que se haya generado
.						en la ruta de almacenamiento que se configuró previa-
.						mente.
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_SiguienteTDMS()
{
	stEdoTDM estado = TDMS_Correcto;	//estado de la función
	char cBuscar[260]={0};		//contiene el patron de busqueda
	char cEncontrado[260]={0};	//contiene el archivo encontrado
	int iEncontrado=0;			//indica el estado de la busqueda
	int iNumero=1;				//define el consecutivo a utilizar
	char cConsecutivo[20]={0};	//numero consecutivo
	
	
	Fmt(cBuscar,"%s<%s%s",cRuta,"\\*.tdms");
	//localiza el primer archivo con la estructura propuesta
	iEncontrado = GetFirstFile (cBuscar, 1, 0, 0, 0, 0, 0, cEncontrado);
	
	while (iEncontrado == 0)
		iEncontrado = GetNextFile (cEncontrado);
	
	if (strlen(cEncontrado)>0)
	{
		//se necesita obtener el consecutivo del archivo
		iNumero = TDM_ConsecutivoTDMS(cEncontrado);
	}

	Fmt(cConsecutivo,"%s<%d[w4p0]",iNumero);
	Fmt(Tdms.cArchivoNuevo,"%s<%s%s%s%s%s",cRuta,"\\",cPrefijo,cConsecutivo,".tdms");
	
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_TipoDato
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Esta función retorna a partir de un valor de tipo
.						stTipoTDM, su equivalente en tipo TDMSDataType
. Parámetro de entrada:	int iTipo | Valor de tipo stTipoTDM
. Parámetro de salida:	Valor de tipo TDMSDataType
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
TDMSDataType TDM_TipoDato(int iTipoDato)
{
	TDMSDataType Tipo = TDMS_Int8;	//para retorno del tipo de dato
	
	switch (iTipoDato)
	{
		case TDM_Entero:
			Tipo = TDMS_Int32;
			break;
		case TDM_Doble:
			Tipo = TDMS_Double;
			break;
		case TDM_Flotante:
			Tipo = TDMS_Float;
			break;
		case TDM_Char:
			Tipo = TDMS_Int8;
			break;
	}

	return Tipo;
}

/*****************************************************************************
.
. Función C:			TDM_TipoDatoRev
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Esta función retorna a partir de un valor de tipo
.						TDMSDataType, su equivalente en stTipoTDM.
. Parámetro de entrada:	TDMSDataType iTipoDato | Valor de tipo TDMSDataType
. Parámetro de salida:	Valor de tipo stTipoTDM
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stTipoTDM TDM_TipoDatoRev(TDMSDataType iTipoDato)
{
	stTipoTDM Tipo = TDM_Entero;
	
	switch (iTipoDato)
	{
		case TDMS_Int32:
			Tipo = TDM_Entero;
			break;
		case TDMS_Double:
			Tipo = TDM_Doble;
			break;
		case TDMS_Float:
			Tipo = TDM_Flotante;
			break;
		case TDMS_Int8:
			Tipo = TDM_Char;
			break;
	}

	return Tipo;
}

/*****************************************************************************
.
. Función C:			TDM_CrearNuevoArchivo
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Función que crea un nuevo archivo TDMS, utilizando
.						la información que se ha cargado previamente.
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_CrearNuevoArchivo()
{
	stEdoTDM estado = TDMS_Correcto;	//estado de esta funcion
	int iError=0;						//estado de error de funciones
	int i=0;	//para uso del for
	
	
	if (Tdms.iConfigurado == 1)
	{
		SetBreakOnLibraryErrors (0);
		//crea el documento
		iError = TDMS_CreateFileEx (Tdms.cArchivoNuevo, TDMS_Streaming2_0,
									TDMS_ByteOrderLittleEndian, 0,
									Tdms.cNombre, Tdms.cDescripcion,
									Tdms.cTitulo, Tdms.cAutor,
									&Tdms.TdmsHandle);
		SetBreakOnLibraryErrors (1);
	
		if (iError == 0)
		{
			//crea un grupo de datos
			TDMS_AddChannelGroup (Tdms.TdmsHandle, "Grupo", "Descripcion",
								  &Tdms.GrupoHandle);

			for (i=0;i<Tdms.iNumeroCanales;i++)
			{
				TDMS_AddChannel (Tdms.GrupoHandle, TDM_TipoDato(Tdms.iTipoDatos[i]),
								 Tdms.pcTitulosCanales[i], "Descripcion", "Unidad",
								 &Tdms.CanalHandle[i]);
			}
			Tdms.iTdmsActivo = 1;
		}
		else
		{
			estado = TDM_Error(TDMS_GetLibraryErrorDescription(iError));
		}
	}
	else
	{
		estado = TDM_Error("No se ha configurado aún la información del TDMS.");
	}
	
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_DatosArchivo
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Recibe la información que se utilizará para configurar
.						la cabecera del archivo TDMS que se creará.
.						Dentro de la información, se especifica la ruta en
.						donde se desea almacenar el archivo.
. Parámetro de entrada:	char *pcRuta |	Contiene la ruta donde se desea guardar
.										el archivo TDMS.
.						char *pcPrefijo | Prefijo que se puede utilizar en el
.										  nombre del archivo.
.						char *pcDescripcion | Descripción que se incluirá en la
.											  cabecera del archivo TDMS.
.						char *pcTitulo | Contiene una cadena que será utilizada
.										 como título para el archivo dentro de la
.										 cabecera del documento.
						char *pcAutor | Nombre del autor del documento.
						char *pcNombre | Nombre que describa el archivo, misma
.										 que se incluirá en la cabecera del TDMS.
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_DatosArchivo(char *pcRuta, char *pcPrefijo, char *pcDescripcion,
						char *pcTitulo, char *pcAutor, char *pcNombre)
{
	stEdoTDM estado = TDMS_Correcto;	//estado de la función
	
	TDM_LimpiaDatosAnteriores();
	
	Fmt(cRuta,"%s<%s",pcRuta);
	Fmt(cPrefijo,"%s<%s",pcPrefijo);
	
	//Determina la información que se utilizará para crear el archivo
	if (strlen(pcDescripcion)>0)
		Fmt(Tdms.cDescripcion,"%s<%s",pcDescripcion);
	else
		Fmt(Tdms.cDescripcion,"%s<%s","Descripción");
	
	if (strlen(pcTitulo)>0)
		Fmt(Tdms.cTitulo,"%s<%s",pcTitulo);
	else
		Fmt(Tdms.cTitulo,"%s<%s","Título");
	
	if (strlen(pcAutor)>0)
		Fmt(Tdms.cAutor,"%s<%s",pcAutor);
	else
		Fmt(Tdms.cAutor,"%s<%s","CIDESI");
	
	if (strlen(pcNombre)>0)
		Fmt(Tdms.cNombre,"%s<%s",pcNombre);
	else
		Fmt(Tdms.cNombre,"%s<%s","CIDESI");
	
	Tdms.iConfigurado = 1;
	
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_Crear
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Crea el archivo TDMS utilizando la información que
.						se asignó.
. Parámetro de entrada:	int iNumeroCanales | Indica el número de canales que
.										se van a incluir en el archivo.
.						... | Aqui van parámetros variables. El orden en que
.							  se deben suministrar es:
.							  	- Cadena con título del canal
								- Tipo de dato de stTipoTDM
							  Los parámetros se deben separar por coma.
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_Crear(int iNumeroCanales, ...)
{
	stEdoTDM estado = TDMS_Correcto;	//estado de la funcion
	va_list pa;		//control de parametros variables
	int i=0;		//para uso del for
	
	//obtiene la lista de los tipos de datos a utilizar
	va_start(pa, iNumeroCanales);
	
	Tdms.iNumeroCanales = iNumeroCanales;
	Tdms.iTipoDatos = malloc(sizeof(stTipoTDM)*Tdms.iNumeroCanales);
	Tdms.pcTitulosCanales = malloc(sizeof(char*)*Tdms.iNumeroCanales);
	Tdms.CanalHandle = malloc(sizeof(TDMSChannelHandle)*Tdms.iNumeroCanales);
	
	for (i=0;i<Tdms.iNumeroCanales; i++)
	{
		Tdms.pcTitulosCanales[i] = malloc(sizeof(char)*100);
		Fmt(Tdms.pcTitulosCanales[i],"%s<%s",va_arg(pa, char *));
		Tdms.iTipoDatos[i] = va_arg(pa, stTipoTDM);
	}
	
	//necesita obtener el nombre de archivo siguiente
	TDM_SiguienteTDMS();
	
	//crea el nuevo archivo TDMS
	estado = TDM_CrearNuevoArchivo();
	
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_NombreArchivo
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Retorna el nombre del archivo que se ha creado.
. Parámetro de entrada:	char *pcNombre | referencia a la variable en donde se
.										 depositará el nombre del archivo
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_NombreArchivo(char *pcNombre)
{
	stEdoTDM estado = TDMS_Correcto;	//estado de la funcion
	int iError = 0;		//error de las funciones
	
	SetBreakOnLibraryErrors (0);
	iError = Fmt(pcNombre,"%s<%s",Tdms.cArchivoNuevo);	
	SetBreakOnLibraryErrors (1);
	
	if (iError < 0)
	{
		estado = TDM_Error("No se pudo guardar el nombre del archivo");
	}
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_Insertar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Esta función recibe la información de los canales
.						que se desean insertar en el archivo TDMS
. Parámetro de entrada:	int iNumeroDatos | Indica la cantidad de datos que se
										   incluyen en cada uno de los vectores
.						... | parámetros variables con los vectores a guardar
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_Insertar(int iNumeroDatos, ...)
{
	stEdoTDM estado = TDMS_Correcto;	//estado de la funcion
	va_list pa; 	//control de parametros
	void *vVector;	//recibe los vectores de cualquier tipo
	int i=0;	//para uso del for

	va_start(pa, iNumeroDatos);
	
	if (Tdms.iTdmsActivo == 1)
	{
		//Según el tipo de dato que se espera en la configuración, se utilizará
		//el apuntador destino
		for (i=0;i<Tdms.iNumeroCanales;i++)
		{
			vVector = va_arg(pa, void *); 
			TDMS_AppendDataValues (Tdms.CanalHandle[i], vVector,
							   iNumeroDatos, 1);	
		}
	}
	else
	{
		estado = TDM_Error("No se ha configurado aún la información del TDMS.");	
	}
	
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_Cerrar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Cierra el archivo TDMS activo
. Parámetro de entrada:	Ninguno
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_Cerrar()
{
	stEdoTDM estado = TDMS_Correcto;
	
	TDMS_CloseFile(Tdms.TdmsHandle);	
	TDM_LimpiaDatosAnteriores();
	
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_Abrir
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Habilita un archivo existente para el proceso de
.						lectura de sus canales.
. Parámetro de entrada:	char *pcArchivo | Nombre de archivo a abrir
.						unsigned int *piDatos | Retorna la cantidad de datos
.												disponibles para leer por canal
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_Abrir(char *pcArchivo, unsigned int *piDatos)
{
	unsigned __int64 iNumero=0;		//total de datoa por canal
	unsigned int iNumeroCanales=0;	//numero de canales en el archivo
	stEdoTDM estado = TDMS_Correcto;	//estado de la funcion
	TDMSDataType	tipoDato=0;			//tipo de dato de canal
	int iError=0;	//error de las funciones
	int i=0;	//para uso del for
	
	TDM_LimpiaDatosAnteriores();
	iError = TDMS_OpenFileEx (pcArchivo, 1, 0, &Tdms.TdmsHandle);
	if (iError == 0)
	{
		TDMS_GetFileProperty (Tdms.TdmsHandle, TDMS_FILE_NAME, Tdms.cNombre,
							  250);
		TDMS_GetFileProperty (Tdms.TdmsHandle, TDMS_FILE_DESCRIPTION,
							  Tdms.cDescripcion, 250);
		TDMS_GetFileProperty (Tdms.TdmsHandle, TDMS_FILE_TITLE, Tdms.cTitulo,
							  250);
		TDMS_GetFileProperty (Tdms.TdmsHandle, TDMS_FILE_AUTHOR, Tdms.cAutor,
							  250);
	
		//obtiene el grupo 
		TDMS_GetChannelGroups (Tdms.TdmsHandle, &Tdms.GrupoHandle, 1);
	
		//obtiene las referencias para el numero de canales existentes
		TDMS_GetNumChannels (Tdms.GrupoHandle, &iNumeroCanales);
		Tdms.iNumeroCanales = iNumeroCanales;
	
		Tdms.iTipoDatos = malloc(sizeof(stTipoTDM)*Tdms.iNumeroCanales);
		Tdms.pcTitulosCanales = malloc(sizeof(char*)*Tdms.iNumeroCanales);
		Tdms.CanalHandle = malloc(sizeof(TDMSChannelHandle)*Tdms.iNumeroCanales);
	
		TDMS_GetChannels(Tdms.GrupoHandle, Tdms.CanalHandle, iNumeroCanales);
	
		for (i=0;i<Tdms.iNumeroCanales; i++)
		{
			TDMS_GetDataType (Tdms.CanalHandle[i], &tipoDato);
		
			Tdms.iTipoDatos[i] = TDM_TipoDatoRev(tipoDato);
			Tdms.pcTitulosCanales[i] = malloc(sizeof(char)*100);
			TDMS_GetChannelProperty (Tdms.CanalHandle[i], TDMS_CHANNEL_NAME,
									 Tdms.pcTitulosCanales[i], 100);
			TDMS_GetNumDataValues (Tdms.CanalHandle[i], &iNumero);
		}
	
		Tdms.iTotalDatos = (unsigned int)iNumero;
		*piDatos = (unsigned int)iNumero;
		
		Tdms.iTdmsActivo = 1;
	}
	else
	{
		estado = TDM_Error(TDMS_GetLibraryErrorDescription(iError));
	}
	return estado;
}

/*****************************************************************************
.
. Función C:			TDM_Leer
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Obtiene información del archivo que se ha habilitado
.						para la lectura.
. Parámetro de entrada:	char *pcArchivo | Nombre de archivo a abrir
.						unsigned int *piDatos | Retorna la cantidad de datos
.												disponibles para leer por canal
. Parámetro de salida:	stEdoTDM | TDMS_Correcto, TDM_Error
. Fecha de creación:	31 de Marzo de 2014
.
*****************************************************************************/
stEdoTDM TDM_Leer(int iNumeroDatos, ...)
{
	stEdoTDM estado = TDMS_Correcto;	//estado de la funcion
	unsigned int iIndice;	//indice en el TDMS
	va_list pa;		//control de parametros variables
	void *vComun;	//apuntador void para los argumentos variables
	int i=0;		//para uso en el for
	
	va_start(pa, iNumeroDatos);	
	
	if (Tdms.iTdmsActivo == 1)
	{
	
		iIndice = va_arg(pa, unsigned int);
		Tdms.iIndice=iIndice;
	
		for (i=0;i<Tdms.iNumeroCanales; i++)
		{
			vComun = va_arg(pa, void *);
			TDMS_GetDataValuesEx (Tdms.CanalHandle[i], iIndice, iNumeroDatos,
								  vComun);
		}
	}
	else
	{
		estado = TDM_Error("No se ha abierto un archivo TDMS.");
	}
	return estado;
}
