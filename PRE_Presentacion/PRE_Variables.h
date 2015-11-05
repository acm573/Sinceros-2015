#ifndef __PRE_Principal__
#define __PRE_Principal__

#include "PRE_Principal.h"
#include "pre_tipos.h"
#include "tdm_archivos.h"
#include <NIDAQmx.h>




//prototipo de funciones
int PRE_InicializarInterfaz(void);
int PRE_AjustarComponentes(int iDato);
int PRE_ConfiguraMenu(void);
int PRE_AnimacionMenu(stMenuAcciones Accion);

int PRE_BarraEstado(char *pcCadena);
int PRI_MensajeControl(int panel, int control);
int PRE_FinalizarAplicacion(void);

int PRE_CatalogoTransmisiones(void);
int PRE_UbicarPanel(int iPanel);

int PRE_ResaltarOpcion(int iPanel, int iControl, int iFondo, int iCantidad, ...);
int PRE_PanelVisible(int iPanel);

int PRE_ControlesEntrenamiento(stEntrenar Modo);
int PRE_Desmarcar(int iControl);

int PRE_OcultarPanel(int iPanel);
int PRE_InicializarSistema(void);

int PRE_IniciarEntrenamiento(void);
int PRE_Finalizar(void);

int PRI_LlamadasRemotas(int (*funcionPaso)(void));

ListType miLista;
stPosicion MistPosicion;

//eliminar esta variable
double dPausaMovimientoEmb;
double dPausaMovimientoDes;

//DAQ
TaskHandle miTarea;

//Variable para incrementar cuentas
int iAumentarCuentas;

#endif
