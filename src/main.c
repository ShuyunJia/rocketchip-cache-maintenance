//--------------------------------------------------------------------------
// Main
//#include "pthread_lib.h"

//__thread unsigned executing_hartid;
//unsigned n_harts = 0;

extern int printf(const char* fmt, ...);
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "tileAcc.h"
#include "plic.h"
#include "encoding.h"
//#define DEBUG

plic_instance_t plic0;


int main(int argc, char** argv);
int main(int argc, char** argv)
{
  // single-threaded programs override this function.
#ifdef DEBUG
  printf("Hello, World from my MAIN !\n");
#endif
  uint64_t i=0, j=0;
  volatile uint64_t out;
  uint64_t ref_data_addr = 0x80004000;
  volatile uint16_t ref_data[16][16];

  PLIC_init(&plic0, 0x0c000000, 2, 1);
  plic_source int_spi0 = 1;
  PLIC_enable_interrupt(&plic0, int_spi0);
  PLIC_set_priority(&plic0, int_spi0, 1);
  set_csr(mie,MIP_MEIP);

  for (int i=0;i<16;i++){
    for(int j=0; j<16; j++){
      //ref_data[i][j] = ref_data[i][j]+i*16+j;
      *((uint64_t *)ref_data_addr + i*16 +j)= i*16+j;
    }
  }

#ifdef DEBUG
  printf("start flush cache!\n");
  uint64_t addr = &ref_data[15][0];
  CFLUSH_D_L1(addr);
  printf("done flush cache!\n");
#endif
   
  return 0;
}
