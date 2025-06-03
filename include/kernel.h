#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "std_type.h" 


extern void _putInMemory(int segment, int address, byte value); 
extern int _interrupt_invoke(int interrupt_vector, int ax, int bx, int cx, int dx);
extern unsigned int getBiosTick(void); 


void setGlobalTextColor(byte color);
void printChar(char c); 
void printString(char* str);
void readString(char* buf);
void clearScreen();
char readKeyboardChar(); 



#endif // __KERNEL_H__
