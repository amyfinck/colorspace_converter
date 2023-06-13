#include "arm_neon.h"
#include <stdio.h>
#include <stdlib.h>

// two vectors of size 4
uint32x4_t aa, bb, ss;

int main( void )
{
    printf("hello\n");
    const uint32_t init_aa[4] = {1, 2, 3, 4};
    const uint32_t init_bb[4] = {5, 6, 7, 8};
    aa = vld1q_u32((uint32_t*)&init_aa);
    bb = vld1q_u32((uint32_t*)&init_bb);

    ss = vaddq_u32( aa, bb);
    int32_t* res_ptr = (int32_t*)&ss;

    printf("Result: %u, %u, %u, %u\n", res_ptr[0], res_ptr[1], res_ptr[2], res_ptr[3]);

    return 0;
}
