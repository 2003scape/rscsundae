#ifndef BUFFER_H
#define BUFFER_H
#include <stdint.h>
#include <stddef.h>

int buf_getu8(void *, size_t, size_t, uint8_t *);
int buf_getu16(void *, size_t, size_t, uint16_t *);
int buf_getu32(void *, size_t, size_t, uint32_t *);
int buf_gets64(void *, size_t, size_t, int64_t *);

int buf_putu8(void *, size_t, size_t, uint8_t);
int buf_putu16(void *, size_t, size_t, uint16_t);
int buf_putu32(void *, size_t, size_t, uint32_t);
int buf_putbits(void *, size_t, size_t, int, int);
#endif
