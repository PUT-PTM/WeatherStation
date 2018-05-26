// MPU-6050 h file

#include "stm32f10x.h"

#define SHT_Address_W		0x88	  //A0 -> GND
#define SHT_Address_R		0x89	  //A0 -> GND

void SHT_StartMeasure();
void SHT_Data_Read(void);
