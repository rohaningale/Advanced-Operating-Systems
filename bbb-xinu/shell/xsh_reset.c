/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - print Hello <string>, Welcome to the world of Xinu!!
 *------------------------------------------------------------------------
 */
shellcmd xsh_reset(int nargs, char *args[]) {
  volatile uint32 *prm_rstctrl;
  if(nargs > 1){
	fprintf(stderr, "%s: too many arguments\n", args[0]);
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "$ reset\n");
	return 1;
  }
  prm_rstctrl = (volatile uint32*) 0x44E00F00;  // getting the address of the register
  *prm_rstctrl = 0x01; // generate a warm software reset (setting the LSB to 1)
  return 0;
}
