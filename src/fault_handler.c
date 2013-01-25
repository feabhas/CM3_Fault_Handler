#include "ARMCM3.h"
#include <stdio.h>

void printErrorMsg(const char * errMsg);
void printUsageErrorMsg(uint32_t CFSRValue);

void HardFault_Handler(void)
{
   static char msg[80];
   printErrorMsg("In Hard Fault Handler\n");
   sprintf(msg, "SCB->HFSR = 0x%08x\n", SCB->HFSR);
   printErrorMsg(msg);
   if ((SCB->HFSR & (1 << 30)) != 0) {
      printErrorMsg("Forced Hard Fault\n");
      sprintf(msg, "SCB->CFSR = 0x%08x\n", SCB->CFSR );
      printErrorMsg(msg);
      if((SCB->CFSR & 0xFFFF0000) != 0) {
         printUsageErrorMsg(SCB->CFSR);
      }      
   }  
   __ASM volatile("BKPT #01");
   while(1);
}

void printErrorMsg(const char * errMsg)
{
   while(*errMsg != '\0'){
      ITM_SendChar(*errMsg);
      ++errMsg;
   }
}

void printUsageErrorMsg(uint32_t CFSRValue)
{
   printErrorMsg("Usage fault: ");
   CFSRValue >>= 16; // right shift to lsb
   
   if((CFSRValue & (1<<9)) != 0) {
      printErrorMsg("Divide by zero\n");
   }
}
