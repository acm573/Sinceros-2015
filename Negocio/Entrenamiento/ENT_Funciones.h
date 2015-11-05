#include "pre_tipos.h"



//prototipo de funciones de entrenamiento
stEntrenar ENT_CargarTransmisiones(int iPanel, int iControl);
stPosiciones ENT_CargarPosiciones(int iPanel, int iControl, int iCanvas);
int ENT_AccionesIniciarEntrenamiento(int (*funcionPaso)(void));
int ENT_AccionesDetenerEntrenamiento(void);
int ENT_AccionesSegundoPasoEntrenamiento(int (*funcionPaso)(void), int (*funcionPaso2)(void));
int ENT_CalculaDesplazamientoPrueba(void);
char *ENT_NeutralActual(void);
int ENT_LimpiarDesplazamientoPrueba(void);
char *ENT_LeerDesplazamientoPrueba(void);
int ENT_EntrenamientoPaso1(void);
int ENT_EntrenamientoPaso2(void);
int ENT_EntrenamientoPaso3(void);
int ENT_ActualizarTabla(int iE1, int iE2);
int ENT_InformacionTabla(stTabla accion);
int ENT_PintarPuntos(stPintaMarcas Tipo,...);
int ENT_PruebaRecorrido(void);
stEdoEntrenamiento ENT_EstadoEntrenamiento(void);
int ENT_TorqueMotores(double dRotacional, double dLineal);
int ENT_ConfirmaInicioRecorridoPrueba(int (*funcionPaso)(void), int (*funcionPaso2)(void));
int ENT_AsignaEstadoEntrenamiento(stEdoEntrenamiento Tipo);
int ENT_AceptarPosicionTest01(int (*funcionPaso)(void), int (*funcionPaso2)(void));
int ENT_SiguientePaso(int (*funcionPaso)(void));
int ENT_ModoPasoAPaso(int (*funcionPaso)(void), int (*funcionPaso2)(int));


