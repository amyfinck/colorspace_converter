#include "arm_neon.h"

uint32x2_t aa, bb, ss;

int main()
{
    ss = vadd_u32( aa, bb);
}