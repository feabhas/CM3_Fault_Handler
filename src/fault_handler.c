#include "ARMCM3.h"
#include <stdio.h>

void printErrorMsg(const char * errMsg)
{
   while(*errMsg != '\0'){
      ITM_SendChar(*errMsg);
      ++errMsg;
   }
}

void HardFault_Handler(void)
{
   static char msg[80];
   printErrorMsg("In Hard Fault Handler\n");
   sprintf(msg, "SCB->HFSR = 0x%08x\n", SCB->HFSR);
   printErrorMsg(msg);
   __ASM volatile("BKPT #01");
   while(1);
}
