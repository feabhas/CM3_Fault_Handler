#include "ARMCM3.h"
#include "div.h"

void SysTick_Handler(void)
{
   ITM_SendChar('.');
}

int main(void)
{
   int a = 10;
   int b = 0;
   int c;
   
   SysTick_Config(SystemCoreClock/100);
   
   c = div(a, b);
   
   while(1);
}
