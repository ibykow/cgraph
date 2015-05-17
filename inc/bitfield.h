#ifndef BITFIELD_H_
#define BITFIELD_H_

typedef struct bitfield_s {
    unsigned *field, len, max;
} Bitfield;

#ifndef BYTE_SIZE
#define BYTE_SIZE 8
#endif

#define bitfield_wipe(f) bitfield_write_all(f, 0)
#define bitfield_fill(f) bitfield_write_all(f, ~0u)

void bitfield_write_all(Bitfield *f, unsigned v);

#define bitfield_index_of(n) ((unsigned) (n / (sizeof(unsigned) * BYTE_SIZE)))
#define bitfield_at(fp, bit) ((fp)->field[bitfield_index_of(bit)])
#define bitfield_size(len) (sizeof(unsigned) * len)

#define bitfield_set(fp, bit) (bitfield_at(fp, bit) |= \
    (1u << (bit % sizeof(unsigned))))

#define bitfield_unset(fp, bit) (bitfield_at(fp, bit) &= \
    ~(1u << (bit % sizeof(unsigned))))

#define bitfield_toggle(fp, bit) (bitfield_at(fp, bit) ^= \
    (1u << (bit % sizeof(unsigned))))

#define bitfield_check(fp, bit) (bitfield_at(fp, bit) & \
    (1u << (bit % sizeof(unsigned))))

void free_bitfield(Bitfield *f);
Bitfield *new_bitfield(unsigned num_bits);
void bitfield_write_all(Bitfield *f, unsigned v);

#endif
