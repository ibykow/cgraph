#include "common.h"

void free_bitfield(Bitfield *f)
{
    if(!f)
        return;

    if(f->field)
        free(f->field);

    free(f);
}

Bitfield *new_bitfield(unsigned num_bits)
{
    if(!num_bits)
        return 0;

    Bitfield *f = (Bitfield *) malloc(sizeof(Bitfield));
    if(!f)
        return 0;

    f->len = bitfield_index_of(num_bits) + 1u;
    f->field = (unsigned *) malloc(bitfield_size(f->len));
    if(!f->field) {
        free(f);
        return 0;
    }

    bitfield_wipe(f);

    return f;
}

void bitfield_write_all(Bitfield *f, unsigned v)
{
    if(!f)
        return;

    memset(f->field, v, sizeof(unsigned) * f->len);
}
