#include <NIDAQmx.h>


TaskHandle miHandle;


//Prototipo de funciones
int DIG_CrearTarea(int ipanel, int icontrol);
int DIG_Detener(void);
int DIG_Estado(int iBit);
double DIG_Tiempo(int iBit);
int DIG_Evento(stNumeroBit numeroBit, int (*funcionEvento)(stNumeroBit, stEstadoBit));
