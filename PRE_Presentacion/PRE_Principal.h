/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1

#define  PANEL_2                          2
#define  PANEL_2_TOGGLEBUTTON             2       /* control type: textButton, callback function: (none) */

#define  pCatTransm                       3       /* callback function: PRE_PanelCatTransmisiones */
#define  pCatTransm_lstTransmisiones      2       /* control type: listBox, callback function: PRE_SeleccionCatTransmisiones */
#define  pCatTransm_cnvFondo              3       /* control type: canvas, callback function: (none) */
#define  pCatTransm_DECORATION            4       /* control type: deco, callback function: (none) */
#define  pCatTransm_txtNombreTransmision  5       /* control type: string, callback function: PRE_SeleccionCatTransmisiones */
#define  pCatTransm_txtTitulo             6       /* control type: textMsg, callback function: (none) */
#define  pCatTransm_picEditar             7       /* control type: picture, callback function: PRE_SeleccionCatTransmisiones */
#define  pCatTransm_picEliminar           8       /* control type: picture, callback function: PRE_SeleccionCatTransmisiones */
#define  pCatTransm_picGuardar            9       /* control type: picture, callback function: PRE_SeleccionCatTransmisiones */
#define  pCatTransm_picCancelar           10      /* control type: picture, callback function: PRE_SeleccionCatTransmisiones */
#define  pCatTransm_picCerrar             11      /* control type: picture, callback function: PRE_SeleccionCatTransmisiones */
#define  pCatTransm_numNumeroVelocidades  12      /* control type: numeric, callback function: PRE_SeleccionCatTransmisiones */
#define  pCatTransm_picAgregar            13      /* control type: picture, callback function: PRE_SeleccionCatTransmisiones */

#define  pEntrenar                        4       /* callback function: PRE_PanelEntrenamiento */
#define  pEntrenar_lstTransmisiones       2       /* control type: listBox, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_cnvFondo               3       /* control type: canvas, callback function: (none) */
#define  pEntrenar_DECORATION             4       /* control type: deco, callback function: (none) */
#define  pEntrenar_txtTitulo              5       /* control type: textMsg, callback function: (none) */
#define  pEntrenar_canDistribucionVel     6       /* control type: canvas, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picNuevo               7       /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picEliminar            8       /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picCerrar              9       /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picInciarEntrenamient  10      /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picCancelaEntrenamien  11      /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picAceptarPuntos       12      /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_tblEntrenamiento       13      /* control type: table, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picDescartarPuntos     14      /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_txtAyuda               15      /* control type: textBox, callback function: (none) */
#define  pEntrenar_picVelocidades         16      /* control type: picture, callback function: (none) */
#define  pEntrenar_picPruebaRecorrido     17      /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picAceptarConfiguraci  18      /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_picDetenerPrueba       19      /* control type: picture, callback function: PRE_SeleccionEntrenamiento */
#define  pEntrenar_txtTituloAyuda         20      /* control type: string, callback function: (none) */
#define  pEntrenar_numRotacionalMaximo    21      /* control type: numeric, callback function: (none) */
#define  pEntrenar_numRotacionalMinimo    22      /* control type: numeric, callback function: (none) */
#define  pEntrenar_numLinealMaximo        23      /* control type: numeric, callback function: (none) */
#define  pEntrenar_numLinealMinimo        24      /* control type: numeric, callback function: (none) */
#define  pEntrenar_btnActivaTimer         25      /* control type: command, callback function: ActivaTimer */
#define  pEntrenar_btnSetPosActual        26      /* control type: command, callback function: SetPosicionActual */
#define  pEntrenar_numNumeroCiclosR       27      /* control type: numeric, callback function: (none) */
#define  pEntrenar_numNumeroCiclos        28      /* control type: numeric, callback function: (none) */
#define  pEntrenar_numNumeroCuentas       29      /* control type: numeric, callback function: numeroCuentas */
#define  pEntrenar_numCorriente           30      /* control type: numeric, callback function: corrienteMotores */
#define  pEntrenar_numPausaMovimiento_2   31      /* control type: numeric, callback function: pausarMovimientoDes */
#define  pEntrenar_numPausaMovimiento     32      /* control type: numeric, callback function: pausarMovimiento */
#define  pEntrenar_numRPMAbb              33      /* control type: numeric, callback function: actualizarVelocidad */
#define  pEntrenar_numTiempoMovimiento    34      /* control type: numeric, callback function: (none) */
#define  pEntrenar_strRutaCiclo           35      /* control type: string, callback function: (none) */
#define  pEntrenar_strMensajeEspera       36      /* control type: string, callback function: (none) */
#define  pEntrenar_strCercaDe             37      /* control type: string, callback function: (none) */
#define  pEntrenar_txtCajaTexto           38      /* control type: textBox, callback function: (none) */
#define  pEntrenar_ledMotorABB            39      /* control type: LED, callback function: (none) */
#define  pEntrenar_ledPasoAPaso           40      /* control type: LED, callback function: (none) */
#define  pEntrenar_cnvDemo                41      /* control type: canvas, callback function: (none) */
#define  pEntrenar_binColor               42      /* control type: binary, callback function: (none) */
#define  pEntrenar_btnLeerPosiciones      43      /* control type: command, callback function: PRE_LeerPosiciones */
#define  pEntrenar_btnGuardarPosiciones   44      /* control type: command, callback function: PRE_GuardarPosiciones */
#define  pEntrenar_BINARYSWITCH           45      /* control type: binary, callback function: habilitarMotores */
#define  pEntrenar_tmrEliminar            46      /* control type: timer, callback function: PosicionActual */

