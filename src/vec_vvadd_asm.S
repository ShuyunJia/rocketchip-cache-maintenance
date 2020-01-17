## Hwacha v4 VVADD ASM code

#include "vec-util.h"

.text
.align 2

.globl vec_vvadd_asm
.type  vec_vvadd_asm,@function

# assumes calling convention:
# a0 has int n
# a1 has float* result  <---
# a2 has float* x
# a3 has float* y
vec_vvadd_asm:
    #li a4, VCFG(1, 4, 0, 1)
    li a4, VCFG(0, 4, 0, 2)
    vsetcfg a4

stripmine:
    vsetvl t0, a0 #a0 is requested vec len, actual is placed in t0
    vmca va0, a2
    vmca va1, a3
    vmca va2, a1
    la t5, vvadd_v
    vf 0(t5)

    slli t1, t0, 2
    add a1, a1, t1
    add a2, a2, t1
    add a3, a3, t1
    sub a0, a0, t0
    bnez a0, stripmine
    fence
    ret

# vector thread asm
.align 3
vvadd_v:
    vpset vp0
    #vcmpeq vp1, vv2, vs0
    vlw vv1, va0
    vlw vv2, va1
    #vfsub.h vv0, vv1, vv2
    #vcmpfeq.s vp1, vv1, vv2
    #vfsgnjn.s vv0, vv1, vv2
    vfcvt.d.l vv0, vv1
#@!vp1 vfirst vs1, vv1
    vsw vv0, va2
    #vssd vs1, vs0
    vstop
