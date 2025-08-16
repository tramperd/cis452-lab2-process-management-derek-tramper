/**********************************************************
 *
 * sampleProgram1.c
 * CIS 451 Lab 2
 *
 * !!!NAMES!!!
 *************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
   printf("Before fork\n");
   fork();
   printf("After fork\n");
   return 0;
}
