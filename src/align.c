//--------------------------------------------------------------------------
// Main
//#include "pthread_lib.h"

//__thread unsigned executing_hartid;
//unsigned n_harts = 0;

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "tileAcc.h"

int align(void)
{
  // single-threaded programs override this function.
  //printf("Hello, World from my MAIN !\n");

  //uint16_t ref_data[256][256];
  //uint16_t alt_data[480][480];
  uint16_t ref_data[64][64];
  uint16_t alt_data[48][48];
  uint16_t * ref_data_ptr;
  uint16_t level1_ref_sample[16][16];
  uint16_t level2_ref_sample[16][16];
  uint16_t level3_ref_sample[8][8];

  uint16_t level1_alt_sample[120][120];
  uint16_t level2_alt_sample[28][28];
  uint16_t level3_alt_sample[10][10];
  
  uint16_t i=0, j=0, n=0;
  uint16_t sampleAddr, shiftAddr, loadAddr;
  volatile uint64_t out;
  volatile uint64_t sample;
  volatile uint64_t tile_size = 16 | (0 << 5);
#ifdef DEBUG
  printf("entering the align function\n");
#endif

  for(i=0; i<16; i++){
    for(j=0; j<16; j++){
      ref_data[i][j] = i*256+j + 0x20;
    }
  }

  for(i=0; i<48; i++){
    for(j=0; j<48; j++){
      alt_data[i][j] = i*512+j + 0x10;
    }
  }

#ifdef DEBUG
  printf("initial tiles done!\n");
#endif
 
  FENCE();
  //----------downsample--------------
  doSetTile(tile_size);

  //first layer downsample
  for(i =0; i< 4; i++){
    ref_data_ptr = &ref_data[0][i * 16];
    //load the line-0 tile
    for(j=0 ; j<16; j++){
      doLoadRef(ref_data_ptr + j*64*sizeof(uint16_t) , j);
    }

    for (n=0; n <64-16; n++){
      loadAddr = (n+16)%32;
      sampleAddr = (n/4) % 8;
      shiftAddr = n % 4;
      //load the line+8
      doLoadRef(ref_data_ptr + (j+16)*64*sizeof(uint16_t) , loadAddr);
      
      //calc the line:0 downsample
      doBoxdown4Sample( sampleAddr | (shiftAddr << (16 + 2) ));
      doReadSample(level1_ref_sample[n][shiftAddr + i *4], sampleAddr);
    }
 
    //calc the line:56-63 downsample
    for(n=56; n<64; n++){
      sampleAddr = (n/4) % 16;
      shiftAddr = n % 4;
      doBoxdown4Sample( sampleAddr | (shiftAddr << (16 + 2) ));
      doReadSample(level1_ref_sample[n][shiftAddr + i *4], sampleAddr);
    }
  }


  //FENCE();
  tile_size = 16 | (4 << 5);
  doSetTile(tile_size);

  for(i=0; i<32; i++){
    doLoadRef(&(ref_data[i][0]), i);
  }
  for(i=0; i<40; i++){
    doLoadAlt(&(alt_data[i][1]), i);
  }

  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
     doSad((i << 16)| j);
     doReadSad(out, j);
#ifdef DEBUG
     printf("read sad[%d][%d] = %x\n", i, j, out);
#endif
    }
    doReduce(out)
#ifdef DEBUG
    printf("minimum sad is [%d] = %x\n",out & 0xFF, out >> 8);
#endif
  }

  return 0;
}
