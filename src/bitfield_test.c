#include "common.h"

void bitfield_test()
{
    unsigned num_bits = 1000, i;
    Bitfield *f = new_bitfield(num_bits);
    fail(!f, "Couldn't create bitfield.");
    for(i = 0; i < num_bits; i++)
        fail(bitfield_check(f, i), "Bitfield is set when it shouldn't be.");

    bitfield_set(f, 10);
    fail(!bitfield_check(f, 10), "Low bit isn't set when it should be.");

    fail(bitfield_check(f, 11), "Low bit is set when it shouldn't be.");
    fail(bitfield_check(f, 11), "Low bit is set when it shouldn't be.");

    bitfield_set(f, 434);
    fail(!bitfield_check(f, 434), "Higher bit not set when it should be.");
    fail(!bitfield_check(f, 434), "Higher bit not set when it should be.");

    fail(bitfield_check(f, 555), "Higher bit is set when it shouldn't be.");
    fail(bitfield_check(f, 555), "Higher bit is set when it shouldn't be.");

    bitfield_fill(f);
    for(i = 0; i < num_bits; i++)
        fail(!bitfield_check(f, i), "Bitfield wasn't filled.");

    free_bitfield(f);
}
