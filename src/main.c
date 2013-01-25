#include "ARMCM3.h"

void SysTick_Handler(void)
{
   ITM_SendChar('.');
}

int main(void)
{
   SysTick_Config(SystemCoreClock/100);
   while(1);
}
