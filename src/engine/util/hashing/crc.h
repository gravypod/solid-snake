#ifndef ENGINE_UTIL_HASHING_CRC_H
#define ENGINE_UTIL_HASHING_CRC_H

#include <stdint.h>
#include <stddef.h>


/**
 * Hash a string using CRC32.
 * Algorithm from https://en.wikipedia.org/wiki/Cyclic_redundancy_check#CRC-32_algorithm
 *
 * @param data Pointer to data
 * @param length The size of the data block
 * @return Hash of the data
 */
uint_fast32_t hashing_crc32_make(const char *data, size_t length);

#endif