#define  pMenu                            5
#define  pMenu_picModoManual              2       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_picCalibracion             3       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_picRegistros               4       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_picSalir                   5       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_picModoAutomatico          6       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_picSecuencias              7       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_picEntrenamiento           8       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_picTransmisiones           9       /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_picCerrar                  10      /* control type: picture, callback function: PRE_SeleccionMenu */
#define  pMenu_cnvFondoPanel              11      /* control type: canvas, callback function: PRE_SeleccionMenu */
#define  pMenu_cnvFondo                   12      /* control type: canvas, callback function: (none) */

#define  pPrincipal                       6       /* callback function: PRE_Principal */
#define  pPrincipal_txtEstado             2       /* control type: string, callback function: PRE_SeleccionPantallaPrincipal */
#define  pPrincipal_btnMenu               3       /* control type: picture, callback function: PRE_MostrarMenu */
#define  pPrincipal_picLinea              4       /* control type: picture, callback function: (none) */
#define  pPrincipal_PICTURE               5       /* control type: picture, callback function: (none) */
#define  pPrincipal_picLogoCidesi         6       /* control type: picture, callback function: (none) */
#define  pPrincipal_SPLITTER              7       /* control type: splitter, callback function: (none) */
#define  pPrincipal_TEXTMSG               8       /* control type: textMsg, callback function: (none) */
#define  pPrincipal_ledParoEmergencia     9       /* control type: LED, callback function: (none) */
#define  pPrincipal_ledBotonRearme        10      /* control type: LED, callback function: (none) */
#define  pPrincipal_ledEquipoPXI          11      /* control type: LED, callback function: (none) */
#define  pPrincipal_tmrLlamadas           12      /* control type: timer, callback function: PRI_Llamadas */
#define  pPrincipal_COMMANDBUTTON_3       13      /* control type: command, callback function: detener */
#define  pPrincipal_COMMANDBUTTON_2       14      /* control type: command, callback function: iniciarAdquisicion */
#define  pPrincipal_COMMANDBUTTON         15      /* control type: command, callback function: crearArchivo */
#define  pPrincipal_TEXTMSG_2             16      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ActivaTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK actualizarVelocidad(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK corrienteMotores(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK crearArchivo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK detener(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK habilitarMotores(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK iniciarAdquisicion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK numeroCuentas(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK pausarMovimiento(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK pausarMovimientoDes(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PosicionActual(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_GuardarPosiciones(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_LeerPosiciones(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_MostrarMenu(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_PanelCatTransmisiones(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_PanelEntrenamiento(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_Principal(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_SeleccionCatTransmisiones(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_SeleccionEntrenamiento(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_SeleccionMenu(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRE_SeleccionPantallaPrincipal(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PRI_Llamadas(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetPosicionActual(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
