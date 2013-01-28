#include "ARMCM3.h"
#include <stdio.h>

void printErrorMsg(const char * errMsg);
void printUsageErrorMsg(uint32_t CFSRValue);
void printBusFaultErrorMsg(uint32_t CFSRValue);
void printMemoryManagementErrorMsg(uint32_t CFSRValue);
void stackDump(uint32_t stack[]);

void Hard_Fault_Handler(uint32_t stack[])
{
   static char msg[80];
   //if((CoreDebug->DHCSR & 0x01) != 0) {
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
         if((SCB->CFSR & 0xFF00) != 0) {
            printBusFaultErrorMsg(SCB->CFSR);
         }
         if((SCB->CFSR & 0xFF) != 0) {
            printMemoryManagementErrorMsg(SCB->CFSR);
         }      
      }  
      stackDump(stack);
      __ASM volatile("BKPT #01");
   //}
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

void printBusFaultErrorMsg(uint32_t CFSRValue)
{
   printErrorMsg("Bus fault: ");
   CFSRValue = ((CFSRValue & 0x0000FF00) >> 8); // mask and right shift to lsb
}

void printMemoryManagementErrorMsg(uint32_t CFSRValue)
{
   printErrorMsg("Memory Management fault: ");
   CFSRValue &= 0x000000FF; // mask just mem faults
}

#if defined(__CC_ARM)
__asm void HardFault_Handler(void)
{
   TST lr, #4
   ITE EQ
   MRSEQ r0, MSP
   MRSNE r0, PSP
   B __cpp(Hard_Fault_Handler)
}
#elif defined(__ICCARM__)
void HardFault_Handler(void)
{
   __asm("TST lr, #4");
   __asm("ITE EQ");
   __asm("MRSEQ r0, MSP");
   __asm("MRSNE r0, PSP");
   __asm("B Hard_Fault_Handler");
}
#else
  #warning Not supported compiler type
#endif

enum { r0, r1, r2, r3, r12, lr, pc, psr};

void stackDump(uint32_t stack[])
{
   static char msg[80];
   sprintf(msg, "r0  = 0x%08x\n", stack[r0]);
   printErrorMsg(msg);
   sprintf(msg, "r1  = 0x%08x\n", stack[r1]);
   printErrorMsg(msg);
   sprintf(msg, "r2  = 0x%08x\n", stack[r2]);
   printErrorMsg(msg);
   sprintf(msg, "r3  = 0x%08x\n", stack[r3]);
   printErrorMsg(msg);
   sprintf(msg, "r12 = 0x%08x\n", stack[r12]);
   printErrorMsg(msg);
   sprintf(msg, "lr  = 0x%08x\n", stack[lr]);
   printErrorMsg(msg);
   sprintf(msg, "pc  = 0x%08x\n", stack[pc]);
   printErrorMsg(msg);
   sprintf(msg, "psr = 0x%08x\n", stack[psr]);
   printErrorMsg(msg);
}
