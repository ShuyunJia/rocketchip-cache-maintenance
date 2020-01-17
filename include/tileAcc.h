// See LICENSE for license details.

#ifndef SRC_MAIN_C_TILE_H
#define SRC_MAIN_C_TILE_H

#include "xcustom.h"

#define k_DO_WRITE 0
#define k_DO_READ  1
#define k_DO_LOADREF 2
#define k_DO_LOADALT 3
#define k_DO_SAD 4
#define k_DO_REDUCE 5
#define k_DO_SHIFTLEFT 6
#define k_DO_READSAD 7
#define k_DO_SETTILE 8
#define k_DO_GAUSSSAMPLE 9
#define k_DO_BOXDOWN4SAMPLE 10
#define k_DO_READSAMPLE 11

#define XCUSTOM_ACC 0

#define doWrite(y, rocc_rd, data)                                       \
  ROCC_INSTRUCTION(XCUSTOM_ACC, y, data, rocc_rd, k_DO_WRITE);
#define doLoadRef(load_ptr, addr)                                       \
  ROCC_DIRECT_INSTRUCTION(XCUSTOM_ACC, load_ptr, addr, k_DO_LOADREF);
#define doLoadAlt(load_ptr, addr)                                       \
  ROCC_DIRECT_INSTRUCTION(XCUSTOM_ACC, load_ptr, addr, k_DO_LOADALT);
#define doSad(addr)                                       \
  ROCC_DIRECT_INSTRUCTION(XCUSTOM_ACC, 0, addr, k_DO_SAD);
#define doReduce(y)                                       \
  ROCC_INSTRUCTION(XCUSTOM_ACC, y, 0, 0, k_DO_REDUCE);
#define doShiftLeft()                                       \
  ROCC_DIRECT_INSTRUCTION(XCUSTOM_ACC, 0, 0, k_DO_SHIFTLEFT);
#define doSetTile(data)                                   \
  ROCC_DIRECT_INSTRUCTION(XCUSTOM_ACC, data, 0, k_DO_SETTILE);
#define doReadSad(y, addr)                                   \
  ROCC_INSTRUCTION(XCUSTOM_ACC, y, 0, addr, k_DO_READSAD);
#define doGaussSample(addr)                                   \
  ROCC_DIRECT_INSTRUCTION(XCUSTOM_ACC, 0, addr, k_DO_GAUSSSAMPLE);
#define doBoxdown4Sample(addr)                                   \
  ROCC_DIRECT_INSTRUCTION(XCUSTOM_ACC, 0, addr, k_DO_BOXDOWN4SAMPLE);
#define doReadSample(y, addr)                                   \
  ROCC_INSTRUCTION(XCUSTOM_ACC, y, 0, addr , k_DO_READSAMPLE);
#endif  // SRC_MAIN_C_TILE_H
