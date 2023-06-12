#include "arm_neon.h"
#include <stdio.h>
#include <stdlib.h>

// two vectors of size 4
uint32x4_t aa, bb, ss;

int main( void )
{
    aa = {1, 2, 3, 4};
    bb = {5, 6, 7, 8};

    ss = vadd_u32( aa, bb);
    int32_t* res_ptr = (int32_t*)&result;

    printf("Result: %d, %d, %d, %d", res_ptr[0], res_ptr[1], res_ptr[2], res_ptr[3]);

    return 0;
}
